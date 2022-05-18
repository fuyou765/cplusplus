cmake_minimum_required(VERSION 2.8)
project(PC)
add_executable(PC producer_consumer.cpp)
target_link_libraries(PC pthread)
