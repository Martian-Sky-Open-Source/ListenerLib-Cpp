#ifndef FLEXXLISTENER_H
#define FLEXXLISTENER_H

class FlexxInterface;

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <royale.hpp>

#include "abstract_listeners/SingleListener.h"

/*!
 * @brief Implemented subclass that represents a physical PMD FLEXX2 LiDAR.
 * 
 * Inherits SingleListener and 'royale::IDepthDataListener'. Uses the 'royale' API and associated dynamic libraries.
 * 
 * A CompositeFrame from this listener contains:  
 * POINTCLOUD_GRID: numpy.ndarray[float32](M, N, 3) - Organized point cloud  
 * GRAYSCALE_IMAGE: numpy.ndarray[uint8](M, N) - Normalized grayscale image  
 * RGB_IMAGE: numpy.ndarray[uint8](M, N, 3) - Integer RGB color frame (NON-MAPPABLE)  
 * POINTCLOUD_MASK: numpy.ndarray[bool](M, N) - Boolean mask for organized point cloud
 */
class FlexxListener final : public SingleListener, public royale::IDepthDataListener
{
    static std::unique_ptr<royale::CameraManager> managerPtr;
    static royale::Vector<royale::String> deviceList;
    static int camNumber;

    std::unique_ptr<royale::ICameraDevice> m_sensorPtr;

public:

    /*!
     * @brief Constructor method that handles connecting to the physical FLEXX2 sensor.
     * 
     * @param p_sensor_interface Object containing information about a connected or hypothetical physical sensor
     * @param name Unique identifier for the implemented sensor
     * @param resize_factor A scaling factor the sensor applies to all produced data
     * @param param_dir Directory path from which camera parameters will be loaded if present
     */
    explicit FlexxListener(std::shared_ptr<FlexxInterface> p_sensor_interface, const std::string& name = "", const float resize_factor = 1.0f, const std::string& param_dir = "");

    /*!
     * @brief Implemented GenericListener::startStream method that starts the 'royale::IDepthDataListener' object's data stream.
     *
     * FlexxListener::onNewData is already registered to receive sensor data.
     */
    void startStream() override;

    /*!
     * @brief Implemented GenericListener::stopStream method that stops the 'royale::IDepthDataListener' object's data stream.
     */
    void stopStream() override;

    /*!
     * @brief Overrides GenericListener::getSensorInfo for this listener.
     * 
     * @return Pointer to a map with string keys and values corresponding to information fields and messages respectively
     */
    std::unique_ptr<std::map<std::string, std::string>> getSensorInfo() const override;

    /*!
     * @brief Overrides GenericListener::getSensorStatus for this listener.
     * 
     * @return Pointer to a map with string keys and values corresponding to status fields and messages respectively
     */
    std::unique_ptr<std::map<std::string, std::vector<std::string>>> getSensorStatus() const override;

    /*!
     * @brief Implements SingleListener::onNewData with this listener's processing requirements and adds frames expected of it to queue.
     * 
     * @param data A royale frame object containing latest sensor data
     */
    void onNewData(const royale::DepthData* data) override;
};

#endif // FLEXXLISTENER_H