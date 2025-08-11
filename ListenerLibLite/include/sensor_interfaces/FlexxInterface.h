#ifndef FLEXXINTERFACE_H
#define FLEXXINTERFACE_H

struct CamParameters;

#include <unordered_map>

#include "sensor_interfaces/SensorInterface.h"

/*!
 * @brief Object that contains information about a connected or hypothetical FLEXX2 sensor.
 * 
 * Inherits SensorInterface
 */
class FlexxInterface final : public SensorInterface
{
	static const std::unordered_map<int, int> modeFramerateMap;

	const int m_mode;

public:

	/*!
	 * @brief Constructor method to set attributes.
	 * 
	 * @param mode FLEXX2 operation mode (4, 5, 6, 7, 8)
	 */
	explicit FlexxInterface(int mode = 4);

	/*!
	 * @brief Getter for FLEXX2 operation mode.
	 * 
	 * @return FLEXX2 operation mode
	 */
	const int& getMode() const;
};

#endif // FLEXXINTERFACE_H