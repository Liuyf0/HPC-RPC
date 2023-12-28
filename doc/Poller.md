# Poller类

网络编程中想要监听多个事件做常用的就是I/O复用技术，select/poll/epoll，网络库模块对Linux下的epoll提供了封装，对应的子类为EPollPoller。

先来看看基类Poller定义：

```c++
/**
* IO复用接口
* 禁止编译器生成copy构造函数和copy assignment
* 支持poll(2), epoll(7)
*/
class Poller : noncopyable
{
public:
    typedef std::vector<Channel*> ChannelList;
    explicit Poller(EventLoop* loop);
    virtual ~Poller();
    /*
     * 监听函数，根据激活的通道列表，监听指定fd的相应事件
     * 对于EPollPoller会调用epoll_wait(2), 对于PollPoller会调用poll(2)
     *
     * 返回调用完epoll_wait/poll的当前时间（Timestamp对象）
     */
    virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;

    /* 更新监听通道的事件 */
    virtual void updateChannel(Channel* channel) = 0;
    /* 删除监听通道 */
    virtual void removeChannel(Channel* channel) = 0;
    /* 判断当前Poller对象是否持有指定通道 */
    virtual bool hasChannel(Channel* channel) const;

    /* 默认创建Poller对象的类函数 */
    static Poller* newDefaultPoller(EventLoop* loop);
    /*
     * 断言所属EventLoop为当前线程.
     * 如果断言失败，将终止程序（LOG_FATAL）
     */
    void assertInLoopThread() const
    {
        ownerLoop_->assertInLoopThread();
    }
protected:
    /*
     * 该类型保存fd和需要监听的events，以及各种事件回调函数（可读/可写/错误/关闭等）
     */
    typedef std::map<int, Channel*> ChannelMap;
    // Poller don't own the Channel, so the channel must be unregister(EventLoop::removeChannel) before its dtor.
    // std::map used for speeding up to find out a channel by fd
    /* 保存所有事件的Channel，一个Channel绑定一个fd */
    ChannelMap channels_;

private:
    /*
     * 事件驱动循环, 用于调用poll监听fd事件
     */
    EventLoop* ownerLoop_;
};
```

只有拥有EventLoop的IO线程，才能调用EventLoop所拥有的Poller对象的接口，因此考虑Poller的线程安全不是必要的。

一个Channel对应一个fd（文件描述符），一个fd有三种事件状态：空事件（kNoneEvent），读事件（kReadEvent，即POLLIN | POLLPRI），写事件（kWriteEvent，即POLLOUT）。只有后2个，poll/epoll才会进行监听。

EventLoop会根据Poller::newDefaultPoller()，Poller对象。实际策略是根据是否设置了环境变量，来选择创建PollPoller，还是EPollPoller。

**newDefaultPoller**

正常情况下，我们可能会在 `Poller.cpp` 文件中完成该成员函数的实现。但是这并不是一个好的设计，因为 Poller 是一个基类。如果在 `Poller.cpp` 文件内实现则势必会在 `Poller.cpp`包含 `EPollPoller.h` 等头文件。在一个基类中包含其派生类的头文件，这个设计可以说是很诡异的，这并不是一个好的抽象。

因此，我们专门设置了另一个 `DefaultPoller.cpp` 文件，在其中包含了 `Poller.h` 和 `EPollPoller.h` 的头文件。这样就让 `Poller.h` 文件显得正常了。

```c++
Poller *Poller::newDefaultPoller(EventLoop *loop) // static
{
    if (::getenv("MUDUO_USE_POLL")) // 如果设置了环境变量
    {
        return new PollPoller(loop);
    }
    else
    {
        return new EPollPoller(loop);
    }
    return nullptr;
}
```

---

# 派生类EPollPoller

EPollPoller 以epoll为核心，实现了基类Poller的virtual函数，在其中调用了epoll_create/ctl/wait等接口。poll返回后，会将就绪的fd添加到激活队列activeChannels中管理。

```c++
/**
* IO Multiplexing with epoll(7).
*/
class EPollPoller : public Poller
{
public:
    EPollPoller(EventLoop* loop);
    ~EPollPoller() override;
    /* 监听函数, 调用epoll_wait() */
    Timestamp poll(int timeoutMs, ChannelList* activeChannels) override;
    /* ADD/MOD/DEL */
    void updateChannel(Channel* channel) override;
    /* DEL */
    void removeChannel(Channel* channel) override;

private:
    /* events_数组初始大小 */
    static const int kInitEventListSize = 16;
    /* 将op(EPOLL_CTL_Add/MOD/DEL)转换成字符串 */
    static const char* operationToString(int op);
    /* poll返回后将就绪的fd添加到激活通道中activeChannels */
    void fillActiveChannels(int numEvents,
                            ChannelList* activeChannels) const;
    /* 由updateChannel/removeChannel调用，真正执行epoll_ctl()控制epoll的函数 */
    void update(int operation, Channel* channel);

    typedef std::vector<struct epoll_event> EventList;
    /* epoll文件描述符，由epoll_create返回 */
    int epollfd_;
    /* epoll事件数组，为了适配epoll_wait参数要求 */
    EventList events_;
};
```

muduo在实现时，创建epoll fd时，并没有用epoll_create，而是用 epoll_create1。原因在于： epoll_create1在打开epoll文件描述符时，可以直接指定FD_CLOEXEC选项，相当于open时指定O_CLOSEXEC。另外，epoll_create的size参数在Linux2.6.8以后，就已经没用了（>0即可），内核会实现自动增长内部数据结构以描述监听事件。

值得一提的是，在Channel中定义了一个名为index_的成员，由Channel构造初值为0，可通过Channel::index()/set_index()访问，在不同的Poller中有不同的含义：在EPollPoller中，index_用来表示事件类型（kNew/kAdded/kDeleted）。

# 参考

[muduo笔记 网络库（二）I/O复用封装Poller - 明明1109 - 博客园 (cnblogs.com)](https://www.cnblogs.com/fortunely/p/15997621.html)
