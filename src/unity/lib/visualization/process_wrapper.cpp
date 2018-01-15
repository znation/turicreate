/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */

#undef CHECK
#include <unity/lib/visualization/tcviz.pb.h>
#include <logger/assertions.hpp>
#include <logger/logger.hpp>

#include "process_wrapper.hpp"

#include <logger/assertions.hpp>
#include <logger/logger.hpp>

#include <cassert>
#include <chrono>
#include <stdexcept>
#include <sstream>
#include <string>

#include <errno.h>

using namespace turi::visualization;

process_wrapper::process_wrapper(const std::string& path_to_client) : m_alive(true) {
  // constructor
  // instantiate visualization client process
  m_client_process.popen(path_to_client.c_str(),
                  std::vector<std::string>(),
                  STDOUT_FILENO,
                  true /* open_write_pipe */);
  if (!m_client_process.exists()) {
    throw std::runtime_error("Turi Create visualization process was unable to launch.");
  }
  m_client_process.autoreap();

  // start the background threads to pull and push data over the pipe
  m_inputThread = std::thread([this]() {
    std::string previousInputRemaining;
    while (good()) {
      // read number of bytes from first 4 bytes (32 bits of message size)
      uint32_t message_size;
      ssize_t bytes_read = m_client_process.read_from_child(&message_size, 4);
      DASSERT_EQ(bytes_read, 4);

      // read message_size bytes
      std::string input;
      input.resize(message_size);

      // const cast on std::string data() should be safe now
      // see https://stackoverflow.com/questions/7518732/why-are-stdvectordata-and-stdstringdata-different#7519345
      bytes_read = m_client_process.read_from_child(const_cast<void *>(static_cast<const void *>(input.data())), message_size);
      DASSERT_EQ(bytes_read, message_size);

      // decode a Message and write it to input buffer
      std::shared_ptr<Message> spec = std::make_shared<Message>();
      if (!spec->ParseFromString(input)) {
        throw std::runtime_error("Could not decode a valid visualization message from input.");
      }

      // we have a valid message, put it in the queue
      m_inputBuffer.write(spec);
    }
  });
  m_outputThread = std::thread([this]() {
    std::unique_lock<mutex> guard(m_mutex);
    while (true) {
      while (m_client_process.exists() && m_outputBuffer.size() > 0) {
        std::shared_ptr<Message> output = m_outputBuffer.read();
        std::string output_str;
        if (!output->SerializeToString(&output_str)) {
          throw std::runtime_error("Could not encode visualization spec to string.");
        }

        // write size (4 bytes)
        DASSERT_LE(output_str.size(), std::numeric_limits<uint32_t>::max());
        uint32_t message_size = static_cast<uint32_t>(output_str.size());
        ssize_t bytes_written = m_client_process.write_to_child(&message_size, 4);
        DASSERT_EQ(bytes_written, 4);
        
        // write output and compare size
        bytes_written = m_client_process.write_to_child(output_str.c_str(), message_size);
        DASSERT_EQ(bytes_written, message_size);
      }
      if (!good()) {
        break;
      }
      m_cond.wait(guard);
    }
  });

  // workaround to pop-under GUI app window from popen:
  // https://stackoverflow.com/a/13553471
  ::turi::process osascript;
  osascript.popen("/usr/bin/osascript",
      std::vector<std::string>({
        "-e",
        "delay .5",
        "-e",
        "tell application \"Turi Create Visualization\" to activate"
      }),
      0, false);
}

process_wrapper::~process_wrapper() {
  // clean up member threads
  {
    std::lock_guard<mutex> guard(m_mutex);
    m_alive = false;
    m_cond.signal();
  }
  m_inputThread.join();
  m_outputThread.join();
}

process_wrapper& process_wrapper::operator<<(const std::shared_ptr<Message>& to_client) {
  // TODO - error handling?
  if (good()) {
    std::lock_guard<mutex> guard(m_mutex);
    m_outputBuffer.write(to_client);
    m_cond.signal();
  }
  return *this;
}

process_wrapper& process_wrapper::operator>>(std::shared_ptr<Message>& from_client) {
  // TODO - error handling?
  if (good()) {
    from_client = m_inputBuffer.read();
  }
  return *this;
}

bool process_wrapper::good() {
  return m_alive && m_client_process.exists();
}
