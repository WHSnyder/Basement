#Tests for basic TF models w/C++ & Python APIs

cmake_minimum_required(VERSION 3.2)
project(tf_tests)

if (BUILDTEST)

	add_library(test_TFCPP STATIC)
	#add_executable(test_TFCPP)

	target_sources(test_TFCPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/StyleTransfer.cpp ${CMAKE_CURRENT_SOURCE_DIR}/cpp_tf_test.cpp)

	target_include_directories(test_TFCPP PUBLIC ${OpenCV_INCLUDE_DIRS})

	target_link_directories(test_TFCPP PUBLIC ${OpenCV_LIBRARY_DIRS})
	target_link_libraries(test_TFCPP PUBLIC ${OpenCV_LIBRARIES})
	target_link_libraries(test_TFCPP PUBLIC -ldl)
	target_link_libraries(test_TFCPP PUBLIC -lpthread)

	target_include_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/tensorflow/lite/tools/make/downloads/flatbuffers/include)
	target_include_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl)
	target_include_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/tensorflow/lite/tools/make/downloads/absl)
	target_include_directories(test_TFCPP PUBLIC ${CD}/src/tf_driver)


	target_link_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/bazel-bin/tensorflow/lite)
	target_link_libraries(test_TFCPP PUBLIC tensorflowlite)

	target_link_directories(test_TFCPP PUBLIC ${CD}/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu)
	target_link_libraries(test_TFCPP PUBLIC tensorflowlite_gpu_delegate) 



	if (UNIX AND NOT APPLE)
		target_link_libraries(test_TFCPP PUBLIC "-Wl,--no-as-needed")
	endif()

endif()