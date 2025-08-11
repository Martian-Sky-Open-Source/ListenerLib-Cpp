#include "sensor_interfaces/SensorInterface.h"

#include <memory>

#include "listener_utils/general_utils.hpp"

SensorInterface::SensorInterface(const SensorID sensor_id, const int framerate, const bool rgb_mappable)
	: m_sensorID(sensor_id), m_framerate(framerate), m_rgbMappable(rgb_mappable)
{
}

SensorInterface::~SensorInterface()
{
}

const SensorID& SensorInterface::getSensorID() const
{
	return m_sensorID;
}

const int& SensorInterface::getFramerate() const
{
	return m_framerate;
}

const bool& SensorInterface::getRGBMappable() const
{
	return m_rgbMappable;
}

std::shared_ptr<CamParameters> SensorInterface::getDefaultCamParams() const
{
	return std::make_shared<CamParameters>();
}