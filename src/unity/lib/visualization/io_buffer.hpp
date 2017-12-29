/* Copyright © 2017 Apple Inc. All rights reserved.
 *
 * Use of this source code is governed by a BSD-3-clause license that can
 * be found in the LICENSE.txt file or at https://opensource.org/licenses/BSD-3-Clause
 */
#ifndef __TC_VISUALIZATION_IO_BUFFER
#define __TC_VISUALIZATION_IO_BUFFER

#include <mutex>
#include <string>
#include <queue>

namespace vega {
  class Specification;
}

namespace turi {
  namespace visualization {

    template<typename T>
    class io_buffer {
      private:
        std::mutex m_mutex;
        std::queue<T> m_queue;

      public:
        T read() {
          std::lock_guard<std::mutex> lock(m_mutex);
          if (m_queue.empty()) {
            return T();
          }
          auto ret = m_queue.front();
          m_queue.pop();
          return ret;
        }

        void write(const T& t) {
          std::lock_guard<std::mutex> lock(m_mutex);
          m_queue.push(t);
        }

        size_t size() const {
          return m_queue.size();
        }
    };
  }
}

#endif // __TC_VISUALIZATION_IO_BUFFER
