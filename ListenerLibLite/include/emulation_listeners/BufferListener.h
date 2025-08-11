#ifndef BUFFERLISTENER_H
#define BUFFERLISTENER_H

class SensorInterface;

#include <string>
#include <memory>

#include "abstract_listeners/ThreadListener.h"
#include "abstract_listeners/SingleListener.h"

/*!
 * @brief Implemented subclass that scans a folder for buffered point cloud data saved to file.
 *
 * Inherits SingleListener and ThreadListener. Initialized with parameters of sensor to be emulated via
 * constructor and by loading relevant camera parameters and extrinsic matrix.
 */
class BufferListener final : public ThreadListener, public SingleListener
{

	const std::string m_bufferDir;

	std::string m_prevFile;

public:

	/*!
	 * @brief Constructor method, initializes subclass and superclass members. 
	 * 
	 * @param buffer_dir Path to data buffer directory
	 * @param p_sensor_interface Object containing information about a connected or hypothetical physical sensor
	 * @param name Unique identifier for the implemented sensor
	 * @param resize_factor A scaling factor the sensor applies to all produced data
	 * @param param_dir Directory path from which camera parameters will be loaded if present
	 */
	BufferListener(const std::string& buffer_dir, const std::shared_ptr<SensorInterface>& p_sensor_interface, const std::string& name = "", const float resize_factor = 1.0f, const std::string& param_dir="");

	/*!
	 * @brief Implements ThreadListener::streamLoop.
	 *
	 * In another thread, reads in point cloud data from buffer folder using the appropriate loading methods and 
	 * adds them to queue as a :class:`~ListenerLib.listener_frames.CompositeFrame`. Sleeps between unsuccessful
	 * checks according to given framerate.
	 */
	void streamLoop() override;
};

#endif // BUFFERLISTENER_H