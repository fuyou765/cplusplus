#include <iostream>
#include <memory>
#include <vector>
using namespace std;
void shared_unique_ptr_test()
{
  shared_ptr<int> sp = make_shared<int>(1);//最安全的分配和使用动态内存的方法就是调用一个名为make_shared的标准库函数，
  //此函数在动态内存中分配一个对象并初始化它，返回指向此对象的shared_ptr
  unique_ptr<int> up(new int(1));
  cout<<"sp.get()"<<sp.get()<<endl; // 返回sp中保存的指针 
  cout<<"up.get()"<<up.get()<<endl;
  shared_ptr<int> sq = make_shared<int>(2);
  unique_ptr<int> uq(new int(2));
  cout<<"before swap"<<endl;
  cout<<"sp: "<<*sp<<"up: "<<*up<<endl;
  cout<<"sq: "<<*sq<<"uq: "<<*uq<<endl;
  swap(sq,sp);//交换p和q中的指针
  up.swap(uq);
  cout<<"after swap"<<endl;
  cout<<"sp: "<<*sp<<"up: "<<*up<<endl;
  cout<<"sq: "<<*sq<<"uq: "<<*uq<<endl;

}
void shared_ptr_only()
{
  //shared_ptr<string> sp = new string("linfuyou");//错误，必须使用直接初始化
  shared_ptr<string> sp(new string("lfy"));
  cout<<"before reset: "<<*sp<<endl;
  sp.reset(new string("linfuyou"));
  cout<<"after reset: "<<*sp<<endl;
  shared_ptr<string> sq(sp);//sq 是 shard_ptr sp 的拷贝，此操作会递增sp的计数器。sp中的指针必须能转换为string*
  shared_ptr<string> ss = sp;//sp 和 ss 都是shared_ptr 所保存的指针必须能相互转换。此操作会递减ss的引用计数，递增sp的引用计数.若ss的引用计数变为0，则原内存释放 
  cout<<"sp.unique()"<<sp.unique()<<endl;//若sp.use_count()为1,返回true,否则返回false
  cout<<"sp.use_count()"<<sp.use_count()<<endl;
  

}
unique_ptr<bool> unique_ptr_test()
{
  unique_ptr<string> up(new string("linfuyou_unique_ptr"));
  unique_ptr<string> uq;
  uq.reset(up.release());
  vector<unique_ptr<string>> vec;
  vec.push_back(move(uq));//unique_ptr 可作为容器元素
  if (vec.size() != 0)
    return unique_ptr<bool>(new bool(true));//不能拷贝unique_ptr,但可以在函数返回值中
  else
    return unique_ptr<bool>(new bool(false));
   
}
int main(){
  //shared_ptr 和 unique_ptr 都支持的操作
  shared_unique_ptr_test();
  shared_ptr_only();
  if (unique_ptr_test())
     cout<<"unique_ptr_test pass"<<endl;
  else
     cout<<"unique_ptr_test failure"<<endl;

  return 0;
}
