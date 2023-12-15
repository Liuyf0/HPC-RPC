## Buffer类的设计

* **非阻塞网络编程中应用层buffer是必须的**
  原因：非阻塞IO的核心思想是避免阻塞在read()或write()或其他IO系统调用上，这样可以最大限度复用thread-of-control，让一个线程能服务于多个socket连接。IO线程只能阻塞在IO-multiplexing函数上，如select()/poll()/epoll_wait()。这样一来，应用层的缓冲是必须的，每个TCP socket都要有input buffer和output buffer。
* **TcpConnection必须有output buffer**
  原因：使程序在write()操作上不会产生阻塞，当write()操作后，操作系统一次性没有接受完时，网络库把剩余数据则放入output buffer中，然后注册POLLOUT事件，一旦socket变得可写，则立刻调用write()进行写入数据。——应用层buffer到操作系统buffer
* **TcpConnection必须有input buffer**
  原因：当发送方send数据后，接收方收到数据不一定是整个的数据，网络库在处理socket可读事件的时候，必须一次性把socket里的数据读完，否则会反复触发POLLIN事件，造成busy-loop。所以网路库为了应对数据不完整的情况，收到的数据先放到input buffer里。——操作系统buffer到应用层buffer
