#!/bin/bash
# set -e
# set -x

#../build/sample_modelv2 model True 900

echo "===> benckmarking qps test..."
build/PC  10 10 4 

#for qps in $(seq 10 10 100)
#do
#    for threads in 8
#    do
#        echo "===> start run $qps qps $threads threads..."
#        #../build/qps_test model 2096550_sample_id_num_batch_size.txt $qps $threads
#        build/PC  $qps 1 $threads 
#    done
#done
