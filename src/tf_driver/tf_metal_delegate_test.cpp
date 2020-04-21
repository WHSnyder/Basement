






int main(int argc, char **argv){

	auto model = FlatBufferModel::BuildFromFile(model_path);
	
	if (!model) return false;
	tflite::ops::builtin::BuiltinOpResolver op_resolver;
	std::unique_ptr<Interpreter> interpreter;
	InterpreterBuilder(*model, op_resolver)(&interpreter);

	// NEW: Prepare GPU delegate.

	const GpuDelegateOptions options = {
	  .allow_precision_loss = false,
	  .wait_type = kGpuDelegateOptions::WaitType::Passive,
	};

	auto* delegate = NewGpuDelegate(options);
	if (interpreter->ModifyGraphWithDelegate(delegate) != kTfLiteOk) return false;

	// Run inference.
	WriteToInputTensor(interpreter->typed_input_tensor<float>(0));
	if (interpreter->Invoke() != kTfLiteOk) return false;
	ReadFromOutputTensor(interpreter->typed_output_tensor<float>(0));

	// Clean up.
	DeleteGpuDelegate(delegate);





}

