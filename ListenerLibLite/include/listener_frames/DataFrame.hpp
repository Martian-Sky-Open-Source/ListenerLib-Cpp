#ifndef DATAFRAME_HPP
#define DATAFRAME_HPP

struct CamParameters;

#include <memory>

#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>
#include <opencv2/core.hpp>

#include "listener_frames/GenericDataFrame.h"

/*!
 * @brief Templated container class for a specific data frame type acquired by a sensor.
 * 
 * Adds internal data tensor to GenericDataFrame specialized for raw data type.
 * 
 * @tparam T Primitive data type of internal data tensor
 */
template <typename T>
class DataFrame : public GenericDataFrame
{
    std::shared_ptr<Eigen::Tensor<T, 3>> m_dataTensorPtr;

protected:

    /*!
     * @brief Method to create a new internal data tensor based on provided dimensions.
     * 
     * @param rows Number of rows in internal data tensor
     * @param cols Number of columns in internal data tensor
     * @param channels Number of channels in internal data tensor
     */
    void initializeTensor(const int rows, const int cols, const int channels)
    {
        m_rows = rows;
        m_cols = cols;
        m_channels = channels;
        m_dataTensorPtr = std::make_shared<Eigen::Tensor<T, 3>>(m_rows, m_cols, m_channels);
        m_dataTensorPtr->setZero();
    }

public:

    /*!
     * @brief Constructor to create an instance of this class without a defined Eigen::Tensor.
     *
     * @param p_cam_params Pointer to container for intrinsic matrix and distortion coefficients of the frame's creator sensor
     * @param p_extrinsic Pointer to the 4x4 extrinsic matrix of the sensor that acquired the frame relative to its identity sensor
     */
    DataFrame(const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic)
        : GenericDataFrame(0, 0, 0, p_cam_params, p_extrinsic)
    {
    }

    /*!
     * @brief Constructor to create an instance of this class from a pointer to an Eigen::Tensor.
     * 
     * @param p_tensor 3-dimensional Eigen::Tensor containing the pixel data in the frame
     * @param p_cam_params Pointer to container for intrinsic matrix and distortion coefficients of the frame's creator sensor
     * @param p_extrinsic Pointer to the 4x4 extrinsic matrix of the sensor that acquired the frame relative to its identity sensor
     */
    DataFrame(std::shared_ptr<Eigen::Tensor<T, 3>> p_tensor, const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic)
        : GenericDataFrame(p_tensor->dimension(0), p_tensor->dimension(1), p_tensor->dimension(2), p_cam_params, p_extrinsic), m_dataTensorPtr(p_tensor)
    {
    }

    /*!
     * @brief Getter for raw data contained in instance.
     * 
     * @return Mutable reference to the internal Eigen::Tensor containing the pixel data in the frame
     */
    Eigen::Tensor<T, 3>& getData()
    {
        return *m_dataTensorPtr;
    }

    /*!
     * @brief Const getter for raw data contained in instance.
     *
     * @return Immutable reference to the internal Eigen::Tensor containing the pixel data in the frame
     */
    const Eigen::Tensor<T, 3>& getData() const
    {
        return *m_dataTensorPtr;
    }

    /*!
     * @brief Getter for the data value in the internal Eigen::Tensor at the specified indices.
     *
     * @param i Row index for internal data tensor
     * @param j Column index for internal data tensor
     * @param k Channel index for internal data tensor
     * @return Mutable reference to the data value contained at the indices
     */
    T& getElement(const unsigned int i, const unsigned int j, const unsigned int k)
    {
        return (*m_dataTensorPtr)(i, j, k);
    }

    /*!
     * @brief Const getter for the data value in the internal Eigen::Tensor at the specified indices.
     * 
     * @param i Row index for internal data tensor
     * @param j Column index for internal data tensor
     * @param k Channel index for internal data tensor
     * @return Immutable reference to the data value contained at the indices
     */
    const T& getElement(const unsigned int i, const unsigned int j, const unsigned int k) const
    {
        return (*m_dataTensorPtr)(i, j, k);
    }

    /*!
     * @brief Method to resize resolution in place for all raw data in the instance.
     * 
     * Image resampling performed using nearest-neighbor interpolation to preserve sharp edges.
     * 
     * @param factor Float that acts as a fractional resizing factor
     */
    void resize(const float factor) override
    {
        const std::shared_ptr<cv::Mat> p_mat = asCvMat();
        cv::resize(*p_mat, *p_mat, cv::Size(), factor, factor, cv::INTER_NEAREST);
        initializeTensor(p_mat->rows, p_mat->cols, p_mat->channels());
        fromCvMat(*p_mat);
    }

    /*!
     * @brief Method to get a boolean mask of this instance to mask out zero points.
     * 
     * @return Pointer to boolean Eigen::Tensor that has 'true' at indices where this data frame is nonzero, 'false' elsewhere
     */
    std::shared_ptr<Eigen::Tensor<bool, 3>> getNonzeroMask() override
    {
        auto mask_frame = std::make_shared<Eigen::Tensor<bool, 3>>(m_rows, m_cols, 1);
        mask_frame->setConstant(false);

        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_cols; ++j)
            {
                for (int k = 0; k < m_channels; ++k)
                {
                    (*mask_frame)(i, j, 0) = (*mask_frame)(i, j, 0) || (*m_dataTensorPtr)(i, j, k) != 0;
                }
            }
        }
        return mask_frame;
    }
};

#endif // DATAFRAME_HPP