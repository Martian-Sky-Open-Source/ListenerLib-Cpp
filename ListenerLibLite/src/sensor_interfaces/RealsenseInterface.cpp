#include "sensor_interfaces/RealsenseInterface.h"

#include <vector>
#include <algorithm>
#include <stdexcept>

#include "listener_utils/general_utils.hpp"
#include "sensor_interfaces/SensorInterface.h"

std::vector valid_widths = { 1280, 848, 640, 480, 424, 256, 0 };
std::vector valid_heights = { 800, 720, 480, 360, 270, 240, 144, 100, 0 };
std::vector valid_framerates = { 5, 6, 15, 30, 60, 90, 100, 0 };

RealsenseInterface::RealsenseInterface(const int width, const int height, const int framerate)
	: SensorInterface(SensorID::REALSENSE, framerate, true), m_width(width), m_height(height)
{
	const bool invalid_settings = std::find(valid_widths.begin(), valid_widths.end(), width) == valid_widths.end() ||
							      std::find(valid_heights.begin(), valid_heights.end(), height) == valid_heights.end() ||
							      std::find(valid_framerates.begin(), valid_framerates.end(), framerate) == valid_framerates.end();
	if (invalid_settings)
	{
		throw std::runtime_error("Invalid RealSense settings.");
	}
}

const int& RealsenseInterface::getWidth() const
{
	return m_width;
}

const int& RealsenseInterface::getHeight() const
{
	return m_height;
}