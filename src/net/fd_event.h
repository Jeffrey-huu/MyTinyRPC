
#ifndef MyTinyRPC_NET_FDEVENT_H
#define MyTinyRPC_NET_FDEVENT_H

#include <functional>
#include <sys/epoll.h>

namespace MyTinyRPC {
	class FdEvent {
		public:
			enum TriggerEvent {
				IN_EVENT = EPOLLIN,
				OUT_EVENT = EPOLLOUT,
				ERROR_EVENT = EPOLLERR,
			};

			FdEvent(int fd);
			FdEvent();
			~FdEvent();

			void setNonBlock();
			std::function<void()> handler(TriggerEvent event_type);
			// 监听，并给出对应事件的处理回调函数
			void listen(TriggerEvent event_type, std::function<void()> callback, std::function<void()> error_callback = nullptr);
			void cancel(TriggerEvent event_type);

			int getFd() const {
				return m_fd;
			}
			epoll_event getEpollEvent() {
				return m_listen_events;
			}

		protected:
			int m_fd {-1};
			epoll_event m_listen_events;

			std::function<void()> m_read_callback {nullptr};
			std::function<void()> m_write_callback {nullptr};
			std::function<void()> m_error_callback {nullptr};
	};
}

#endif