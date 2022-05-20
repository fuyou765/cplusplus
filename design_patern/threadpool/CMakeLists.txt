cmake_minimum_required(VERSION 3.6)
project(thread_pool)
add_executable(thread_pool main.cc)
target_link_libraries(thread_pool pthread)
