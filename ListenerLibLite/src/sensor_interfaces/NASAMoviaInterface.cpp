#include "sensor_interfaces/NASAMoviaInterface.h"

#include <string>
#include <unordered_map>

#include <Eigen/Dense>

#include "listener_utils/general_utils.hpp"
#include "sensor_interfaces/ScanningLidarInterface.h"

const std::unordered_map<int, std::unordered_map<std::string, float>> NASAMoviaInterface::modeMap = {
    { 0, { {"focal", 86.6f}, {"width", 100.0f}, {"height", 58.0f}, {"filter_size", 1.0f} } },
    { 1, { {"focal", 216.5f}, {"width", 250.0f}, {"height", 146.0f}, {"filter_size", 3.0f} } },
    { 2, { {"focal", 433.0f}, {"width", 500.0f}, {"height", 294.0f}, {"filter_size", 5.0f} } },
    { 3, { {"focal", 649.4f}, {"width", 750.0f}, {"height", 440.0f}, {"filter_size", 7.0f} } },
    { 4, { {"focal", 866.0f}, {"width", 1000.0f}, {"height", 588.0f}, {"filter_size", 7.0f} } }
};

NASAMoviaInterface::NASAMoviaInterface(const int mode, const int framerate, const bool apply_processing)
    : ScanningLidarInterface(SensorID::MOVIA, framerate, mode, apply_processing)
{
    m_propertyMap = modeMap.at(mode);
}

void NASAMoviaInterface::pcdConditioner(open3d::geometry::PointCloud& pcd) const
{
    pcd.RemoveDuplicatedPoints();
    const std::vector<Eigen::Vector3d> points = pcd.points_;
    std::vector<Eigen::Vector3d> new_points;
    for (Eigen::Vector3d point : points)
    {
        Eigen::Vector3d new_point = {point[1], point[2], point[0]};
        if (0.2 < new_point[2] && new_point[2] < 84.0)
        {
            new_points.push_back(new_point);
        }
    }
    pcd.points_ = new_points;
}