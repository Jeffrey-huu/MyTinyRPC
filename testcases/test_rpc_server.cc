#include <assert.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <memory>
#include <unistd.h>
#include <google/protobuf/service.h>
#include "src/common/log.h"
#include "src/common/config.h"
#include "src/common/log.h"
#include "src/net/tcp/tcp_client.h"
#include "src/net/tcp/net_addr.h"
#include "src/net/coder/string_coder.h"
#include "src/net/coder/abstract_protocol.h"
#include "src/net/coder/tinypb_coder.h"
#include "src/net/coder/tinypb_protocol.h"
#include "src/net/tcp/net_addr.h"
#include "src/net/tcp/tcp_server.h"
#include "src/net/rpc/rpc_dispatcher.h"

#include "order.pb.h"

class OrderImpl : public Order {
	public:
	void makeOrder(google::protobuf::RpcController* controller,
						const ::makeOrderRequest* request,
						::makeOrderResponse* response,
						::google::protobuf::Closure* done) {
		APPDEBUGLOG("start sleep 5s");
		// sleep(5);
		APPDEBUGLOG("end sleep 5s");
		if (request->price() < 10) {
			response->set_ret_code(-1);
			response->set_res_info("short balance");
			return;
		}
		response->set_order_id("20230514");
		APPDEBUGLOG("call makeOrder success");
		if (done) {
			done->Run();
			delete done;
			done = NULL;
		}
	}
};


int main(int argc, char* argv[]) {
	if (argc != 2) {
		printf("Start test_rpc_server error, argc not 2 \n");
		printf("Start like this: \n");
		printf("./test_rpc_server ../conf/MyTinyRPC.xml \n");
		return 0;
	}

	MyTinyRPC::Config::SetGlobalConfig(argv[1]);

	MyTinyRPC::Logger::InitGlobalLogger();

	std::shared_ptr<OrderImpl> service = std::make_shared<OrderImpl>();
	MyTinyRPC::RpcDispatcher::GetRpcDispatcher()->registerService(service);

	MyTinyRPC::IPNetAddr::s_ptr addr = std::make_shared<MyTinyRPC::IPNetAddr>("127.0.0.1", MyTinyRPC::Config::GetGlobalConfig()->m_port);

	MyTinyRPC::TcpServer tcp_server(addr);

	tcp_server.start();

	return 0;
}