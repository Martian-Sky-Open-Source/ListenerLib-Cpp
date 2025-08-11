#include "listener_frames/TempFrame.h"

class SensorInterface;

#include <string>
#include <vector>
#include <memory>

#include <npy.hpp>
#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>
#include <opencv2/core.hpp>

TempFrame::TempFrame(const std::string& file_path, const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic, const SensorInterface& sensor_interface)
    : DataFrame(p_cam_params, p_extrinsic)
{
    load(file_path, sensor_interface);
}

std::shared_ptr<cv::Mat> TempFrame::asCvMat() const
{
    auto p_mat = std::make_shared<cv::Mat>(m_rows, m_cols, CV_32FC1);
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            p_mat->at<float>(i, j) = getElement(i, j, 0);
        }
    }
    return p_mat;
}

void TempFrame::fromCvMat(const cv::Mat& mat)
{
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            getElement(i, j, 0) = mat.at<float>(i, j);
        }
    }
}

void TempFrame::save(const std::string& file_path)
{
    std::vector<float> grid_vec;
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            grid_vec.push_back(getElement(i, j, 0));
        }
    }

    npy::npy_data_ptr<float> data;
    data.data_ptr = grid_vec.data();
    data.shape = { static_cast<unsigned long>(m_rows), static_cast<unsigned long>(m_cols), static_cast<unsigned long>(m_channels) };
    data.fortran_order = false;
    npy::write_npy(file_path + ".npy", data);
}

void TempFrame::load(const std::string& file_path, const SensorInterface& sensor_interface)
{
    const npy::npy_data data = npy::read_npy<float>(file_path);
    const std::vector<float> data_vec = data.data;
    const std::vector<unsigned long> shape = data.shape;

    initializeTensor(shape[0], shape[1], 1);
    int counter = 0;
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            getElement(i, j, 0) = data_vec[counter++];
        }
    }
}