#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define PATH "../file/"
int scanDirectorFromPath(const char* path)
{
   DIR *dir;
   dir = opendir(path);
   if(dir == NULL){
   cout<<"Open Dir failed!"<<endl;
   return -1;
   }
   struct dirent *dirent_;
   while(dirent_ = readdir(dir))
   {
     string dir_name = dirent_ ->d_name;
     //ignore . or .. file
     if(dir_name  == "." || dir_name  == ".."){
       continue;
     }
     else{
       cout<< " File‘s Name:"<<dir_name <<endl;
     }
   }
   return 0;
}
int main()
{
   scanDirectorFromPath(PATH);
   return 0;
}

