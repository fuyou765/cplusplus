#include <thread>
#include <iostream>

using namespace std;

void fun1(int x) {
    while (x-- > 0) {
        cout << x << endl;
    }
}

class Base {
public:
    void fun1(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
    static void fun2(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }
    void operator()(int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    }

};
// 注意：如果我们创建多线程 并不会保证哪一个先开始
int main() {
    auto fun2 = [](int x) {
        while (x-- > 0) {
            cout << x << endl;
        }
    };
    Base b;
    thread t1(fun1, 10);
    thread t2(fun2, 10);
    thread t3(Base(), 10);
    thread t4(&Base::fun1,&b, 10);
    thread t5(&Base::fun2, 10);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    return 0;
}
