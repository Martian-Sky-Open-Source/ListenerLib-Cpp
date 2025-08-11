#include "sensor_interfaces/FlexxInterface.h"

#include <unordered_map>

#include "listener_utils/general_utils.hpp"
#include "sensor_interfaces/SensorInterface.h"

const std::unordered_map<int, int> FlexxInterface::modeFramerateMap = {
    {8, 5}, {4, 10}, {5, 15}, {6, 20}, {7, 30}
};

FlexxInterface::FlexxInterface(const int mode)
    : SensorInterface(SensorID::FLEXX, modeFramerateMap.at(mode), false), m_mode(mode)
{
}

const int& FlexxInterface::getMode() const
{
    return m_mode;
}