
#ifndef MODEL_QPS_H_
#define MODEL_QPS_H_

#include <queue>
#include <mutex>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>
#include <fstream>
#include "infer_model.h"
namespace hcfengine {
class ModelParQPSTest {
 public:
  ModelParQPSTest(int qps,
                    int thread_size,
                    std::string hcf_model_file,
                    int test_secs,
                    int batch_size,
                    int max_engine_pool_size);
  ~ModelParQPSTest();
  bool InitProducer();
  bool InitConsumer();
  bool RunnerAsync();
  bool RunnerSynchronize();

 protected:
  struct Require {
    std::chrono::high_resolution_clock::time_point gen_time_;
    std::chrono::high_resolution_clock::time_point run_st_time_;
    std::chrono::high_resolution_clock::time_point run_en_time_;
    HcfEngine::Tracer run_tracer_;
    bool is_finished_ = false;
    size_t batch_size = 0;
    size_t copy_size  = 0;
  };

 private:
  void producer_thread();
  void consumer_thread(int thread_id);
  bool enqueue(int require_id);
  int try_dequeue();
  bool all_requires_finished();
  float StatisticMax(std::vector<float> &v);
  float StatisticMin(std::vector<float> &v);
  float StatisticMean(std::vector<float> &v);
  float StatisticTP99(std::vector<float> &v);

  std::queue<int> requires_index;
  std::vector<HcfEngine *> hcf_engine_;
  IRuntime *runtime = {nullptr};
  ICudaEngine *engine = {nullptr};
  IExecutionContext *context = {nullptr};
  std::vector<std::thread> consumer_threads_;
  std::thread producer_thread_;
  std::vector<Require> all_requires_;
  int qps_;
  int thread_size_;
  std::string hcf_model_file_;
  int test_secs_;
  int batch_size_;
  std::mutex q_mut;
  bool is_statistic      = false;
  int _sleep_update_time = 1000 * 1000 * 10;
  std::thread _update;
  size_t READER_THREAD_SIZE = 80;
  float prob[OUTPUT_SIZE];
};

}  // namespace hcfengine

#endif  // GDT_SMART_PCTR_CONTRIB_HCFENGINE_HCF_ENGINE_DENSE_MODEL_QPS_H
