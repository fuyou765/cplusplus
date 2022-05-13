#include <iostream>
#include <cassert>
#include <vector>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>
#include <random>
using namespace std;

void readdata(char const *dirpath, float *data) {
    string lineStr;
    vector<float> lineArray;
    ifstream inFile(dirpath, ios::in);
    while (getline(inFile, lineStr))
    {
      stringstream ss(lineStr);
      string str;
      // 按照逗号分隔
      while (getline(ss, str, ','))
        lineArray.push_back(atof(str.c_str()));
    }
    cout <<lineArray.size()<<endl;
    for (int i = 0; i < lineArray.size(); i++) {
              data[i] = lineArray.at(i);
          }
}
void writedata(float *data, char const *dirpath,int row, int col){
    ofstream outFile;
    outFile.open(dirpath, ios::app); // 打开模式可省略
    for (int hh =0; hh <row; ++hh){
        for (int ww =0;ww<col; ++ww){
            outFile << data[hh*row+ww] ;
            if (ww < 19){
            outFile << ',';
            }
         }
        outFile <<endl;
    }
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
    char const *dirpath = "output.txt";
    gen_sparse_field_input(data,fields_num,row*col);
    for (int i = 0; i < col; ++i)
    {
        cout<<data[i]<<" ";
    }
    cout<<endl;
    writedata(data,dirpath,row,col);
    readdata(dirpath, data);
    for (int i = 0; i < col; ++i)
    {
       cout<<data[i]<<" ";
    }
    cout<<endl;
    return 0;

    
}
