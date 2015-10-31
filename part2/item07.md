## 条款07：为多态基类声明virtual析构函数
* 带多态性质的基类应该声明一个virtual析构函数
* 如果一个类有virtual函数，那么它需要一个virtual析构函数

反过来讲：
* 如果一个类不是作为基类而设计的，那它的析构函数就不应该是virtual的
* 如果一个基类，不具有多态特性，那它的析构函数就不应该是virtual的

### 技巧：使用pure virtual析构函数构造抽象类
如果你想将一个类定义为抽象类，但是有没有任何成员函数是pure virtual的，那么你就可以将其析构函数声明为pure virtual的来满足需求，但是你除了声明**还必须给该pure virtual析构函数一个实现**,因为其子类在析构式，编译器会让它们调用该pure virtual析构函数。
```cpp
//抽象类定义
class AWOV{
public:
	// pure virtual析构函数
	virtual ~AWOV() = 0;
};
// 必须给pure virtual析构函数一份定义
AWOV::~AWOV() {}

// 子类，析构时会调用~AWOV()
class Derived:public AWOV{};
```