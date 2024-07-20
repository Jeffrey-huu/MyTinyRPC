#ifndef MyTinyRPC_NET_WAKEUP_FDEVENT_H
#define MyTinyRPC_NET_WAKEUP_FDEVENT_H

#include "src/net/fd_event.h"

namespace MyTinyRPC {
	class WakeUpFdEvent : public FdEvent {
		public:
			WakeUpFdEvent(int fd);
			~WakeUpFdEvent();
			void wakeup();
	};
}
#endif