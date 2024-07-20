#ifndef MyTinyRPC_NET_FD_EVENT_GROUP_H
#define MyTinyRPC_NET_FD_EVENT_GROUP_H

#include <vector>
#include "src/common/mutex.h"
#include "src/net/fd_event.h"

namespace MyTinyRPC {
	class FdEventGroup {
		public:
			FdEventGroup(int size);
			~FdEventGroup();
			
			FdEvent* getFdEvent(int fd);
			static FdEventGroup* GetFdEventGroup();

		private:
			int m_size {0};
			std::vector<FdEvent*> m_fd_group;
			Mutex m_mutex;
	};
}

#endif