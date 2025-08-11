#ifndef SENSORINTERFACE_H
#define SENSORINTERFACE_H

#include <memory>

#include "listener_utils/general_utils.hpp"
#include "listener_utils/CamParameters.hpp"

/*!
 * @brief Object that contains information about a connected or hypothetical physical sensor and verifies it.
 */
class SensorInterface
{
protected:

	const SensorID m_sensorID;
	const int m_framerate;
	const bool m_rgbMappable;

public:

	/*!
	 * @brief Constructor method to set attributes.
	 * 
	 * @param sensor_id Enum identifier for type of physical sensor
	 * @param framerate Nominal sensor framerate
	 * @param rgb_mappable Boolean informing whether this sensor's RGB data can be sensibly mapped to point cloud data
	 */
	SensorInterface(SensorID sensor_id, int framerate, bool rgb_mappable);

	/*!
	 * @brief Virtual default destructor
	 */
	virtual ~SensorInterface();

	/*!
	 * @brief Getter for physical sensor type identifier.
	 * 
	 * @return Sensor type identifier enum
	 */
	const SensorID& getSensorID() const;

	/*!
	 * @brief Getter for nominal sensor framerate.
	 * 
	 * @return Nominal sensor framerate
	 */
	const int& getFramerate() const;

	/*!
	 * @brief Getter for a boolean informing whether this sensor's RGB data can be sensibly mapped to point cloud data.
	 * 
	 * @return True if RGB data can be mapped to a point cloud, False if not
	 */
	const bool& getRGBMappable() const;

	/*!
	 * @brief Internal virtual method to return a set of default camera parameters.
	 * 
	 * @return Set of sensor-interface-specific default camera parameters
	 */
	virtual std::shared_ptr<CamParameters> getDefaultCamParams() const;
};

#endif // SENSORINTERFACE_H