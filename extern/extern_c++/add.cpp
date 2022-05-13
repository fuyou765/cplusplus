#include <iostream>
using namespace std;
extern "C" {
    #include "add.h"
}
int main() {
    cout<<"the sum is: "<<add(2,3)<<endl;
    return 0;
}
