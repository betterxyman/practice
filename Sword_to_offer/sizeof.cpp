#include<iostream>
using namespace std;

// 任何类型的实例都必须在内存中占有一定的空间，否则无法使用
// 此处空实例占用1个字节
class empty0{

};
// 构造函数和析构函数只与类型相关，与类型的实例无关。
class empty1{
    empty1(){}
    ~empty1(){}
};
// C++编译器发现类型中有虚函数，就会为该类型生成虚函数表，并在该类型的每一个实例中添加一个指向虚函数表的指针。
class empty2{
    empty2(){}
    virtual ~empty2(){}
};
int main()
{
    cout<<"empty0 class size is "<<sizeof(empty0)<<endl;
    cout<<"empty1 class size is "<<sizeof(empty1)<<endl;
    cout<<"empty2 class size is "<<sizeof(empty2)<<endl;
    return 0;
}