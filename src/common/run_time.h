#ifndef ROCKET_COMMON_RUN_TIME_H
#define ROCKET_COMMON_RUN_TIME_H

#include <string>

namespace MyTinyRPC {
	class RpcInterface;

	class RunTime {
		public:
			RpcInterface* getRpcInterface();
			static RunTime* GetRunTime();
			std::string m_msgid;
			std::string m_method_name;
			RpcInterface* m_rpc_interface {NULL};
	};
}

#endif