#include <iostream>

int main(){
  int a = 0;  // 在这条语句中，a 是左值，0 是临时值，就是右值。a 为非常量左值（有确定存储地址，也有变量名）
  const int a1=10;      //a1 为常量左值（有确定存储地址，也有变量名）
  const int a2=20;      //a2 为常量左值（有确定存储地址，也有变量名）
  //非常量左值引用
  int &lb1=a;            //正确，a是一个非常量左值，可以被非常量左值引用绑定
  //int &lb2=a1;           //错误,a1是一个常量左值，不可以被非常量左值引用绑定
  //int &lb3=10;           //错误，10是一个非常量右值，不可以被非常量左值引用绑定
  //int &lb4=a1+a2;        //错误,（a1+a2）是一个常量右值，不可以被非常量左值引用绑定

  //常量左值引用
  const int &lc1=a;      //正确，a是一个非常量左值，可以被非常量右值引用绑定
  const int &lc2=a1;     //正确,a1是一个常量左值，可以被非常量右值引用绑定
  const int &lc3=a+a1;   //正确，（a+a1）是一个非常量右值，可以被常量右值引用绑定
  const int &lc4=a1+a2;  //正确,（a1+a2）是一个常量右值，可以被非常量右值引用绑定
  //非常量右值引用
  //int &&rb1=a;            //错误，a是一个非常量左值，不可以被非常量右值引用绑定
  //int &&rb2=a1;           //错误,a1是一个常量左值，不可以被非常量右值引用绑定
  int &&rb3=10;           //正确，10是一个非常量右值，可以被非常量右值引用绑定
  //int &&rb4=a1+a2;        //错误,（a1+a2）是一个常量右值，不可以被非常量右值引用绑定

  //常量右值引用
  //const int &&rc1=a;      //错误，a是一个非常量左值，不可以被常量右值引用绑定
  //const int &&rc2=a1;     //错误,a1是一个常量左值，不可以被常量右值引用绑定
  const int &&rc3=a+a1;   //正确，（a+a1）是一个非常量右值，可以被常量右值引用绑定
  const int &&rc4=a1+a2;  //正确,（a1+a2）是一个常量右值，不可以被常量右值引用绑定
  //非常量右值引用
  int &&d1=std::move(a);    //正确，将非常量左值a转换为非常量右值，可以被非常量右值引用绑定
  //int &&d2=std::move(a1);    //错误,将常量左值a1转换为常量右值，不可以被非常量右值引用绑定

  //常量右值引用
  const int &&c1=std::move(a);      //正确，将非常量左值a转换为非常量右值，可以被常量右值引用绑定
  const int &&c2=std::move(a1);     //正确,将常量左值a1转换为常量右值，可以被常量右值引用绑定


}
