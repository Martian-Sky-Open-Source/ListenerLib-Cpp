#ifndef MOVIAINTERFACE_H
#define MOVIAINTERFACE_H

struct CamParameters;

#include <unordered_map>

#include "sensor_interfaces/ScanningLidarInterface.h"

/*!
 * @brief Object that contains information about a connected or hypothetical Movia sensor.
 * 
 * Inherits ScanningLidarInterface
 */
class NASAMoviaInterface final : public ScanningLidarInterface
{
protected:

	static const std::unordered_map<int, std::unordered_map<std::string, float>> modeMap;

public:

	/*!
	 * @brief Constructor method to set attributes.
	 *
	 * @param mode Movia operation mode (0, 1, 2, 3, 4) informing resolution and filtering scale
	 * @param framerate Nominal sensor framerate
	 * @param apply_processing true if data point cloud data should be cropped and processed, false if not
	 */
	explicit NASAMoviaInterface(int mode = 2, int framerate = 2, bool apply_processing = true);

	/*!
	 * @brief Method to reformat axes of unorganized Movia point clouds loaded from file.
	 *
	 * @param pcd Reference to Open3D PointCloud object that will be modified.
	 */
	void pcdConditioner(open3d::geometry::PointCloud& pcd) const override;
};

#endif // MOVIAINTERFACE_H