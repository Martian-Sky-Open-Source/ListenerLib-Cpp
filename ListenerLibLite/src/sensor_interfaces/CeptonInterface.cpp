#include "sensor_interfaces/CeptonInterface.h"

#include <string>
#include <unordered_map>

#include "listener_utils/general_utils.hpp"
#include "sensor_interfaces/ScanningLidarInterface.h"

const std::unordered_map<int, std::unordered_map<std::string, float>> CeptonInterface::modeMap = {
    { 0, { {"focal", 86.6f}, {"width", 100.0f}, {"height", 36.0f}, {"filter_size", 1.0f} } },
    { 1, { {"focal", 216.5f}, {"width", 250.0f}, {"height", 92.0f}, {"filter_size", 3.0f} } },
    { 2, { {"focal", 433.0f}, {"width", 500.0f}, {"height", 184.0f}, {"filter_size", 5.0f} } },
    { 3, { {"focal", 649.4f}, {"width", 750.0f}, {"height", 276.0f}, {"filter_size", 7.0f} } },
    { 4, { {"focal", 866.0f}, {"width", 1000.0f}, {"height", 368.0f}, {"filter_size", 7.0f} } }
};

CeptonInterface::CeptonInterface(const int mode, const int framerate, const bool apply_processing)
    : ScanningLidarInterface(SensorID::CEPTON, framerate, mode, apply_processing, 0.85f, 1.5f)
{
    m_propertyMap = modeMap.at(mode);
}