#ifndef MyTinyRPC_NET_TCP_SERVER_H
#define MyTinyRPC_NET_TCP_SERVER_H

#include <set>
#include "src/net/tcp/tcp_acceptor.h"
#include "src/net/tcp/tcp_connection.h"
#include "src/net/tcp/net_addr.h"
#include "src/net/eventloop.h"
#include "src/net/io_thread_group.h"

namespace MyTinyRPC {
	class TcpServer {
		public:
			TcpServer(NetAddr::s_ptr local_addr);
			~TcpServer();
			void start();

		private:
			void init();
			// 当有新客户端连接之后需要执行
			void onAccept();
			// 清除 closed 的连接
			void ClearClientTimerFunc();

			TcpAcceptor::s_ptr m_acceptor;
			NetAddr::s_ptr m_local_addr;    // 本地监听地址
			EventLoop* m_main_event_loop {NULL};    // mainReactor
			IOThreadGroup* m_io_thread_group {NULL};   // subReactor 组
			FdEvent* m_listen_fd_event;
			int m_client_counts {0};
			std::set<TcpConnection::s_ptr> m_client;
			TimerEvent::s_ptr m_clear_client_timer_event;
	};
}

#endif