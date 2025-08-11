#include <vector>
#include <map>

#include <memory>
#include <chrono>
#include <iostream>
#include <Eigen/Dense>
#include <type_traits>
#include <librealsense2/rs.hpp>

#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>

#include "abstract_listeners/GenericListener.h"

#include "listener_utils/general_utils.hpp"
#include "listener_utils/CamParameters.hpp"
#include "listener_frames/RGBFrame.h"
#include "listener_frames/CompositeFrame.h"
#include "listener_frames/GrayFrame.h"
#include "listener_frames/GridFrame.h"

#include "sensor_interfaces/RealsenseInterface.h"
#include "RealsenseListener.h"

rs2::device_list RealsenseListener::deviceList = rs2::context().query_devices();

int RealsenseListener::camNumber = 0;

RealsenseListener::RealsenseListener(std::shared_ptr<RealsenseInterface> p_sensor_interface, const std::string& name, const float resize_factor, const std::string& param_dir)
	: GenericListener(name, p_sensor_interface->getFramerate()), SingleListener(p_sensor_interface, name, resize_factor, param_dir)
{
	// Register device with config object
	if (deviceList.size() <= camNumber)
	{
		throw std::runtime_error("Required RealSense device not found.");
	}
	deviceList[0].hardware_reset();

	m_device = deviceList[camNumber];
	++camNumber;
	m_config.enable_device(m_device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER));

	// Set properties and physical sensor of pipeline stream
	m_config.enable_stream(RS2_STREAM_DEPTH, p_sensor_interface->getWidth(), p_sensor_interface->getHeight(), RS2_FORMAT_Z16, p_sensor_interface->getFramerate());
	m_config.enable_stream(RS2_STREAM_COLOR, p_sensor_interface->getWidth(), p_sensor_interface->getHeight(), RS2_FORMAT_BGR8, p_sensor_interface->getFramerate());
	m_config.enable_stream(RS2_STREAM_INFRARED, camNumber, p_sensor_interface->getWidth(), p_sensor_interface->getHeight(), RS2_FORMAT_Y8, p_sensor_interface->getFramerate());

	rs2::pipeline_profile pipeline_profile = m_config.resolve(m_pipeline);
	auto streams = pipeline_profile.get_streams();
	//Start the Pipeline to get the intrinsics
	
	m_pipeline.start(RealsenseListener::m_config);

	rs2_intrinsics intrinsics;
	rs2::video_stream_profile profile = rs2::video_stream_profile(pipeline_profile.get_stream(RS2_STREAM_DEPTH));
	intrinsics = profile.get_intrinsics();

	
	auto p_intrinsics = std::make_shared<Eigen::Matrix3f>();
	*p_intrinsics << intrinsics.fx, 0.0f, intrinsics.ppx,
					 0.0f, intrinsics.fy, intrinsics.ppy,
					 0.0f, 0.0f, 1.0f;
	std::vector<float> intrinsics_vector ({ intrinsics.coeffs[0], intrinsics.coeffs[1], intrinsics.coeffs[2], intrinsics.coeffs[3], intrinsics.coeffs[4] });
	
	auto p_distortion = std::make_shared<std::vector<float>>(intrinsics_vector);
	m_pipeline.stop();
	setCamParams(std::make_shared<CamParameters>(p_intrinsics, p_distortion));
	std::cout << *p_intrinsics << std::endl;
};

void RealsenseListener::startStream()
{	
	Sleep(5000);
	if (m_isStreaming)
	{
		throw std::runtime_error("Sensor '" + m_name + "' cannot be started while already streaming.");
	}
	m_isStreaming = true;
	auto callback_func = std::bind(&RealsenseListener::onNewData, this, std::placeholders::_1);
	m_pipeline.start(RealsenseListener::m_config, callback_func);
}

void RealsenseListener::stopStream()
{
	if (!m_isStreaming)
	{
		throw std::runtime_error("Sensor '" + m_name + "' cannot be stopped while not streaming.");
	}
	else
	{
		m_isStreaming = false;
		m_pipeline.stop();
	}
}

