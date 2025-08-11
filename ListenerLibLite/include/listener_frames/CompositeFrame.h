#ifndef COMPOSITEFRAME_H
#define COMPOSITEFRAME_H

struct CamParameters;
class GenericDataFrame;

#include <unordered_map>
#include <memory>
#include <chrono>

#include <open3d/Open3D.h>

#include "listener_utils/general_utils.hpp"

/*!
 * @brief Class that contains and provides processing methods for all data frames acquired at a given time by a sensor.
 * 
 * Holds GenericDataFrame objects, with at most one instance of each frame type stored in an internal dictionary.
 * Each is mapped to a corresponding unique FrameID key. The GenericListener::getNextFrame and
 * GenericListener::getLatestFrame methods return an instance of this class that should always contain a 'GRAYSCALE_IMAGE'
 * frame type and an 'RGB_IMAGE' frame type.
 * 
 * Types of frames that can be added/extracted from an instance of this class:  
 * POINTCLOUD_GRID: numpy.ndarray[float32](M, N, 3) - Organized point cloud  
 * GRAYSCALE_IMAGE: numpy.ndarray[uint8](M, N) - Normalized grayscale image  
 * RGB_IMAGE: numpy.ndarray[uint8](M, N, 3) - Integer RGB color frame  
 * TEMPERATURE_GRID: numpy.ndarray[float32](M, N) - Frame of float temperature values  
 * POINTCLOUD_MASK: numpy.ndarray[bool](M, N) - Boolean mask for organized point cloud
 */
class CompositeFrame
{
	std::unordered_map<FrameID, std::shared_ptr<GenericDataFrame>> m_dataMap;
	std::chrono::microseconds m_timestamp;

protected:

	const bool m_rgbMappable;

public:

	/*!
	 * @brief Constructor method for creating a new container with a fixed timestamp.
	 * 
	 * @param timestamp Epoch time in microseconds at which contained frame data was received by the sensor
	 * @param mappable A boolean informing whether this instance's RGB data can be sensibly mapped to its point cloud data
	 */
	explicit CompositeFrame(const std::chrono::microseconds& timestamp, bool mappable = true);

	/*!
	 * @brief Getter for data creation timestamp.
	 * 
	 * @return Epoch time in microseconds at which the contained data was received from a sensor
	 */
	const std::chrono::microseconds& getTimestamp() const;

	/*!
	 * @brief Adds a GenericDataFrame object to this instance's internal frame dictionary. If the frame is a 'POINTCLOUD_GRID',
	 * a 'POINTCLOUD_MASK' that masks out all (0, 0, 0) points is automatically created and added as well.
	 * 
	 * @param frame_id Unique (per instance) identifier for the type of GenericDataFrame
	 * @param p_data_frame A pointer to an instance of a GenericDataFrame subclass
	 */
	void addFrame(FrameID frame_id, std::shared_ptr<GenericDataFrame> p_data_frame);

	/*!
	 * @brief Const getter for specified GenericDataFrame pointer in container using a FrameID enum key.
	 *
	 * @param frame_id Desired type of data frame
	 * @return Const pointer to specified GenericDataFrame object in container
	 */
	std::shared_ptr<GenericDataFrame> getFrame(FrameID frame_id) const;

	/*!
	 * @brief Getter for specified GenericDataFrame pointer in container using a FrameID enum key.
	 * 
	 * @param frame_id Desired type of data frame
	 * @return Pointer to specified GenericDataFrame object in container
	 */
	std::shared_ptr<GenericDataFrame> getFrame(FrameID frame_id);

	/*!
	 * @brief Checks whether a GenericDataFrame of a certain FrameID resides in the internal data map.
	 * 
	 * @param frame_id Type of GenericDataFrame to check for
	 * @return 'true' if specified frame type is contained in internal data map, 'false' if not
	 */
	bool has(FrameID frame_id) const;

	/*!
	 * @brief Method to resize resolution in place for all GenericDataFrame objects in the instance.
	 *
	 * Calls all internal data frames' GenericDataFrame::resize method.
	 * 
	 * @param factor Float that acts as a fractional resizing factor
	 */
	void resizeAll(float factor);

	/*!
	 * @brief Writes all raw data in instance to file with format based on FrameID.
	 *
	 * File name includes the identifier string representation and the current frame number.
	 * 
	 * @param save_dir Path to directory where all data will be saved in individual data type directories
	 * @param frame_number Identifies instance number based on total number of instances created by a given sensor
	 */
	void saveAll(const std::string& save_dir, unsigned int frame_number) const;

	/*!
	 * @brief Converts the instance's organized point cloud into an unorganized one and populates an Open3D PointCloud.
     * 
	 * Also gives the PointCloud colors corresponding to the instance's RGB image.
	 * 
	 * @param pcd Handle to Open3D PointCloud object to be populated with instance's organized point cloud
	 */
	void toPointCloud(open3d::geometry::PointCloud& pcd) const;
};

#endif // COMPOSITEFRAME_H