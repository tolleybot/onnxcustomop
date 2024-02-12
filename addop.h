// AddOp.h
#pragma once

#include "onnxruntime_cxx_api.h" // Make sure this include path is correct

class AddOpKernel {
public:
    AddOpKernel(const OrtApi& api, const OrtKernelInfo* info) {}
    void Compute(OrtKernelContext* context);
};

class AddOp : public Ort::CustomOpBase<AddOp, AddOpKernel> {
public:
    void* CreateKernel(const OrtApi& api, const OrtKernelInfo* info) const {
        return new AddOpKernel(api, info);
    }
    const char* GetName() const { return "AddOp"; }

    // Define the rest based on the CustomOpBase interface
    size_t GetInputTypeCount() const { return 2; }
    ONNXTensorElementDataType GetInputType(size_t index) const {
        return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
    }

    size_t GetOutputTypeCount() const { return 1; }
    ONNXTensorElementDataType GetOutputType(size_t index) const {
        return ONNX_TENSOR_ELEMENT_DATA_TYPE_FLOAT;
    }

    // If your operator supports different execution providers, implement this
    const char* GetExecutionProviderType() const {
        return ""; // Return the appropriate execution provider, or leave empty for default
    }
};
