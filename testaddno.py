import os
import numpy as np
import torch
import onnxruntime

torch.manual_seed(0)
CUSTOM_OP_DOMAIN = "test"
CUSTOM_OP_VERSION = 1
MODEL_FILE = "model.onnx"
LIBRARY_FILE = "libcustomop.so"
DEVICE_NAME = "cpu"
DEVICE_INDEX = 0  # Replace this with the index of the device you want to run on
DEVICE = f"{DEVICE_NAME}:{DEVICE_INDEX}"


def register_custom_op():
    def custom_op_one(g, x):
        return g.op(CUSTOM_OP_DOMAIN + "::MyCustomOp", x)

    from torch.onnx import register_custom_op_symbolic

    register_custom_op_symbolic(
        symbolic_name=f"mynamespace::custom_op_one",
        symbolic_fn=custom_op_one,
        opset_version=CUSTOM_OP_VERSION,
    )


class CustomModel(torch.nn.Module):
    def forward(self, x):
        return torch.ops.mynamespace.custom_op_one(x)


def create_custom_model(model_file: str = MODEL_FILE):
    dtype = torch.float32
    sample_x = torch.ones(2, 2, dtype=dtype)
    inputs = sample_x

    torch.onnx.export(
        CustomModel(),
        inputs,
        model_file,
        opset_version=9,
        input_names=["x"],
        output_names=["z"],
        dynamic_axes={"x": {0: "rows_x", 1: "cols_x"}},
        custom_opsets={CUSTOM_OP_DOMAIN: CUSTOM_OP_VERSION},
    )


# Create an ONNX Runtime session with the provided model and custom ops library
def create_session(model: str) -> onnxruntime.InferenceSession:
    shared_library = LIBRARY_FILE

    so1 = onnxruntime.SessionOptions()
    so1.register_custom_ops_library(shared_library)

    # Model loading successfully indicates that the custom op node could be resolved successfully
    providers = ["GPUExecutionProvider"]
    sess1 = onnxruntime.InferenceSession(model, so1, providers=providers)

    return sess1


def main():
    # register_custom_op()

    # Create the custom model and export it to ONNX
    #    create_custom_model(MODEL_FILE)

    # Create an ONNX Runtime session for inference
    sess = create_session(MODEL_FILE)

    # Prepare input data - ensure it matches the model's input shape and type
    # For this example, we use a 2x2 tensor of ones, as used in create_custom_model
    input_name = sess.get_inputs()[0].name
    input_shape = sess.get_inputs()[0].shape
    dtype = np.float32  # Make sure this matches the expected input type of your model

    # Generate sample input data
    input_data = np.ones(input_shape, dtype=dtype)

    # Run the session (model inference)
    # Note: The input data needs to be in a dictionary format
    output = sess.run(None, {input_name: input_data})

    # Output is a list of outputs. Since we expect one output, we can just take the first one.
    output_data = output[0]

    # Print or process the output data
    print("Inference Output:", output_data)


if __name__ == "__main__":
    main()
