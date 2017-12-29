/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef __TC_PROCESS_WRAPPER
#define __TC_PROCESS_WRAPPER

#include "io_buffer.hpp"

#include <cstdio>
#include <string>
#include <thread>
#include <platform/parallel/pthread_tools.hpp>
#include <process/process.hpp>

namespace turi {
  namespace visualization {

    class Message;

    class process_wrapper {
      private:
        typedef io_buffer<std::shared_ptr<Message>> spec_buffer;

        volatile bool m_alive;
        turi::mutex m_mutex;
        turi::conditional m_cond;
        ::turi::process m_client_process;
        spec_buffer m_inputBuffer;
        std::thread m_inputThread;
        spec_buffer m_outputBuffer;
        std::thread m_outputThread;

      public:
        explicit process_wrapper(const std::string& path_to_client);
        ~process_wrapper();
        process_wrapper& operator<<(std::shared_ptr<Message> to_client);
        process_wrapper& operator>>(std::shared_ptr<Message>& from_client);
        bool good();
    };

  }
}

#endif // __TC_PROCESS_WRAPPER
