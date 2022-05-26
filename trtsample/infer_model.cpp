#include "infer_model.h"

HcfEngine::HcfEngine(IExecutionContext& context) {
    const ICudaEngine& hengine = context.getEngine();

    // Pointers to input and output device buffers to pass to engine.
    // Engine requires exactly IEngine::getNbBindings() number of buffers.
    assert(hengine.getNbBindings() == 2);

    // In order to bind the buffers, we need to know the names of the input and output tensors.
    // Note that indices are guaranteed to be less than IEngine::getNbBindings()
    inputIndex = hengine.getBindingIndex(INPUT_BLOB_NAME);
    outputIndex = hengine.getBindingIndex(OUTPUT_BLOB_NAME);
    int max_batch_size = 100;
    // Create GPU buffers on device
    CHECK(cudaMalloc(&buffers[inputIndex], max_batch_size * 3 * INPUT_H * INPUT_W * sizeof(float)));
    CHECK(cudaMalloc(&buffers[outputIndex], max_batch_size * OUTPUT_SIZE * sizeof(float)));

    // Create stream
    CHECK(cudaStreamCreate(&stream));
  
}

void HcfEngine::PredictBatchDenseModel(IExecutionContext& context,
		                       int batchSize,
                                       float* output,
                                       int thread_id) {
    float input[3 * INPUT_H * INPUT_W];
    for (int i = 0; i < 3 * INPUT_H * INPUT_W; i++)
        input[i] = 1;
    CHECK(cudaMemcpyAsync(buffers[inputIndex], input, batchSize * 3 * INPUT_H * INPUT_W * sizeof(float), cudaMemcpyHostToDevice, stream));
    context.enqueue(batchSize, buffers, stream, nullptr);
    CHECK(cudaMemcpyAsync(output, buffers[outputIndex], batchSize * OUTPUT_SIZE * sizeof(float), cudaMemcpyDeviceToHost, stream));
    cudaStreamSynchronize(stream);
}

HcfEngine::~HcfEngine() {
    // Release stream and buffers
    cudaStreamDestroy(stream);
    CHECK(cudaFree(buffers[inputIndex]));
    CHECK(cudaFree(buffers[outputIndex])); 
}
