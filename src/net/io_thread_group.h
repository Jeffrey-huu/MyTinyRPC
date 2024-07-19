#ifndef ROCKET_NET_IO_THREAD_GROUP_H
#define ROCKET_NET_IO_THREAD_GROUP_H

#include <vector>
#include "src/common/log.h"
#include "src/net/io_thread.h"

namespace MyTinyRPC {
	class IOThreadGroup {
		public:
			IOThreadGroup(int size);
			~IOThreadGroup();

			void start();
			void join();
			IOThread* getIOThread();

		private:
			int m_size {0};
			std::vector<IOThread*> m_io_thread_groups;
			int m_index {0};
			// not need mutex cause only the main thread will use IOThreadGroup
	};
}

#endif