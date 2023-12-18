# 值语义与引用语义

值语义指的是对象的拷贝与元对象无关，就像拷贝int一样。C++的内置类型（bool/int/double/char）都是值语义，标准库里的complex<>,pair<>,vector<>,map<>等等也都是值语义，拷贝之后就与原对象脱离关系。

与值语义对应的对象语义，或者叫做引用语义，对象语义指的是面向对象意义下的对象，对象拷贝是禁止的。

# 如何为一个class实现值语义，引用语义？

《深度探索C++对象模型》中提到，当构造一个class对象时，会先bit-wise构造其数据成员。而继承的base class那部分，会被派生类隐式继承，作为派生类数据成员。

形如下面的代码，编译器会将base class对象作为derive class的数据成员。

```C++
class A
{
public:
    ...
private:
   int a;
};

class B : A
{
public:
    ...
private:
    int b;
    A a; // 编译器自动生成
}
```

如果A的构造函数（ctor）是private，编译器就无法在B中构造base class，即A那部分。这样，编译器也就无法为B合成构造函数。
copy操作、move操作也是如此。

因此，我们可以为定义2个标记class，其他类继承这2个标记类，用于表示是否支持copy操作。

# 阻止copy操作

为什么不直接使用C++ 11关键字default/delete，指定支持/阻止编译器合成相关ctor、copy操作、move操作？
答案是当然可以，default/delete 能达到同样目的，但不像继承自copyable、noncopyable这种标记类一样作用明显，程序员一眼都能看出其特性：是否允许copy。
