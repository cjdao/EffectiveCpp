## 条款09：绝不在构造和析构过程中调用virtual函数

### 为嘛呢？
首先我们可以得出的结论是，一个类如果有其成员函数是virtual的，那说明它就有可能会作为一个继承体系的父类而存在，而该virtual成员函数也极有可能被重写。  
而在这种情况中，在父类的构造或者析构过程中调用virtual函数会怎么样呢？我们来看看书上给出来的例子:  
```cpp
//这是一个父类
class Transaction{
public:
    Transaction();
    virtual void logTransaction() const = 0;  // virtual 成员函数
};
// 父类在构造过程中，调用virtual函数
Transaction::Transaction() 
{
    logTransaction();
}

// 其中的一个子类
class BuyTransaction:public Transaction{
public:
    // 子类实现父类的virtual函数
    virtual void logTransaction() const {//...}; 
};

// 那么下面的代码将会有问题
{
    BuyTransaction b; // ERROR！链接错误！
}
```
上述的链接错误是因为，在构造BuyTransaction前其父类Transaction的构造函数会先被调用，而Transaction的构造函数调用了logTransaction函数，它这时还是一个pure virtual函数，所以链接时会发生错误。  
但是，BuyTransaction类不是实现了logTransaction吗，为什么它还是个pure virtual函数呢？  
**根本原因：在derived class 对象的base class构造期间，对象的类型是base class而不是derived class。不只是virtual函数会被编译器解析至base class，若使用运行期类型信息(如dynamic_cast和typeid)，也会把对象视为base class.**
同样道理也适用于析构函数。  

> 有一点需要特别注意，在我们实现类的过程中，我们经常会将一个类中各个构造函数所共同的代表抽离成一个函数(一般会叫init)，而如果在init函数中，我们调用virtual函数。这种情况下容易将错误隐藏得比较深。

### 解决之道
既然我们无法使用在构造或析构过程中调用virtual函数，那我们怎么来实现上面例子中的需求呢？  
一种解决方法：首先在Transaction类内，将logTransaction改为non-virtual函数，然后，让drived class在构造期间传递必要的信息给Transaction的构造函数，最后Transaction构造函数便可以安全的调用logTransaction函数。

