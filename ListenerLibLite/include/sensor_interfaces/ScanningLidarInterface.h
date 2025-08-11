#ifndef SCANNINGLIDARINTERFACE_H
#define SCANNINGLIDARINTERFACE_H

struct CamParameters;

#include <unordered_map>
#include <memory>

#include "listener_utils/general_utils.hpp"
#include "sensor_interfaces/SensorInterface.h"

/*!
 * @brief Object that contains information about a connected or hypothetical scanning LiDAR sensor and verifies it
 */
class ScanningLidarInterface : public SensorInterface
{
    const bool m_applyProcessing;

    const float m_crop_ratio;
    const float m_asp_ratio;

protected:

    static const std::unordered_map<int, std::unordered_map<std::string, float>> modeMap;

    std::unordered_map<std::string, float> m_propertyMap;

public:
    /*!
     * @brief Constructor method.
     *
     * @param sensor_id Enum identifier for type of physical sensor
     * @param framerate Nominal sensor framerate
     * @param mode Scanning LiDAR operation mode (0, 1, 2, 3, 4) informing resolution and filtering scale
     * @param apply_processing True if data point cloud data should be cropped and processed, False if not
     * @param vertical_crop_ratio Fraction of frame height to remain for processed scanning LiDAR depth frame
     * @param aspect_ratio Desired aspect ratio for processed scanning LiDAR depth frame
     */
    ScanningLidarInterface(SensorID sensor_id, int framerate, int mode, bool apply_processing, float vertical_crop_ratio = 1.0f, float aspect_ratio = 1.0f);

    /*!
     * @brief Getter for size of median filter window applied when organizing point cloud.
     *
     * @return Pixel size of floor median filter window
     */
    int getFilterSize() const;

    /*!
     * @brief Overrides SensorInterface::getDefaultCamParams.
     *
     * @return Set of sensor-interface-specific default camera parameters
     */
    std::shared_ptr<CamParameters> getDefaultCamParams() const override;

    /*!
     * @brief Dummy method to condition unorganized point cloud from file, shadow in subclasses for custom behavior.
     *
     * @param pcd Reference to Open3D PointCloud object that will be modified
     */
    virtual void pcdConditioner(open3d::geometry::PointCloud& pcd) const;
};

#endif //SCANNINGLIDARINTERFACE_H
