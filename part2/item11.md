## 条款11：在operator=中处理"自我赋值"
本条款说的是，在重载operator=时，要记得处理'自我赋值'的情况。  
也许我们会很自信的说，我才不会写出'自我赋值'这么SB的代码呢，像下面这样:  
```cpp
class Widget{//...};
Widget w;
//...
w = w; // 自我赋值
```
但是别忽视了，一些'潜在的自我赋值',如下面:  
```cpp
// 数组
a[i] = a[j]; // 如果i和j有相同的值，这便是自我赋值了
// 指针
*px = *py;   // 如果px和py指向同一个，这也是自我赋值
// 继承
class Base {//...};
class Derived:public Base {//...};
void doSomething(const Base&rb, Derived *pd); // rb 和 *pd可能其实是同一个对象
```
好吧, 那就算是出现了'自我赋值'，那又会有什么问题呢?  
我们知道默认情况下，C++编译器是会为我们自动生成operator=的实现的([条款05](https://github.com/cjdao/EffectiveCpp/blob/master/part2/item05.md))，而一旦我们需要重载operator=，则说明默认生成的版本不能满足我们的需求的了，如我们需要进行更深层次的资源拷贝。看下面的例子:  
```cpp
class Bitmap{//...};
class Widget{
//...
private:
    Bitmap *pb;
};

Widget& Widget::operator=(const Widget &rhs)
{
    delete pb;
    pb = new Bitmap(*rhs.pb); // 我们需要拷贝指针所指向的内容，而不是简单的拷贝指针
    return *this;
}
```
如果是自我赋值的情况，那么,rhs.pb和pb将是相同的值，因此此处rhs.pb将变成一个悬空的指针。

### 解决方案
* 方案一: 证同测试 
这是最最基本的处理方式了, 如下：  
```cpp
Widget& Widget::operator=(const Widget &rhs)
{
    if (&rhs == this) return *this;   // 证同测试, 若是自我赋值，则不做任何事
    
    delete pb;
    pb = new Bitmap(*rhs.pb);
    return *this;
}
```

* 方案二: 异常安全  
**让operator=具备"异常安全性"(参考[条款29]())往往能够自动获得"自我赋值安全"的回报，因此愈来愈多人对"自我赋值"的处理态度是倾向不去管它，把焦点放在实现"异常安全"上。因此，上述例子可以这么实现：**    
```cpp
Widget& Widget::operator=(const Widget &rhs)
{
    Bitmap* pOrig = pb;         // 记住原先的pb
    pb = new Bitmap(*rhs.pb);   // 令pb指向*rhs.pb的一个副本
    delete pOrig;               // 删除原先的pb
    return *this;
}
```
> 上述'异常安全'的operator=代码，能够正确的处理'自我赋值'的情况，只是效率上有点差。如果你很关心效率，可以把'证同测试'再次放回函数起始处。然而这样做之前先问问自己，你估计'自我赋值'的发生频率有多高？因为这项测试也需要成本。 

* 方案三：copy and swap技术  
它还是一个'异常安全性'的解决方案，只是更加地道了, 参见[条款29]()
```cpp
class Widget{
//...
    void swap(Widget& r);   // 交换*this和r的数据，详见条款29
//...
};

Widget& Widget::operator=(const Widget &rhs)
{
    Widget temp(rhs);  // 为rhs数据制作一份副本
    swap(temp);        // 与*this交换数据
    return *this;
}
```
