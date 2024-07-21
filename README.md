# MyTinyRPC
This is a tiny RPC framework.

## The workspace structure
* bin: to save the test cases
* conf: to save the config xml file
* lib: to save the libmytinyrpc.a
* obj: to save the middle *.o file
* src: this is the source code folder
    * common: include the utilitis like log module, mutex, error code and config reading
    * net: include the rpc, tcp and eventloop module
* testcases: some test files

## The Log Module

## The TCP Module
1. Server: Main eventloop listens EPOLLIN by TCPAcceptor (socket->bind->listen), and excutes `onAccept` when a new tcp request comes. Allocate the IO events to the Sub eventloops. To the end of server: 
    * onRead: only read the data and put into in_buffer, call excute.
    * excute: decode the data from in_buffer, process RPC request, encode the data into out_buffer, call onWrite.
    * onWrite: only write the data from out_buffer.

2. Client: has only one eventloop, package the connect method in the asynchronous way. After successfully connect to the server, call the read and write method (also asynchronously):
    * onRead: only read the data and put into in_buffer, call excute.
    * excute: decode the data from in_buffer, excute the done callback.
    * onWrite: encode the data into out_buffer and then write the data from out_buffer.


