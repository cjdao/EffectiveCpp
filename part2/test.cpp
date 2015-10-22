#include <iostream>
using namespace std;

class super{
public :
    super(){cout << "super defualt cotor called."<< endl;}
    super(const super&) {cout << "super copy cotor called." << endl;}
};
class sub1:super{

};

class sub2:super{
public:
    sub2(){cout << "sub2 default cotor called."<< endl;}
    sub2(const sub2&) {cout << "sub2 copy cotor called."<< endl;}
};
int main()
{
    cout << "sub1" << endl;
    sub1 s11;
    sub1 s12(s11) ;
    cout << "sub2" << endl;
    sub2 s21;
    sub2 s22(s21) ;  

    return 0;
}
