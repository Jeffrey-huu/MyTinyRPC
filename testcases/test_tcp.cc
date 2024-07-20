#include <memory>
#include "src/common/log.h"
#include "src/net/tcp/net_addr.h"
#include "src/net/tcp/tcp_server.h"

void test_tcp_server() {
	MyTinyRPC::IPNetAddr::s_ptr addr = std::make_shared<MyTinyRPC::IPNetAddr>("127.0.0.1", 12346);

	DEBUGLOG("create addr %s", addr->toString().c_str());

	MyTinyRPC::TcpServer tcp_server(addr);

	tcp_server.start();
}

int main() {
	// MyTinyRPC::Config::SetGlobalConfig("../conf/MyTinyRPC.xml");
	// MyTinyRPC::Logger::InitGlobalLogger();

	MyTinyRPC::Config::SetGlobalConfig(NULL);
	MyTinyRPC::Logger::InitGlobalLogger(0);

	test_tcp_server();
}