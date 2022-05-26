#include <numeric>
#include <vector>
#include "model_qps.h"
using namespace std;
using namespace nvinfer1;
using namespace hcfengine;

int main(int argc, char *argv[]) {
  if (argc !=6) {
    printf("Usage:%s model_name time qps thread_num batchsize  \n", argv[0]);
    return -1;
  }
  const char *model_name = argv[1];
  int time = atoi(argv[2]); 
  int qps = atoi(argv[3]);
  int thread_num = atoi(argv[4]);
  int batchsize = atoi(argv[5]);
  ModelParQPSTest *test =
    new ModelParQPSTest(qps, thread_num, model_name, time, batchsize, 1);
  test->RunnerAsync();
  test->RunnerSynchronize();
  delete test;
  return 0;
}
