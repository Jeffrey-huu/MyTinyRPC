#ifndef MyTinyRPC_COMMON_MSGID_UTIL_H
#define MyTinyRPC_COMMON_MSGID_UTIL_H

#include <string>

namespace MyTinyRPC {
	class MsgIDUtil {
		public:
			static std::string GenMsgID();
	};
}

#endif