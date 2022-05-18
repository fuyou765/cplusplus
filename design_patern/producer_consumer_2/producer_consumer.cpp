#include "producer_consumer.h"
Pro_Con::Pro_Con(int qps,
                 int thread_size,
                 int test_secs)
      : qps_(qps),
      thread_size_(thread_size),
      test_secs_(test_secs){
  InitConsumer();
  std::cout<<"Init Pro_Con Finished"<<std::endl;
}



bool Pro_Con::InitConsumer() {
  std::cout<<"Init Consumer"<<std::endl;
  consumer_threads_.resize(thread_size_);
  std::cout<<"Init Consumer Finished"<<std::endl;
  return true;
}

bool Pro_Con::enqueue(int require_id) {
  std::lock_guard<std::mutex> lock(q_mut);
  requires_index.push(require_id);
  return true;
}

int Pro_Con::try_dequeue() {
  std::lock_guard<std::mutex> lock(q_mut);
  int require_id = -1;
  if (!requires_index.empty()) {
    require_id = requires_index.front();
    requires_index.pop();
  }
  return require_id;
}

void Pro_Con::producer_thread() {

  while (1) {
    if (!is_start){
      printf("not start\n");
      continue ;
    }
    for (int t = 0; t < test_secs_; t++) {
      for (int id = 0; id < qps_; id ++) {
        enqueue(t * qps_ + id );
        std::cout<<"producer_thread: "<<t * qps_ + id<<std::endl;
	std::this_thread::sleep_for(std::chrono::microseconds((t * qps_ + id - cur_qps)) * 10);
      }
    }
   break;
  }
}

void Pro_Con::consumer_thread(int thread_id) {
  int require_id;
  while (!is_statistic) {
    if (!is_start)
      continue;
    require_id = try_dequeue();
    if (require_id != -1){
      cur_qps = require_id;	    
      std::cout<<"consumer_thread: "<<require_id<<std::endl;
    }
  }
}

bool Pro_Con::RunnerAsync() {
  printf("Runner Async Start %d consumer threads", thread_size_);
  for (int i = 0; i < thread_size_; i++)
    consumer_threads_[i] = std::thread(&Pro_Con::consumer_thread, this, i);
  producer_thread_ = std::thread(&Pro_Con::producer_thread, this);
  std::cout<<"Runner Async Success"<<std::endl;
  return true;
}


bool Pro_Con::RunnerSynchronize() {
  std::cout<<"Runner Sync Start"<<std::endl;

  is_start = true;
  producer_thread_.join();

  is_statistic = true;
  for (int i = 0; i < thread_size_; i++)
    consumer_threads_[i].join();

  std::cout<<"Runner Sync End"<<std::endl;

  return true;
}

Pro_Con::~Pro_Con(){
  std::cout<<"test over"<<std::endl;
}
