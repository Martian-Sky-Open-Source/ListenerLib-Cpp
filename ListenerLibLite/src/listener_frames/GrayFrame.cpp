#include "listener_frames/GrayFrame.h"

class SensorInterface;

#include <string>
#include <memory>

#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>
#include <opencv2/core.hpp>

GrayFrame::GrayFrame(const std::string& file_path, const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic, const SensorInterface& sensor_interface)
    : DataFrame(p_cam_params, p_extrinsic)
{
    load(file_path, sensor_interface);
}

std::shared_ptr<cv::Mat> GrayFrame::asCvMat() const
{
    auto p_mat = std::make_shared<cv::Mat>(m_rows, m_cols, CV_8UC1);
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            p_mat->at<unsigned char>(i, j) = getElement(i, j, 0);
        }
    }
    return p_mat;
}

void GrayFrame::fromCvMat(const cv::Mat& mat)
{
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            getElement(i, j, 0) = mat.at<unsigned char>(i, j);
        }
    }
}

void GrayFrame::save(const std::string& file_path)
{
    const std::shared_ptr<cv::Mat> p_mat = asCvMat();
    cv::imwrite(file_path + ".png", *p_mat);
}

void GrayFrame::load(const std::string& file_path, const SensorInterface& sensor_interface)
{
    const cv::Mat mat = cv::imread(file_path, cv::IMREAD_GRAYSCALE);
    initializeTensor(mat.rows, mat.cols, mat.channels());
    fromCvMat(mat);
}