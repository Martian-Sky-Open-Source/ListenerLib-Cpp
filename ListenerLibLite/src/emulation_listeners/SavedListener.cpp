#include "emulation_listeners/SavedListener.h"

#include <string>
#include <vector>
#include <chrono>
#include <memory>
#include <thread>
#include <iostream>
#include <filesystem>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>

#include <natural_sort.hpp>

#include "listener_utils/general_utils.hpp"
#include "listener_frames/GrayFrame.h"
#include "listener_frames/RGBFrame.h"
#include "listener_frames/GridFrame.h"
#include "listener_frames/TempFrame.h"
#include "listener_frames/MaskFrame.h"
#include "listener_frames/CompositeFrame.h"
#include "sensor_interfaces/SensorInterface.h"
#include "abstract_listeners/ThreadListener.h"
#include "abstract_listeners/SingleListener.h"

SavedListener::SavedListener(std::vector<FrameID> frame_id_vec, const std::string& data_dir, std::shared_ptr<SensorInterface> p_sensor_interface, const std::string& name, const float resize_factor, const bool repeat)
	: GenericListener(name, p_sensor_interface->getFramerate()), ThreadListener(name, p_sensor_interface->getFramerate()), SingleListener(p_sensor_interface, name, resize_factor, data_dir), m_repeat(repeat)
{
	// Collect and store vectors of files for each desired frame type
	bool covert_format = true;
	if (frame_id_vec.empty())
	{
		frame_id_vec.push_back(FrameID::POINTCLOUD_GRID);
		covert_format = false;
	}
	for (const auto& frame_id : frame_id_vec)
	{
		std::string file_dir = data_dir;
		if (covert_format)
		{
			file_dir = data_dir + "/" + FrameIDUtils::toString(frame_id);
		}
		std::vector<std::string> file_list = {};
		for (const auto& file : std::filesystem::directory_iterator(file_dir))
		{
			file_list.push_back(file.path().string());
		}
		SI::natural::sort(file_list);
		m_numFiles = file_list.size();
		m_fileVectMap[frame_id] = file_list;
	}
}

void SavedListener::streamLoop()
{
	size_t i = 0;
	bool print_flag = true;
	while (m_isStreaming)
	{
		// Add all data to CompositeFrame and add to queue with resizing
		auto timestamp = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch());
		const auto p_composite_frame = std::make_shared<CompositeFrame>(timestamp, m_sensorInterfacePtr->getRGBMappable());
		for (const auto& pair : m_fileVectMap)
		{
			std::shared_ptr<GenericDataFrame> p_data_frame;
			if (pair.first == FrameID::GRAYSCALE_IMAGE)
			{
				p_data_frame = std::make_shared<GrayFrame>(pair.second[i], m_camParamsPtr, m_extrinsicPtr, *m_sensorInterfacePtr);
			}
			else if (pair.first == FrameID::RGB_IMAGE)
			{
				p_data_frame = std::make_shared<RGBFrame>(pair.second[i], m_camParamsPtr, m_extrinsicPtr, *m_sensorInterfacePtr);
			}
			else if (pair.first == FrameID::POINTCLOUD_GRID)
			{
				p_data_frame = std::make_shared<GridFrame>(pair.second[i], m_camParamsPtr, m_extrinsicPtr, *m_sensorInterfacePtr);
			}
			else if (pair.first == FrameID::TEMPERATURE_GRID)
			{
				p_data_frame = std::make_shared<TempFrame>(pair.second[i], m_camParamsPtr, m_extrinsicPtr, *m_sensorInterfacePtr);
			}
			else if (pair.first == FrameID::POINTCLOUD_MASK)
			{
				p_data_frame = std::make_shared<MaskFrame>(pair.second[i], m_camParamsPtr, m_extrinsicPtr, *m_sensorInterfacePtr);
			}
			p_composite_frame->addFrame(pair.first, p_data_frame);
		}
		p_composite_frame->resizeAll(m_resizeFactor);
		addToQueue(p_composite_frame);
		if (i + 1 < m_numFiles)
		{
			++i;
		}
		else if (m_repeat)
		{
			if (print_flag)
			{
				std::cout << "Reached end of saved data, repeating last frame." << std::endl;
				print_flag = false;
			}
		}
		else
		{
			std::cout << "Reached end of saved data, stopping stream." << std::endl;
			m_isStreaming = false;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000) / m_framerate);
	}
}