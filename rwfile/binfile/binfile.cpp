#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
using namespace std;

template <typename T>
void readdata(char const *file_path, T *data, int size) {
    ifstream infile;
    infile.open(file_path, ios::in | ios::binary);
    if (!infile.is_open())
    {
        cout << "Error opening file: " << file_path << std::endl;
        abort();
    }
    for (int i = 0; i < size; i++)
    {
        infile.read((char *)(data+i), sizeof(T));
    }
    infile.close();

}
template <typename T>
void writedata(T *data, char const *file_path,int row, int col){
    ofstream outFile;
    outFile.open(file_path, ios::binary); // 打开模式可省略
    if (!outFile.is_open())
    {
        cout << "Error opening file: " << file_path << std::endl;
        abort();
    }
    outFile.write(reinterpret_cast<const char*>(data),sizeof(T) * (row * col));
    outFile.close();

}
void gen_sparse_field_input(float *data, int fields_num, int size){
    assert(fields_num > 0);
    default_random_engine generator;
    uniform_real_distribution<float> distribution(1, fields_num);
    for(int i = 0; i < size; i++){
        data[i] = distribution(generator);
    }
}

int main(){
    int row = 6;
    int col = 20;
    float *data = new float[row*col];
    int fields_num =10;
    char const *filepath = "output.bin";
    gen_sparse_field_input(data,fields_num,row*col);
    for (int i = 0; i < col; ++i)
    {
        cout<<data[i]<<" ";
    }
    cout<<endl;
    writedata(data,filepath,row,col);
    readdata(filepath, data, row * col);
    for (int i = 0; i < col; ++i)
    {
       cout<<data[i]<<" ";
    }
    cout<<endl;
    delete data;
    return 0;

    
}
