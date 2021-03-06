## 条款06：若不想使用编译器自动生成的函数，就该明确拒绝
条款05告诉我们，编译器有可能会自动帮我们的生成的函数：默认构造函数、析构函数、拷贝构造函数和赋值操作符。

### 拒绝的理由
情况一：
* 我们不需要编译器为我们生成的函数，我们想自己定义自己版本的的函数

情况二：
* 我们不希望我们定义的类存在上述函数(即我们不希望编译器帮我们生成，又不希望自己去定义它们。)

### 拒绝的实现原理
情况一：
* 我们只要大胆的实现自己的版本，编译器就不再帮我们生成它们。

情况二：
* 如果我们明确声明了上述中的成员函数，编译器就不再生成它们；
* 如果我们明确声明了上述中的成员函数，而又不去定义它们，则任何想调用它们的人都会得到一个**链接时错误**
* 进一步，如果你将它们都声明为private的，则可以在**编译时(而不是在链接时)**成功阻止其他类调用它们，当然同类内的调用错误和friend的调用错误还是**链接时错误**。

**情况二一般只存在于拷贝构造和赋值操作符中**

### 如何让所有对于拷贝构造和赋值操作符中的调用错误都发生在编译时，而不管是否是同一个类内的或者friend。
```cpp
class Uncopyable{
protected:
	Uncopyable(){}
    ~Uncopyable(){}
private:
	Uncopyable(const Uncopyable &);
    Uncopyable &operator=(const Uncopyable &);
};

class Test:public Uncopyable{
// 这里你就不再需要对Test类的拷贝构造函数和赋值操作符做任何
// 定义或声明了。
};//
Test t1;
Test t2(t1); // 编译时错误
Test t3;
t3 = t1; // 编译时错误
```
> Uncopyable 的实现和运用颇为微妙，包括不一定得以public继承它，以及其析构函数不一定得是virtual等等。boost库提供了类似的版本，类名叫noncopyable.
