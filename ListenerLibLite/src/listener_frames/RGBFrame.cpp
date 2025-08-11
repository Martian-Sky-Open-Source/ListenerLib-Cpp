#include "listener_frames/RGBFrame.h"

class SensorInterface;

#include <string>
#include <memory>

#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>
#include <opencv2/core.hpp>

RGBFrame::RGBFrame(const std::string& file_path, const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic, const SensorInterface& sensor_interface)
    : DataFrame(p_cam_params, p_extrinsic)
{
    load(file_path, sensor_interface);
}

std::shared_ptr<cv::Mat> RGBFrame::asCvMat() const
{
    auto p_mat = std::make_shared<cv::Mat>(m_rows, m_cols, CV_8UC3);
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            for (int k = 0; k < m_channels; ++k)
            {
                p_mat->at<cv::Vec<unsigned char, 3>>(i, j)[k] = getElement(i, j, k);
            }
        }
    }
    return p_mat;
}

void RGBFrame::fromCvMat(const cv::Mat& mat)
{
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            for (int k = 0; k < m_channels; ++k)
            {
                getElement(i, j, k) = mat.at<cv::Vec<unsigned char, 3>>(i, j)[k];
            }
        }
    }
}

void RGBFrame::save(const std::string& file_path)
{
    const std::shared_ptr<cv::Mat> p_mat = asCvMat();
    cv::cvtColor(*p_mat, *p_mat, cv::COLOR_RGB2BGR);
    cv::imwrite(file_path + ".png", *p_mat);
}

void RGBFrame::load(const std::string& file_path, const SensorInterface& sensor_interface)
{
    cv::Mat mat = cv::imread(file_path, cv::IMREAD_COLOR);
    cv::cvtColor(mat, mat, cv::COLOR_BGR2RGB);
    initializeTensor(mat.rows, mat.cols, mat.channels());
    fromCvMat(mat);
}