std::unique_ptr<std::map<std::string, std::string>> RealsenseListener::getSensorInfo() const
{
	auto info_dict = std::make_unique<std::map<std::string, std::string>>();
	(*info_dict)["name"] = m_device.get_info(RS2_CAMERA_INFO_NAME);
	(*info_dict)["serial_number"] = m_device.get_info(RS2_CAMERA_INFO_SERIAL_NUMBER);
	(*info_dict)["firmware_version"] = m_device.get_info(RS2_CAMERA_INFO_FIRMWARE_VERSION);
	(*info_dict)["ip_address"] = m_device.get_info(RS2_CAMERA_INFO_IP_ADDRESS);
	(*info_dict)["advanced_mode"] = m_device.get_info(RS2_CAMERA_INFO_ADVANCED_MODE);
	(*info_dict)["dfu_device_path"] = m_device.get_info(RS2_CAMERA_INFO_DFU_DEVICE_PATH);
	(*info_dict)["camera_locked"] = m_device.get_info(RS2_CAMERA_INFO_CAMERA_LOCKED);
	(*info_dict)["asic_serial_number"] = m_device.get_info(RS2_CAMERA_INFO_ASIC_SERIAL_NUMBER);
	(*info_dict)["product_id"] = m_device.get_info(RS2_CAMERA_INFO_PRODUCT_ID);
	(*info_dict)["debug_op_code"] = m_device.get_info(RS2_CAMERA_INFO_DEBUG_OP_CODE);
	(*info_dict)["physical_port"] = m_device.get_info(RS2_CAMERA_INFO_PHYSICAL_PORT);
	(*info_dict)["usb_type_descriptor"] = m_device.get_info(RS2_CAMERA_INFO_USB_TYPE_DESCRIPTOR);
	(*info_dict)["recommended_firmware_version"] = m_device.get_info(RS2_CAMERA_INFO_RECOMMENDED_FIRMWARE_VERSION);
	(*info_dict)["product_line"] = m_device.get_info(RS2_CAMERA_INFO_PRODUCT_LINE);
	(*info_dict)["firmware_update_id"] = m_device.get_info(RS2_CAMERA_INFO_FIRMWARE_UPDATE_ID);
	return std::move(info_dict);
}

void RealsenseListener::onNewData(rs2::frameset data)
{	
	rs2::align m_align(RS2_STREAM_COLOR);
	rs2::frameset frameset = m_align.process(data);
	
	rs2::frame color_frame = frameset.get_color_frame();
	rs2::frame ir_frame = frameset.get_infrared_frame();
	rs2::frame depth_frame = frameset.get_depth_frame();
	const int width = color_frame.as<rs2::video_frame>().get_width();
	const int height = color_frame.as<rs2::video_frame>().get_height();
	
	cv::Mat color_mat(cv::Size(width, height), CV_8UC3, (void*)color_frame.get_data(), cv::Mat::AUTO_STEP);
	cv::Mat ir_mat(cv::Size(width, height), CV_16UC1, (void*)ir_frame.get_data(), cv::Mat::AUTO_STEP);
	cv::Mat depth_mat(cv::Size(width, height), CV_16UC1, (void*)depth_frame.get_data(), cv::Mat::AUTO_STEP);

	rs2::pointcloud pointcloud;
	rs2::points points = pointcloud.calculate(depth_frame);

	const rs2::vertex* vertices = points.get_vertices();

	auto p_grid = std::make_shared<Eigen::Tensor<float, 3>>(height, width, 3);
	auto p_ir = std::make_shared<Eigen::Tensor<unsigned char, 3>>(height, width, 1);
	auto p_rgb = std::make_shared<Eigen::Tensor<unsigned char, 3>>(height, width, 3);

	int row = 0;
	int col = 0;
	int num_pixels = height * width;
	for (size_t pixel = 0; pixel < num_pixels; pixel++)
	{
		if (pixel != 0 && pixel % width == 0)
		{
			col = 0;
			row++;
		}
		
		(*p_grid)(row, col, 0) = vertices[pixel].x;
		(*p_grid)(row, col, 1) = vertices[pixel].y;
		(*p_grid)(row, col, 2) = vertices[pixel].z;

		col++;
	}

	std::chrono::duration<double> seconds_duration(data.get_timestamp());
	
	const std::chrono::microseconds timestamp = std::chrono::duration_cast<std::chrono::microseconds>(seconds_duration); 

	// convert RGB frame
	Eigen::Tensor<unsigned char, 3> rgb_tensor(height, width, 3);
	cv::Mat rgb_image;
	cv::cvtColor(color_mat, rgb_image, cv::COLOR_BGR2RGB);
	cv::cv2eigen(rgb_image, rgb_tensor);
	Eigen::Tensor<unsigned char, 3> ir_tensor(height, width, 1);
	cv::cv2eigen(ir_mat, ir_tensor);
	*p_rgb = rgb_tensor.cast<unsigned char>();
	*p_ir = ir_tensor.cast<unsigned char>();
	
	auto p_composite_frame = std::make_shared<CompositeFrame>(timestamp, m_sensorInterfacePtr->getRGBMappable());
	p_composite_frame->addFrame(FrameID::RGB_IMAGE, std::make_shared<RGBFrame>(p_rgb, m_camParamsPtr, m_extrinsicPtr));
	p_composite_frame->addFrame(FrameID::GRAYSCALE_IMAGE, std::make_shared<GrayFrame>(p_ir, m_camParamsPtr, m_extrinsicPtr));
	p_composite_frame->addFrame(FrameID::POINTCLOUD_GRID, std::make_shared<GridFrame>(p_grid, m_camParamsPtr, m_extrinsicPtr));
	addToQueue(p_composite_frame);
}
