#ifndef CEPTONINTERFACE_H
#define CEPTONINTERFACE_H

struct CamParameters;

#include <unordered_map>

#include "sensor_interfaces/ScanningLidarInterface.h"

/*!
 * @brief Object that contains information about a connected or hypothetical Cepton sensor.
 * 
 * Inherits ScanningLidarInterface
 */
class CeptonInterface final : public ScanningLidarInterface
{
protected:

	static const std::unordered_map<int, std::unordered_map<std::string, float>> modeMap;

public:

	/*!
	 * @brief Constructor method to set attributes.
	 *
	 * @param mode Cepton operation mode (0, 1, 2, 3, 4) informing resolution and filtering scale
	 * @param framerate Nominal sensor framerate
	 * @param apply_processing true if data point cloud data should be cropped and processed, false if not
	 */
	explicit CeptonInterface(int mode = 2, int framerate = 2, bool apply_processing = true);
};

#endif // CEPTONINTERFACE_H