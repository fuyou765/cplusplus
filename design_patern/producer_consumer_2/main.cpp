#include "producer_consumer.h"

int main(int argc, char *argv[]){
   if (argc != 3){
     printf("Usage:%s qps test_sec thread_num \n", argv[0]);
   }
   int qps = atoi(argv[1]);
   int thread_num = atoi(argv[3]);
   int test_sec = atoi(argv[2]);
   Pro_Con *test =
      new Pro_Con(qps, thread_num, test_sec);
    test->RunnerAsync();
    test->RunnerSynchronize();
    delete test;

}
