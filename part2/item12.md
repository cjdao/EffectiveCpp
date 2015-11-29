## 条款12：复制对象时勿忘其每一个成分
这个条款说的是，我们在实现拷贝构造函数和赋值操作符时，应该注意的一个点。  
下面看看，容易发生种错误的两种情况：  

* 情况一：新增成员变量时
假设我们定义了一个叫Customer的类，并自己实现了其拷贝构造函数和赋值操作符
```cpp
class Customer{
public:
    //...
    Customer(const Customer& rhs);
    Customer& operator=(const Customer& rhs);
    //...

private:
    string name;
};

Customer::Customer(const Customer& rhs):name(rhs.name)
{
    //...
}
Customer& Customer::operator=(const Customer& rhs)
{
    name = rhs.name;
    return *this;
}
```
上面的实现，一切看起来都OK，但是当我们因需求变更，需要对Customer类增加一个成员变量时，就很容易会忘记在其对应的拷贝构造函数和赋值操作符中进行相应的修改。  
```cpp
class Customer{
public:
    //...
    Customer(const Customer& rhs);
    Customer& operator=(const Customer& rhs);
    //...

private:
    string name;
    string phone;  //新增成员 
};

// Oops! 拷贝构造函数和赋值操作符忘了做相应修改！！！
```

* 情况二：继承 
这种情况是更加隐晦的一种错误!其出错的基本原因是基于[条款5](https://github.com/cjdao/EffectiveCpp/blob/master/part2/item05.md)的两个原则:  
1. 如果你**自己定义拷贝构造函数**，则编译器帮帮你调用父类的**默认构造函数**，坑爹啊!  
2. 如果你**自己定义赋值操作符**，编译器就**拒绝**帮你调用父类的赋值操作符，你需要自己显式的调用 坑爹啊！
 
继续上例子：  
```cpp
class PriorityCustomer:public Customer {
public:
    //...
    PriotiryCustomer(const PriotiryCustomer& rhs);
    PriotiryCustomer& operator=(const PriotiryCustomer& rhs);
    //...
private:
    int priority;
};

PriotiryCustomer::PriotiryCustomer(const PriotiryCustomer& rhs)
                  priority(rhs.priority)
{
    //....
}

PriotiryCustomer& PriotiryCustomer::operator=(const PriotiryCustomer& rhs)
{
    priotiry = rhs.priority; 
    return *this;
}
```
上面的实现貌似一切正常，但是，你已经遗漏了复制其父类的成员变量！  
But, why?  
其出错的基本原因是基于[条款5]()的两个原则:  
1. 如果你**自己定义拷贝构造函数**，则编译器帮帮你调用父类的**默认构造函数**，坑爹啊!  
2. 如果你**自己定义赋值操作符**，编译器就**拒绝**帮你调用父类的赋值操作符，你需要自己显式的调用 坑爹啊！

小伙伴，来看看正确的姿势吧：  
```cpp
PriotiryCustomer::PriotiryCustomer(const PriotiryCustomer& rhs)
                  Customer(rhs),            // 显式调用父类的拷贝构造函数
                  priority(rhs.priority)
{
    //....
}

PriotiryCustomer& PriotiryCustomer::operator=(const PriotiryCustomer& rhs)
{
    Customer::operator=(rhs);      // 显式调用父类的赋值操作符
    priotiry = rhs.priority; 
    return *this;
}
```

### 总结
当你编写copying函数(拷贝构造函数和赋值操作符)时，请确保：  
1. 复制了所有local成员变量。
2. 调用了所有父类的内的适当copying函数。


### 题外话
拷贝构造函数和赋值操作符这两个函数，往往会有近似相同的实现代码，这容易诱使你让某个函数调用另一个以避免代码重复。但这么做却是错误的！！！

1. 赋值操作符调用拷贝构造函数。  
令赋值操作符调用拷贝构造函数是不合理的，因为这就像试图构造一个已经存在的对象。这件事如此荒谬，以至于根本没有相应的语法。

2. 拷贝构造函数调用赋值操作符。
拷贝构造函数是用来初始化新对象的，而赋值操作符只实施于已初始化对象上。所以，对一个尚未构造好的对象进行赋值，是不合理的。

正确的姿势：  
将拷贝构造函数和赋值操作符中，共同的代码，抽离成一个private成员函数。让两者都调用它。

