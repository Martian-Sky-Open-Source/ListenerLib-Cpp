#ifndef REALSENSELISTENER_H
#define REALSENSELISTENER_H

#include <map>
#include <memory>
#include <librealsense2/rs.hpp>

#include "abstract_listeners/SingleListener.h"

#include "sensor_interfaces/RealsenseInterface.h"

/*!
 * @brief Implemented subclass that represents a physical Intel RealSense depth and color sensor.
 *
 * Inherits SingleListener. Uses the 'librealsense' API.
 *
 * A CompositeFrame from this listener contains:
 * POINTCLOUD_GRID: numpy.ndarray[float32](M, N, 3) - Organized point cloud
 * GRAYSCALE_IMAGE: numpy.ndarray[uint8](M, N) - Normalized grayscale image
 * RGB_IMAGE: numpy.ndarray[uint8](M, N, 3) - Integer RGB color frame
 * POINTCLOUD_MASK: numpy.ndarray[bool](M, N) - Boolean mask for organized point cloud
 */
class RealsenseListener final : public  SingleListener
{
	static rs2::device_list deviceList;
	static int camNumber;

	//rs2::align m_align(RS2_STREAM_COLOR);
	rs2::config m_config;
	rs2::pipeline m_pipeline;

	rs2::device m_device;

public:
	/*!
	 * @brief Constructor method that handles connecting to the physical RealSense sensor.
	 *
	 * @param p_sensor_interface Object containing information about a connected or hypothetical physical sensor
	 * @param name Unique identifier for the implemented sensor
	 * @param resize_factor A scaling factor the sensor applies to all produced data
	 * @param param_dir Directory path from which camera parameters will be loaded if present
	 */
	explicit RealsenseListener(std::shared_ptr<RealsenseInterface> p_sensor_interface, const std::string& name = "", const float resize_factor = 1.0f, const std::string& param_dir = "");

	/*!
	 * @brief Implemented GenericListener::startStream method that starts the 'rs2::pipeline' object's data stream.
	 *
	 * Uses RealsenseListener::onNewData as a callback to receive sensor data.
	 */
	void startStream() override;

	/*!
	 * @brief Implemented GenericListener::stopStream method that stops the 'rs2::pipeline' object's data stream.
	 */
	void stopStream() override;

	/*!
	 * @brief Overrides GenericListener::getSensorInfo for this listener.
	 *
	 * @return Pointer to a map with string keys and values corresponding to information fields and messages respectively
	 */
	std::unique_ptr<std::map<std::string, std::string>> getSensorInfo() const override;

	/*!
	 * @brief Implements SingleListener::onNewData with this listener's processing requirements and adds frames expected of it to queue.
	 *
	 * @param data A rs2 frame object containing latest sensor data
	 */
	void onNewData(rs2::frameset data);
};

#endif // REALSENSELISTENER_H