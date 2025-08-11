#ifndef SAVEDLISTENER_H
#define SAVEDLISTENER_H

class SensorInterface;

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "listener_utils/general_utils.hpp"
#include "abstract_listeners/ThreadListener.h"
#include "abstract_listeners/SingleListener.h"

/*!
 * @brief Implemented subclass that emulates an arbitrary sensor by adding previously saved data to queue.
 * 
 * Inherits SingleListener and ThreadListener. Initialized with parameters of sensor to be emulated via
 * constructor and by loading relevant camera parameters and extrinsic matrix.
 */
class SavedListener final : public ThreadListener, public SingleListener
{
	const bool m_repeat;

	size_t m_numFiles;

	std::unordered_map<FrameID, std::vector<std::string>> m_fileVectMap;

public:

	/*!
	 * @brief Constructor method, initializes subclass and superclass members.
	 * 
	 * @param frame_id_vec Vector of identifiers that inform what frame types will be used, empty means assume raw point cloud data
	 * @param data_dir Directory path containing frame data folders and sensor data to be used
	 * @param p_sensor_interface Pointer to object containing information about a connected or hypothetical physical sensor
	 * @param name Unique name of emulated sensor
	 * @param resize_factor A scaling factor the sensor applies to all produced data
	 * @param repeat Boolean informing whether last frame of save data should be repeated indefinitely
	 */
	SavedListener(std::vector<FrameID> frame_id_vec, const std::string& data_dir, std::shared_ptr<SensorInterface> p_sensor_interface, const std::string& name = "", const float resize_factor = 1.0f, const bool repeat = false);

	/*!
	 * @brief Implements ThreadListener::streamLoop.
	 *
	 * In another thread, reads in data frames from internal file lists using the appropriate loading methods
	 * and adds them to queue as a CompositeFrame. Sleeps after each addition to emulate given framerate.
	 */
	void streamLoop() override;
};

#endif // SAVEDLISTENER_H