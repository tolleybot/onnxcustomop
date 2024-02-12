// AddOp.cpp

#include "addop.h"
#include <vector>
#include <numeric> // For std::accumulate


void AddOpKernel::Compute(OrtKernelContext* context) {
    // Wrap the context in an Ort::KernelContext object
    Ort::KernelContext kernelContext(context);
    
    // Obtain input tensors
    Ort::ConstValue input0 = kernelContext.GetInput(0);
    Ort::ConstValue input1 = kernelContext.GetInput(1);

    // Assuming GetTensorData and other methods are correctly defined elsewhere or replaced with appropriate API calls
    const float* X = input0.GetTensorData<float>();
    const float* Y = input1.GetTensorData<float>();

    // Get tensor shape and compute the total length
    Ort::TensorTypeAndShapeInfo info0 = input0.GetTensorTypeAndShapeInfo();
    std::vector<int64_t> dims = info0.GetShape();
    size_t total_len = std::accumulate(dims.begin(), dims.end(), 1, std::multiplies<int64_t>());

    // Prepare output tensor
    Ort::UnownedValue output = kernelContext.GetOutput(0, dims.data(), dims.size());
    float* out = output.GetTensorMutableData<float>();

    // Compute output
    for (size_t i = 0; i < total_len; ++i) {
        out[i] = X[i] + Y[i];
    }
}
