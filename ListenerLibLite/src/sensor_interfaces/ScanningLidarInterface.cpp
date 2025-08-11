#include "sensor_interfaces/ScanningLidarInterface.h"

#include <string>
#include <unordered_map>
#include <memory>
#include <cmath>

#include <Eigen/Dense>

#include "listener_utils/general_utils.hpp"
#include "listener_utils/CamParameters.hpp"

const std::unordered_map<int, std::unordered_map<std::string, float>> ScanningLidarInterface::modeMap = {
    { 0, { {"filter_size", 1.0f} } },
};

ScanningLidarInterface::ScanningLidarInterface(const SensorID sensor_id, const int framerate, const int mode, const bool apply_processing, const float vertical_crop_ratio, const float aspect_ratio)
    : SensorInterface(sensor_id, framerate, false), m_applyProcessing(apply_processing), m_crop_ratio(vertical_crop_ratio), m_asp_ratio(aspect_ratio)
{
    m_propertyMap = modeMap.at(0);
}

int ScanningLidarInterface::getFilterSize() const
{
    if (m_applyProcessing)
    {
        return m_propertyMap.at("filter_size");
    }
    return 0;
}

std::shared_ptr<CamParameters> ScanningLidarInterface::getDefaultCamParams() const
{
    // Get camera parameters from property dictionary of preset values per mode
    std::shared_ptr<Eigen::Matrix3f> p_intrinsic;
    if (m_applyProcessing)
    {
        p_intrinsic = std::make_shared<Eigen::Matrix3f>(
            Eigen::Matrix3f{
                { m_propertyMap.at("focal"), 0.0f, std::floor(m_propertyMap.at("height") * m_crop_ratio * m_asp_ratio / 2.0f) },
                { 0.0f, m_propertyMap.at("focal"), std::floor(m_propertyMap.at("height") * m_crop_ratio / 2.0f)},
                { 0.0f, 0.0f, 1.0f }
            });
    }
    else
    {
        p_intrinsic = std::make_shared<Eigen::Matrix3f>(
            Eigen::Matrix3f{
                { m_propertyMap.at("focal"), 0.0f, m_propertyMap.at("width") / 2.0f },
                { 0.0f, m_propertyMap.at("focal"), m_propertyMap.at("height") / 2.0f },
                { 0.0f, 0.0f, 1.0f }
            });
    }
    auto p_distortion = std::make_shared<std::vector<float>>(5, 1.0f);
    return std::make_shared<CamParameters>(p_intrinsic, p_distortion);
}

void ScanningLidarInterface::pcdConditioner(open3d::geometry::PointCloud& pcd) const
{
}