#ifndef MASKFRAME_H
#define MASKFRAME_H

class SensorInterface;

#include <memory>

#include <Eigen/Dense>
#include <opencv2/core.hpp>

#include "listener_frames/DataFrame.hpp"

/*!
 * @brief Container class for boolean mask of data acquired by a sensor.
 * 
 * Data stored in instance of this class is of the FrameID:  
 * POINTCLOUD_MASK: numpy.ndarray[bool](M, N) - Boolean mask for organized point cloud
 */
class MaskFrame final : public DataFrame<bool>
{
public:

	using DataFrame::DataFrame;

	/*!
	 * @brief Constructor to create an instance of this class by loading from a file.
	 *
	 * @param file_path String path to file from which data is loaded.
	 * @param p_cam_params Pointer to container for intrinsic matrix and distortion coefficients of the frame's creator sensor
	 * @param p_extrinsic Pointer to the 4x4 extrinsic matrix of the sensor that acquired the frame relative to its identity sensor
	 * @param sensor_interface Object describing properties of the sensor whose data will be loaded
	 */
	MaskFrame(const std::string& file_path, std::shared_ptr<CamParameters> p_cam_params, std::shared_ptr<Eigen::Matrix4f> p_extrinsic, const SensorInterface& sensor_interface);

	/*!
	 * @brief Overrides GenericDataFrame::asCvMat for this specific frame type.
	 * 
	 * @return Pointer to cv::Mat object created from data tensor
	 */
	std::shared_ptr<cv::Mat> asCvMat() const override;

	/*!
	 * @brief Overrides GenericDataFrame::fromCvMat for this specific frame type.
	 * 
	 * @param mat Reference to existing cv::Mat object
	 */
	void fromCvMat(const cv::Mat& mat) override;

	/*!
	 * @brief Overrides GenericDataFrame::save for this specific frame type.
	 * 
	 * @param file_path File path minus extension to which data will be saved
	 */
	void save(const std::string& file_path) override;

	/*!
	 * @brief Overrides GenericDataFrame::save for this specific frame type.
	 *
	 * @param file_path File path from which data will be loaded
	 * @param sensor_interface Object describing properties of the sensor whose data will be loaded
	 */
	void load(const std::string& file_path, const SensorInterface& sensor_interface) override;
};

#endif // MASKFRAME_H