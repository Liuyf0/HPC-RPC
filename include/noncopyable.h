/**
 *@description: noncopyable
 *@author: NAIUI
 *@date: 2024-01-30
 */

#pragma once

/**
 * noncopyable被继承以后，派生类对象可以正常的构造和析构，
 * 派生类对象不能拷贝构造和赋值操作
 */
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default;
    ~noncopyable() = default;
};