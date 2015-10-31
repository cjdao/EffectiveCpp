## 条款08：别让异常逃离析构函数

### 为什么不能让异常逃离析构函数呢?

```cpp
class Widget {
public:
	//...
	~Widget(){//...}; //假设这个可能抛出异常 
};

void doSomething()
{
	std::vector<Widget> v(10);
    //...
}   // v 在这里将自动销毁
```
如果v在销毁的过程中，有两个或以上的Widget元素抛出异常，那么程序将终止或进入行为不明确的境地。

### 如果你的析构函数必须执行一个操作，而该操作在失败时可能会抛出异常，那又该怎么办?
* 如果该异常发生后，程序无法举行正常执行，则在析构函数中捕获该异常，并将程序终止。
* 如果该异常发生后，程序还是能继续可靠的运行，那么就让析构函数将该异常消化掉。
* 如果操作抛出的异常，需要用户介入处理的，那么就需要重新设计类的接口，让用户能够通过成员函数调用该操作并捕获异常。

```cpp
// 数据库连接类
class DBConnection{
public:
	static DBConnection create(); //创建连接
    
    void close();  // 关闭连接， 失败时抛出异常
};

// 为了防止用户在使用完 DBConnection后，忘记close，我们使用一个资源管理类
class DBConn{
public:
	//....
	～DBConn(){
    	db.close();
    }
private:
	DBConnection db;
};

//于是，我们这么使用DBConn类
{
	DBConn dbc(DBConnection::create());//
	//...
} // dbc 自动销毁，close自动调用，此时如果发生异常且从～DBConn()传播出来，将非常麻烦了。
```

我们可以在析构函数中终止程序：
```cpp
DBConn::～DBConn(){
    try{db.close();}
    catch(//...){std::abort();}
 }
```
我们可以在析构函数中吞下异常：
```cpp
DBConn::～DBConn(){
    try{db.close();}
    catch(//...){//记录下对db.close();的失败调用}
 }
```
我们可以重新设计DBConn的接口：
```cpp
class DBConn{
public:
	//...
  	void close() {  // 提供接口给用户调用，从而它们可以捕获异常并加以处理
    	db.close();
    	closed = true;
    }   
    ~DBConn(){
    	if (!closed) { //如果用户没有close，则替用户做close动作
            try{db.close();}
    		catch(//...){//记录下对db.close();的失败调用}
        } 
    }
private:
	DBConnection db;
    bool closed;
};
```

