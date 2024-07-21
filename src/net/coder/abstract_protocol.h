#ifndef MyTinyRPC_NET_ABSTRACT_PROTOCOL_H
#define MyTinyRPC_NET_ABSTRACT_PROTOCOL_H

#include <memory>

namespace MyTinyRPC {
	struct AbstractProtocol : public std::enable_shared_from_this<AbstractProtocol> {
		public:
			typedef std::shared_ptr<AbstractProtocol> s_ptr;
			virtual ~AbstractProtocol() {}
			std::string m_msg_id;     // 请求号，唯一标识一个请求或者响应
	};
}

#endif
