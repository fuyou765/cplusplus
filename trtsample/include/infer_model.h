#ifndef INFER_MODEL_H_
#define INFER_MODEL_H_

#include "NvInfer.h"
#include "cuda_runtime_api.h"
#include "logging.h"
#include <fstream>
#include <map>
#include <chrono>
#include <vector>
#define CHECK(status) \
    do\
    {\
        auto ret = (status);\
        if (ret != 0)\
        {\
            std::cerr << "Cuda failure: " << ret << std::endl;\
            abort();\
        }\
    } while (0)

// stuff we know about the network and the input/output blobs
static const int INPUT_H = 224;
static const int INPUT_W = 224;
static const int OUTPUT_SIZE = 1000;


using namespace nvinfer1;

static Logger gLogger;

class HcfEngine {
 public:
  HcfEngine(IExecutionContext& context);
  struct Tracer {
    float session_run_time_in_us = 0;
    std::chrono::high_resolution_clock::time_point require_gen;
  };

  void PredictBatchDenseModel(IExecutionContext& context, 
		              int batch_size,
                              float* output,
                              int thread_id);
  ~HcfEngine();

  cudaStream_t stream;
  void* buffers[2];
  int inputIndex;
  int outputIndex;
  const char* INPUT_BLOB_NAME = "data";
  const char* OUTPUT_BLOB_NAME = "prob";


};
#endif  // INFER_MODEL_H

