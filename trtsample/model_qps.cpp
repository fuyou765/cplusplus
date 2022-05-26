#include <numeric>
#include "model_qps.h"

int64_t cur_qps = 0;
bool is_start =true;
std::chrono::high_resolution_clock::time_point cur_ts;
std::chrono::high_resolution_clock::time_point ts;


namespace hcfengine {



ModelParQPSTest::ModelParQPSTest(int qps,
                                     int thread_size,
                                     std::string hcf_model_file,
                                     int test_secs,
                                     int batch_size,
                                     int max_engine_pool_size)
    : qps_(qps),
      thread_size_(thread_size),
      hcf_model_file_(hcf_model_file),
      test_secs_(test_secs),
      batch_size_(batch_size) {
  char *trtModelStream{nullptr};
  size_t size{0};
  std::ifstream file("alexnet.engine", std::ios::binary);
  if (file.good()) {
     file.seekg(0, file.end);
     size = file.tellg();
     file.seekg(0, file.beg);
     trtModelStream = new char[size];
     assert(trtModelStream);
     file.read(trtModelStream, size);
     file.close();
  }
  printf("CreateIEngine !\n");
  runtime = createInferRuntime(gLogger);
  assert(runtime != nullptr);
  engine = runtime->deserializeCudaEngine(trtModelStream, size, nullptr);
  assert(engine != nullptr);
  context = engine->createExecutionContext();
  assert(context != nullptr);
  for (int i = 0; i < thread_size_; i++)  {
    HcfEngine *hcf_engine = new HcfEngine(*context);
    hcf_engine_.push_back(hcf_engine);
  }
  InitProducer();
  InitConsumer();
  printf("Init ModelParQPSTest Finished\n");
}
bool ModelParQPSTest::InitProducer() {
  printf("Init Producer\n");
  for (int i = 0; i < qps_ * test_secs_; i++) {
    Require require;
    
    require.batch_size = batch_size_;
    require.copy_size = batch_size_ * 1480;

    all_requires_.push_back(std::move(require));
  }

  int32_t warmup_time = 10;
  printf("Start warmup for %d time.\n", warmup_time);
  for (int32_t id = 0; id < thread_size_; id++) {
    for (int32_t i = 0; i < warmup_time; i++) {
      hcf_engine_[id]->PredictBatchDenseModel(*context, batch_size_, prob, id);
    }
  }
  printf("Warmup done.\n");
  printf("Init Producer Finished\n");
  return true;
}

bool ModelParQPSTest::InitConsumer() {
  printf("Init Consumer\n");
  consumer_threads_.resize(thread_size_);
  printf("Init Consumer Finished\n");
  return true;
}

bool ModelParQPSTest::enqueue(int require_id) {
  std::lock_guard<std::mutex> lock(q_mut);
  all_requires_[require_id].gen_time_ = std::chrono::high_resolution_clock::now();
  all_requires_[require_id].run_tracer_.require_gen = all_requires_[require_id].gen_time_;
  requires_index.push(require_id);
  return true;
}

int ModelParQPSTest::try_dequeue() {
  std::lock_guard<std::mutex> lock(q_mut);
  int require_id = -1;
  if (!requires_index.empty()) {
    require_id = requires_index.front();
    requires_index.pop();
  }
  return require_id;
}

void ModelParQPSTest::producer_thread() {
  
  while (1) {
    if (!is_start){
      printf("not start\n");
      continue ;
    }
      
    auto produce_st = std::chrono::high_resolution_clock::now();        
    int wait_per_require_time = 1000 * 1000 / qps_;
    cur_qps = 0;
    int cur_qps1 = 0;
    int64_t sleep_time = 0;
    int64_t time_process = 0;
    int64_t sleep_us = 0;
    float cur_time = 0;
    printf("sleep time %d us\n", wait_per_require_time);
    for (int t = 0; t < test_secs_; t++) {
      for (int id = 0; id < qps_; id ++) {
        enqueue(t * qps_ + id );
	cur_qps1 = cur_qps;
        cur_ts = std::chrono::high_resolution_clock::now();        
        cur_time = std::chrono::duration<float, std::micro>(cur_ts - ts).count();
        if ((cur_qps1 > 0) && cur_qps1 < qps_ && cur_qps1 >id -2 && cur_qps1 < id + 2){
          time_process = (cur_time - sleep_time) / cur_qps1 * (qps_ - cur_qps1);
          sleep_us = (1e6 - cur_time - time_process) / (qps_ - cur_qps1);
          if(sleep_us > 0){
            //printf("id %d cur_qps %d sleep us %ld\n",id, cur_qps1, sleep_us);
            sleep_time += sleep_us;
            std::this_thread::sleep_for(std::chrono::microseconds(sleep_us));
          }
        }
        else {
          wait_per_require_time = (1e6 - cur_time) / (qps_ - id);
          //printf(" sleep time  %d\n",wait_per_require_time);
	  if (wait_per_require_time > 0){
          sleep_time += wait_per_require_time;
          std::this_thread::sleep_for(std::chrono::microseconds(wait_per_require_time));
	  }
        }
        
      }
    }
    auto produce_end = std::chrono::high_resolution_clock::now();        
    float cost_time = std::chrono::duration<float, std::micro>(produce_end - produce_st).count();
    printf(" produce cost time %f ms\n", cost_time/1e3);
   
    break;
  }
  
  
}

void ModelParQPSTest::consumer_thread(int thread_id) {
  

  int require_id;
  std::vector<float> output_vector;
  while (!is_statistic) {
    if (!is_start)
      continue;
    require_id = try_dequeue();
    if (require_id != -1) {
      cur_qps = require_id + 1;
      all_requires_[require_id].run_st_time_ = std::chrono::high_resolution_clock::now();
      hcf_engine_[thread_id]->PredictBatchDenseModel(*context, all_requires_[require_id].batch_size, prob,
                                         thread_id);
      all_requires_[require_id].run_en_time_ = std::chrono::high_resolution_clock::now();
      all_requires_[require_id].is_finished_ = true;
      all_requires_[require_id].run_tracer_.session_run_time_in_us =
          std::chrono::duration<float, std::micro>(all_requires_[require_id].run_en_time_ -
                                                   all_requires_[require_id].gen_time_).count();
    }
  }
}

bool ModelParQPSTest::RunnerAsync() {
  printf("Runner Async Start %d consumer threads\n", thread_size_);
  for (int i = 0; i < thread_size_; i++)
    consumer_threads_[i] = std::thread(&ModelParQPSTest::consumer_thread, this, i);
  producer_thread_ = std::thread(&ModelParQPSTest::producer_thread, this);
  printf("Runner Async Success\n");
  return true;
}

bool ModelParQPSTest::all_requires_finished() {
  for (int i = 0; i < qps_ * test_secs_; i++)
    if (!all_requires_[i].is_finished_)
      return false;
  return true;
}

bool ModelParQPSTest::RunnerSynchronize() {
  printf("Runner Sync Start\n");
  
  ts = std::chrono::high_resolution_clock::now();
  is_start = true;
  producer_thread_.join();
    
  while (!all_requires_finished()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  is_statistic = true;
  for (int i = 0; i < thread_size_; i++)
    consumer_threads_[i].join();

  auto te = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::milli>(te - ts).count();
  printf("time_use: %.4f s, real_qps: %d", time / 1e3, int(test_secs_ * qps_ / (time / 1e3)));

  printf("Runner Sync End\n");

  std::vector<float> time_use;
  for (int i = 0; i < all_requires_.size(); i++) {
    time_use.push_back(all_requires_[i].run_tracer_.session_run_time_in_us);
  }
  printf(
      "session_run_time max: %.4f ms, min: %.4f ms, mean: %.4f ms, p99: %.4f ms requires_num: %d",
      StatisticMax(time_use), StatisticMin(time_use), StatisticMean(time_use),
      StatisticTP99(time_use), int(all_requires_.size()));
  return true;
}

float ModelParQPSTest::StatisticMax(std::vector<float> &v) {
  return *max_element(v.begin(), v.end()) / 1e3;
}

float ModelParQPSTest::StatisticMin(std::vector<float> &v) {
  return *min_element(v.begin(), v.end()) / 1e3;
}

float ModelParQPSTest::StatisticMean(std::vector<float> &v) {
  float sum = std::accumulate(v.begin(), v.end(), 0.0);
  return sum / v.size() / 1e3;
}

bool LessSort(float a, float b) {
  return a < b;
}

float ModelParQPSTest::StatisticTP99(std::vector<float> &v) {
  sort(v.begin(), v.end(), LessSort);
  int index = floor(v.size() * 0.99);
  return v[index] / 1e3;
}

ModelParQPSTest::~ModelParQPSTest() {
  for (auto infer : hcf_engine_)
    delete infer;
  context->destroy();
  engine->destroy();
  runtime->destroy();
}

}  // namespace hcfengine
