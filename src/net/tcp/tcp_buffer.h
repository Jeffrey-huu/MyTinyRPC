#ifndef MyTinyRPC_NET_TCP_TCP_BUFFER_H
#define MyTinyRPC_NET_TCP_TCP_BUFFER_H

#include <vector>
#include <memory>

namespace MyTinyRPC {
	class TcpBuffer {
		public:
			typedef std::shared_ptr<TcpBuffer> s_ptr;

			std::vector<char> m_buffer;

			TcpBuffer(int size);
			~TcpBuffer();

			// 返回可读字节数
			int readAble();
			// 返回可写的字节数
			int writeAble();
			int readIndex();
			int writeIndex();
			void writeToBuffer(const char* buf, int size);
			void readFromBuffer(std::vector<char> &re, int size);
			void resizeBuffer(int new_size);
			void adjustBuffer();
			void moveReadIndex(int size);
			void moveWriteIndex(int size);

		private:
			int m_read_index {0};
			int m_write_index {0};
			int m_size {0};
	};
}

#endif