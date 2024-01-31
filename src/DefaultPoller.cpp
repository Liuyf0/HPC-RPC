/**
 *@description: DefaultPoller.cpp
 *@author: NAIUI
 *@date: 2024-01-30
 */

#include "Poller.h"
#include "EPollPoller.h"

Poller* Poller::newDefaultPoller(EventLoop *loop)
{
    if (::getenv("USE_POLL"))
    {
        return nullptr; // 生成poll的实例
    }
    else
    {
        return new EPollPoller(loop);   // 生成epoll的实例
    }
}