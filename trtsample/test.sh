#!/bin/bash
export MM_CPP_MIN_LOG_LEVEL=2
# set -e
# set -x

echo "===> build modelv2 mm_model..."
#../build/sample_modelv2 model True 900

echo "===> benckmarking qps test..."
#for qps in 9900 10000 10100
#for qps in 2300 2400
for qps in $(seq 800 100 1500)
do
    for threads in 8
    do
        echo "===> start run $qps qps $threads threads..."
        #../build/qps_test model 2096550_sample_id_num_batch_size.txt $qps $threads
        build/model_qps alexnet.engine 1 $qps $threads 1
        echo ""
    done
done
