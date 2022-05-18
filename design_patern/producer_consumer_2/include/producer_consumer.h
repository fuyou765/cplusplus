#include <queue>
#include <mutex>
#include <chrono>
#include <thread>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>

class Pro_Con {
  public:
    Pro_Con(int qps, int thread_size, int test_secs);
    ~Pro_Con();
    bool InitConsumer();
    bool RunnerAsync();
    bool RunnerSynchronize();
  private:
    void producer_thread();
    void consumer_thread(int thread_id);
    bool enqueue(int require_id);
    bool is_statistic = false;
    bool is_start = false;
    int try_dequeue();
    std::queue<int> requires_index;
    std::vector<std::thread> consumer_threads_;
    std::thread producer_thread_;
    int thread_size_;
    int qps_;
    int test_secs_;
    int cur_qps;
    std::mutex q_mut;
    std::thread _update;
	  
};
