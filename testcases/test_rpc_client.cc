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
#include "src/net/rpc/rpc_controller.h"
#include "src/net/rpc/rpc_channel.h"
#include "src/net/rpc/rpc_closure.h"

#include "order.pb.h"

void test_rpc_channel() {
	NEWRPCCHANNEL("127.0.0.1:12345", channel);
	NEWMESSAGE(makeOrderRequest, request);
	NEWMESSAGE(makeOrderResponse, response);

	request->set_price(100);
	request->set_goods("apple");

	NEWRPCCONTROLLER(controller);
	controller->SetMsgId("99998888");
	// controller->SetTimeout(10000);

	std::shared_ptr<MyTinyRPC::RpcClosure> closure = std::make_shared<MyTinyRPC::RpcClosure>(nullptr, [request, response, channel, controller]() mutable {
		if (controller->GetErrorCode() == 0) {
			INFOLOG("call rpc success, request[%s], response[%s]", request->ShortDebugString().c_str(), response->ShortDebugString().c_str());
			// 执行业务逻辑
			if (response->order_id() == "xxx") {
				// xx
			}
		} else {
			ERRORLOG("call rpc failed, request[%s], error code[%d], error info[%s]", 
				request->ShortDebugString().c_str(), 
				controller->GetErrorCode(), 
				controller->GetErrorInfo().c_str());
		}
	
		INFOLOG("now exit eventloop");
		channel->getTcpClient()->stop();
		channel.reset();
		// std::cout << "use count: " << channel.use_count() << std::endl;
		// exit(0);
	});
	
	channel->Init(controller, request, response, closure);
	Order_Stub(channel.get()).makeOrder(controller.get(), request.get(), response.get(), closure.get());
}

int main() {
	MyTinyRPC::Config::SetGlobalConfig(NULL);

	MyTinyRPC::Logger::InitGlobalLogger(0);

	test_rpc_channel();

	INFOLOG("test_rpc_channel end");

	return 0;
}