// #include <onnxruntime/core/providers/cpu/cpu_provider_factory.h>
// #include <onnxruntime/core/session/onnxruntime_cxx_api.h>
// #include <opencv2/opencv.hpp>
// #include <iostream>

// void hand_sign_detection(Ort::Session& session, Ort::Env& env) {
//     cv::VideoCapture cap(0);
//     if (!cap.isOpened()) {
//         std::cerr << "无法打开摄像头" << std::endl;
//         return;
//     }

//     while (true) {
//         cv::Mat frame;
//         cap >> frame;
//         if (frame.empty()) {
//             std::cerr << "无法读取摄像头画面" << std::endl;
//             break;
//         }

//         // 在这里执行模型推理
//         // 将OpenCV图像转换为适合ONNX的输入格式
//         cv::resize(frame, frame, cv::Size(640, 640));  // 假设输入是640x640
//         frame.convertTo(frame, CV_32F);
//         frame = frame / 255.0;

//         std::vector<int64_t> input_shape = {1, 3, 640, 640};  // 批次1，3通道RGB，尺寸640x640
//         std::vector<float> input_tensor_values(frame.begin<float>(), frame.end<float>());

//         // 创建输入tensor
//         Ort::Value input_tensor = Ort::Value::CreateTensor<float>(session.GetAllocatorWithDefaultOptions(),
//                                                                   input_tensor_values.data(), input_tensor_values.size(),
//                                                                   input_shape.data(), input_shape.size());

//         // 推理
//         const char* input_names[] = {"input"};
//         const char* output_names[] = {"output"};
//         auto output_tensors = session.Run(Ort::RunOptions{nullptr}, input_names, &input_tensor, 1, output_names, 1);

//         // 解析结果 (假设返回的输出是一个bounding box + 置信度)
//         float* output_data = output_tensors[0].GetTensorMutableData<float>();

//         // 对结果进行处理和过滤（例如根据置信度阈值）
//         // 你需要根据模型的输出格式解析结果，例如识别字母和置信度

//         // 显示结果
//         cv::imshow("Hand Sign Detection", frame);

//         if (cv::waitKey(1) == 'q') {
//             break;
//         }
//     }

//     cap.release();
//     cv::destroyAllWindows();
// }

// int main() {
//     // 初始化ONNX Runtime
//     Ort::Env env(ORT_LOGGING_LEVEL_WARNING, "hand_sign_detection");
//     Ort::SessionOptions session_options;
//     session_options.SetIntraOpNumThreads(1);
//     session_options.SetGraphOptimizationLevel(GraphOptimizationLevel::ORT_ENABLE_EXTENDED);

//     // 加载ONNX模型
//     const std::string model_path = "path_to_your_model.onnx";
//     Ort::Session session(env, model_path.c_str(), session_options);

//     // 调用手势识别函数
//     hand_sign_detection(session, env);

//     return 0;
// }
