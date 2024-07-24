#ifndef MyTinyRPC_NET_RPC_RPC_DISPATCHER_H
#define MyTinyRPC_NET_RPC_RPC_DISPATCHER_H

#include <map>
#include <memory>
#include <google/protobuf/service.h>

#include "src/net/coder/abstract_protocol.h"
#include "src/net/coder/tinypb_protocol.h"

namespace MyTinyRPC {
	class TcpConnection;

	class RpcDispatcher {
		public:
			static RpcDispatcher* GetRpcDispatcher();

			typedef std::shared_ptr<google::protobuf::Service> service_s_ptr;

			void dispatch(AbstractProtocol::s_ptr request, AbstractProtocol::s_ptr response, TcpConnection* connection);
			void registerService(service_s_ptr service);
			void setTinyPBError(std::shared_ptr<TinyPBProtocol> msg, int32_t err_code, const std::string err_info);

		private:
			bool parseServiceFullName(const std::string& full_name, std::string& service_name, std::string& method_name);
			
			std::map<std::string, service_s_ptr> m_service_map;
	};
}

#endif