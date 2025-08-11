#include "abstract_listeners/GenericListener.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <memory>
#include <chrono>
#include <mutex>
#include <stdexcept>
#include <iostream>

#include <opencv2/core.hpp>
#include <open3d/Open3D.h>

#include "listener_utils/general_utils.hpp"
#include "listener_frames/CompositeFrame.h"
#include "listener_frames/GenericDataFrame.h"
#include "listener_utils/ListenerDisplayManager.h"

std::set<std::string> GenericListener::activeSensors;

void GenericListener::addToQueue(std::shared_ptr<CompositeFrame> p_composite_frame)
{
	std::lock_guard lock(m_queueMutex);
	m_queue.push(p_composite_frame);
	m_newFrame = true;
	m_queueEvent.notify_one();
}

GenericListener::GenericListener(const std::string& name, const int framerate) : m_name(name), m_framerate(framerate)
{
	// Add sensor's name to the static list of sensor names, checking if a sensor with the name exists already
	const auto result = activeSensors.insert(m_name);
	if (!result.second)
	{
		std::cerr << "A sensor named " << m_name << " already exists" << std::endl;
		throw std::runtime_error("Attempted to create a listener with a duplicate name.");
	}
}

GenericListener::~GenericListener()
{
	activeSensors.erase(m_name);
}

const std::string& GenericListener::getName() const
{
	return m_name;
}

const int& GenericListener::getFramerate() const
{
	return m_framerate;
}

const bool& GenericListener::getIsStreaming() const
{
	return m_isStreaming;
}

const std::chrono::milliseconds& GenericListener::getTimeoutDuration() const
{
	return m_timeoutDuration;
}

void GenericListener::setTimeoutDuration(const std::chrono::milliseconds& timeout_duration)
{
	m_timeoutDuration = timeout_duration;
}

std::unique_ptr<std::map<std::string, std::string>> GenericListener::getSensorInfo() const
{
	return std::make_unique<std::map<std::string, std::string>>();
}

std::unique_ptr<std::map<std::string, std::vector<std::string>>> GenericListener::getSensorStatus() const
{
	return std::make_unique<std::map<std::string, std::vector<std::string>>>();
}

void GenericListener::printInfoAndStatus() const
{
	std::cout << "\nSENSOR INFO:" << std::endl;

	std::unique_ptr<std::map<std::string, std::string>> p_info_map = getSensorInfo();

	for (const auto& pair : *p_info_map)
	{
		std::cout << pair.first << ": " << pair.second << std::endl;
	}

	std::cout << "\nSENSOR STATUS:" << std::endl;

	std::unique_ptr<std::map<std::string, std::vector<std::string>>> p_status_map = getSensorStatus();

	for (const auto& pair : *p_status_map)
	{
		std::cout << pair.first << ":" << std::endl;

		for (const auto& item : pair.second)
		{
			std::cout << item << std::endl;
		}

		std::cout << std::endl;
	}
}

std::shared_ptr<CompositeFrame> GenericListener::getNextFrame()
{
	// If queue is empty, wait for event telling this method a new frame was added to queue
	std::unique_lock<std::mutex> lock(m_queueMutex);
	if (m_queue.empty())
	{
		// Wait for event telling this method a new frame was added to queue
		m_newFrame = false;
		if (!m_queueEvent.wait_for(lock, m_timeoutDuration, [this] { return m_newFrame; }))
		{
			throw std::runtime_error("Timed out waiting for new frame.");
		}
	}

	// If frames in queue, get next frame in FIFO queue
	std::shared_ptr<CompositeFrame> p_composite_frame = m_queue.front();
	m_queue.pop();
	return p_composite_frame;
}

std::shared_ptr<CompositeFrame> GenericListener::getLatestFrame()
{
	// Wait for event telling this method a new frame was added to queue
	std::unique_lock<std::mutex> lock(m_queueMutex);
	if (!m_queueEvent.wait_for(lock, m_timeoutDuration, [this] { return m_newFrame; }))
	{
		throw std::runtime_error("Timed out waiting for new frame.");
	}

	// If frames in queue, store newest frame and iterate to discard older ones
	std::shared_ptr<CompositeFrame> p_composite_frame = m_queue.back();
	while (!m_queue.empty())
	{
		m_queue.pop();
	}
	m_newFrame = false;
	return p_composite_frame;
}

void GenericListener::displayStream()
{
	// Initialize Open3D PointCloud object and ListenerDisplayManager object, and start sensor stream
	const auto display_pcd = std::make_shared<open3d::geometry::PointCloud>();
	auto display_manager = ListenerDisplayManager(m_name, m_framerate, display_pcd);
	std::shared_ptr<CompositeFrame> p_composite_frame;
	std::shared_ptr<cv::Mat> disp_image = nullptr;
	startStream();
	while (true)
	{
		// Check queue for new frame
		try
		{
			p_composite_frame = getLatestFrame();
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			break;
		}

		//  Gather individual frames to output, checking to see which frames CompositeFrame object contains
		if (p_composite_frame->has(FrameID::RGB_IMAGE))
		{
			disp_image = p_composite_frame->getFrame(FrameID::RGB_IMAGE)->asCvMat();
		}
		if (p_composite_frame->has(FrameID::POINTCLOUD_GRID))
		{
			p_composite_frame->toPointCloud(*display_pcd);
		}
		if (p_composite_frame->has(FrameID::TEMPERATURE_GRID))
		{
			// TODO: Mechanism to output frame center temperature like in Python
		}

		// Update ListenerDisplayManager, stopping stream and loop if it is no longer active
		if (!display_manager.updateDisplay(p_composite_frame->getTimestamp(), disp_image))
		{
			break;
		}
	}
	stopStream();
}

void GenericListener::dumpStream(const std::string& dump_dir)
{
	unsigned int frame_number = 0;
	startStream();
	while (true)
	{
		std::shared_ptr<CompositeFrame> p_composite_frame;
		try
		{
			p_composite_frame = getLatestFrame();
		}
		catch (const std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			break;
		}

		++frame_number;
		p_composite_frame->saveAll(dump_dir, frame_number);
	}
	stopStream();
}