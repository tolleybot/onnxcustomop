// #include <onnx/onnx_pb.h>
// #include "onnxruntime_config.h"
// #include "core/framework/op_kernel.h"
#include "core/framework/op_kernel.h"
#include "core/common/common.h"
#include "core/session/onnxruntime_cxx_api.h"


namespace onnxruntime {

class AddTwoNumbersOp : public OpKernel {
public:
  explicit AddTwoNumbersOp(const OpKernelInfo& info) : OpKernel(info) {}

  Status Compute(OpKernelContext* context) const override {
    // Get the input tensors
    const Tensor* input_tensor_0 = context->Input<Tensor>(0);
    const Tensor* input_tensor_1 = context->Input<Tensor>(1);

    // Check that the input tensors have the same shape
    ORT_ENFORCE(input_tensor_0->Shape() == input_tensor_1->Shape(),
                "Input tensors must have the same shape");

    // Create the output tensor
    Tensor* output_tensor = context->Output(0, input_tensor_0->Shape());

    // Compute the output values
    for (size_t i = 0; i < input_tensor_0->Shape().Size(); i++) {
      output_tensor->MutableData<float>()[i] = input_tensor_0->Data<float>()[i] + input_tensor_1->Data<float>()[i];
    }

    return Status::OK();
  }
};

// Register the kernel for the custom operator
ONNX_OPERATOR_KERNEL_EX(
    AddTwoNumbers,  // The name of the operator
    kOnnxDomain,  // The domain of the operator (use kMSDomain for custom operators)
    1,  // The version of the operator
    kCpuExecutionProvider,  // The execution provider for the operator
    (*KernelDefBuilder::Create())
        .TypeConstraint("T", DataTypeImpl::GetTensorType<float>()),  // The data type constraint for the operator
    AddTwoNumbersOp);  // The class that implements the operator

}