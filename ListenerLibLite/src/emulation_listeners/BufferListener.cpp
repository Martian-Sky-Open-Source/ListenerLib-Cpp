#include "emulation_listeners/BufferListener.h"

#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <thread>
#include <filesystem>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

#include <natural_sort.hpp>
#include <unsupported/Eigen/CXX11/Tensor>

#include "listener_frames/GrayFrame.h"
#include "listener_frames/RGBFrame.h"
#include "listener_frames/GridFrame.h"
#include "listener_frames/CompositeFrame.h"
#include "sensor_interfaces/SensorInterface.h"
#include "abstract_listeners/ThreadListener.h"
#include "abstract_listeners/SingleListener.h"

BufferListener::BufferListener(const std::string& buffer_dir, const std::shared_ptr<SensorInterface>& p_sensor_interface, const std::string& name, const float resize_factor, const std::string& param_dir)
	: GenericListener(name, p_sensor_interface->getFramerate()), ThreadListener(name, p_sensor_interface->getFramerate()), SingleListener(p_sensor_interface, name, resize_factor, param_dir), m_bufferDir(buffer_dir)
{
}

void BufferListener::streamLoop()
{
	while (m_isStreaming)
	{
		std::vector<std::string> file_vect = {};
		for (const auto& file : std::filesystem::directory_iterator(m_bufferDir))
		{
			file_vect.push_back(file.path().string());
		}
		SI::natural::sort(file_vect);
		if (file_vect.size() < 2)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000) / m_framerate);
			continue;
		}
		std::string current_file = file_vect.at(file_vect.size() - 2);
		if (current_file == m_prevFile)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000) / m_framerate);
			continue;
		}
		m_prevFile = current_file;

		auto p_grid_frame = std::make_shared<GridFrame>(current_file, m_camParamsPtr, m_extrinsicPtr, *m_sensorInterfacePtr);
		auto p_depth = std::make_shared<Eigen::Tensor<unsigned char, 3>>(p_grid_frame->getRows(), p_grid_frame->getCols(), 1);
		auto p_rgb = std::make_shared<Eigen::Tensor<unsigned char, 3>>(p_grid_frame->getRows(), p_grid_frame->getCols(), 3);
		Eigen::Tensor<float, 3> depth_grid = p_grid_frame->getData().slice(Eigen::array<Eigen::Index, 3>{0, 0, 2}, Eigen::array<Eigen::Index, 3>{p_grid_frame->getRows(), p_grid_frame->getCols(), 1});
		Eigen::Tensor<float, 0> depth_max = depth_grid.maximum();
		if (depth_max(0) == 0)
		{
			depth_max(0) = 1.0f;
		}
		depth_grid = depth_grid * 255.0f / depth_max(0);
		*p_depth = depth_grid.cast<unsigned char>();
		*p_rgb = p_depth->broadcast(Eigen::array<Eigen::Index, 3>{1, 1, 3});

		// Create composite frame object
		auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
		const auto p_composite_frame = std::make_shared<CompositeFrame>(timestamp, m_sensorInterfacePtr->getRGBMappable());

		// Add to data map and queue:
		p_composite_frame->addFrame(FrameID::GRAYSCALE_IMAGE, std::make_shared<GrayFrame>(p_depth, m_camParamsPtr, m_extrinsicPtr));
		p_composite_frame->addFrame(FrameID::RGB_IMAGE, std::make_shared<RGBFrame>(p_rgb, m_camParamsPtr, m_extrinsicPtr));
		p_composite_frame->addFrame(FrameID::POINTCLOUD_GRID, p_grid_frame);
		p_composite_frame->resizeAll(m_resizeFactor);
		addToQueue(p_composite_frame);
	}
}