#ifndef GENERICDATAFRAME_H
#define GENERICDATAFRAME_H

struct CamParameters;
class SensorInterface;

#include <memory>
#include <chrono>

#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>
#include <opencv2/core.hpp>

/*!
 * @brief Abstract container class for a generic data frame type acquired by a sensor.
 *
 * A collection of these objects is stored in a CompositeFrame. Each contains raw data typed by the DataFrame subclass,
 * as well as the camera parameters and extrinsic matrix associated with the sensor.
 */
class GenericDataFrame
{
protected:

    int m_rows;
    int m_cols;
    int m_channels;

    std::shared_ptr<CamParameters> m_camParamsPtr;
    std::shared_ptr<Eigen::Matrix4f> m_extrinsicPtr;

    std::chrono::microseconds m_loadedTimestamp{};

public:

    /*!
     * @brief Constructor method to set CamParameters and extrinsic matrix for the data frame, as well as the number of 
     * rows, columns, and channels of the internal 3-D data tensor without knowledge of its type.
     * 
     * @param rows Number of rows in data tensor
     * @param cols Number of columns in data tensor
     * @param channels Number of channels in data tensor
     * @param p_cam_params Pointer to container for intrinsic matrix and distortion coefficients of the frame's creator sensor
     * @param p_extrinsic Pointer to the 4x4 extrinsic matrix of the sensor that acquired the frame relative to its identity sensor
     */
    GenericDataFrame(const int rows, const int cols, const int channels, const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic);

    /*!
     * @brief Virtual default destructor.
     */
    virtual ~GenericDataFrame();

    /*!
     * @brief Getter for the number of rows in the internal data tensor.
     * 
     * @return Reference to number of rows in the internal data tensor
     */
    const int& getRows() const;

    /*!
     * @brief Getter for the number of columns in the internal data tensor.
     * 
     * @return Reference to number of columns in the internal data tensor
     */
    const int& getCols() const;

    /*!
     * @brief Getter for the number of channels in the internal data tensor.
     * 
     * @return Reference to number of channels in the internal data tensor
     */
    const int& getChannels() const;

    /*!
     * @brief Getter for CamParameters object containing relevant camera parameters.
     * 
     * @return Pointer to the intrinsic matrix and distortion coefficients of the sensor that acquired the frame
     */
    std::shared_ptr<CamParameters> getCamParams() const;

    /*!
     * @brief Getter for the relevant extrinsic matrix.
     * 
     * @return Pointer to the 4x4 extrinsic matrix of the sensor that acquired the frame relative to its identity sensor
     */
    std::shared_ptr<Eigen::Matrix4f> getExtrinsic() const;

    /*!
     * @brief Getter for timestamp loaded from saved frame data.
     *
     * @return Epoch time in microseconds at which the data was created
     */
    const std::chrono::microseconds& getLoadedTimestamp() const;

    /*!
     * @brief Abstract method to resize resolution in place for all raw data in the instance.
     * 
     * Image resampling should be performed using nearest-neighbor interpolation to preserve sharp edges.
     * 
     * @param factor Float that acts as a fractional resizing factor
     */
    virtual void resize(const float factor) = 0;

    /*!
     * @brief Abstract method to convert the internal data tensor to an OpenCV matrix.
     * 
     * @return Pointer to cv::Mat object created from data tensor
     */
    virtual std::shared_ptr<cv::Mat> asCvMat() const = 0;

    /*!
     * @brief Abstract method to populate the internal data tensor with data from an OpenCV matrix.
     * 
     * @param mat Reference to existing cv::Mat object
     */
    virtual void fromCvMat(const cv::Mat& mat) = 0;

    /*!
     * @brief Abstract method to get a boolean mask of this instance to mask out zero points.
     * 
     * @return Pointer to boolean Eigen::Tensor that has 'true' at indices where this data frame is nonzero, 'false' elsewhere
     */
    virtual std::shared_ptr<Eigen::Tensor<bool, 3>> getNonzeroMask() = 0;

    /*!
     * @brief Abstract method that subclasses must implement to save internal raw data in an appropriate format.
     * 
     * @param file_path File path minus extension to which data will be saved
     */
    virtual void save(const std::string& file_path) = 0;

    /*!
     * @brief Abstract method that subclasses must implement to load internal raw data in an appropriate format.
     * 
     * @param file_path File path from which data will be saved
     * @param sensor_interface Object describing properties of the sensor whose data will be loaded
     */
    virtual void load(const std::string& file_path, const SensorInterface& sensor_interface) = 0;
};

#endif // GENERICDATAFRAME_H