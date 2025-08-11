#include "listener_frames/GridFrame.h"

#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <stdexcept>
#include <chrono>
#include <filesystem>

#include <npy.hpp>
#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>
#include <opencv2/core.hpp>
#include <opencv2/calib3d.hpp>
#include <open3d/Open3D.h>

#include "listener_utils/CamParameters.hpp"
#include "sensor_interfaces/SensorInterface.h"
#include "sensor_interfaces/ScanningLidarInterface.h"

float GridFrame::floorMedianFilter(std::vector<float>& window)
{
    const auto max_it = std::max_element(window.begin(), window.end());
    if (max_it != window.end() && *max_it > 0.0f)
    {
        std::sort(window.begin(), window.end());
        const size_t length = window.size();
        if (length % 2 == 1)
        {
            return window[std::floor(length / 2)];
        }
        return window[std::floor(length / 2) - 1];
    }
    return 0.0f;
}

std::shared_ptr<cv::Mat> GridFrame::applyCustomFilter(cv::Mat& image, const int kernel_size) const
{
    auto clone_image = image.clone();
    auto new_image = std::make_shared<cv::Mat>(clone_image);
    const int half_kernel_size = std::floor((kernel_size + 1) / 2);
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            const int window_top = std::max(0, i - half_kernel_size);
            const int window_lft = std::max(0, j - half_kernel_size);
            const int window_btm = std::min(m_rows, i + half_kernel_size);
            const int window_rgt = std::min(m_cols, j + half_kernel_size);
            std::vector<float> window;
            for (int n = window_top; n < window_btm; ++n)
            {
                for (int m = window_lft; m < window_rgt; ++m)
                {
                    float pixel = image.at<float>(n, m);
                    if (pixel > 0)
                    {
                        window.push_back(pixel);
                    }
                }
            }
            new_image->at<float>(i, j) = floorMedianFilter(window);
        }
    }
    return new_image;
}

void GridFrame::organizePcd(std::vector<Eigen::Vector3d>& point_array, const int filter_size)
{
    // Convert point array to OpenCV format
    std::vector<cv::Point3f> point_array_cv;
    for (const auto& point : point_array)
    {
        cv::Point3f point_cv = {static_cast<float>(point[0]), static_cast<float>(point[1]), static_cast<float>(point[2])};
        point_array_cv.push_back(point_cv);
    }

    // Convert intrinsic matrix to OpenCV format
    Eigen::Matrix3f& intrinsic = *(m_camParamsPtr->m_intrinsicPtr);
    cv::Mat intrinsic_cv(3, 3, CV_32F);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            intrinsic_cv.at<float>(i, j) = intrinsic(i, j);
        }
    }

    // Transform vector of points to image frame and get array of corresponding pixel coordinates
    std::vector<cv::Point2f> pixel_array;
    cv::projectPoints(point_array_cv, std::vector(3, 0.0f), std::vector(3, 0.0f), intrinsic_cv, std::vector(5, 0.0f), pixel_array);

    // Pick out pixels and points that lie within the image bounds
    initializeTensor(static_cast<int>(intrinsic(1, 2) * 2), static_cast<int>(intrinsic(0, 2) * 2), 3);
    std::vector<cv::Point2f> valid_pixels;
    std::vector<Eigen::Vector3d> valid_points;
    for (int i = 0; i < pixel_array.size(); ++i)
    {
        if (pixel_array[i].x >= 0 && pixel_array[i].x < m_cols && pixel_array[i].y >= 0 && pixel_array[i].y < m_rows)
        {
            valid_pixels.push_back(pixel_array[i]);
            valid_points.push_back(point_array[i]);
        }
    }

    if (filter_size > 0)
    {
        // Populate depth frame with point cloud depth based on valid projected pixel coordinates
        cv::Mat depth_image_cv = cv::Mat::zeros(m_rows, m_cols, CV_32F);
        for (int i = 0; i < valid_pixels.size(); ++i)
        {
            depth_image_cv.at<float>(valid_pixels[i].y, valid_pixels[i].x) = valid_points[i](2);
        }

        cv::Mat filtered_depth_image_cv = *applyCustomFilter(depth_image_cv, filter_size);

        open3d::geometry::Image depth_image_o3d;
        depth_image_o3d.Prepare(m_cols, m_rows, 1, sizeof(float));
        memcpy(depth_image_o3d.data_.data(), filtered_depth_image_cv.data, m_rows * m_cols * sizeof(float));

        // Transform back to camera frame
        open3d::camera::PinholeCameraIntrinsic intrinsic_o3d(m_cols, m_rows, intrinsic.cast<double>());
        std::shared_ptr<open3d::geometry::PointCloud> pcd = open3d::geometry::PointCloud::CreateFromDepthImage(depth_image_o3d, intrinsic_o3d, Eigen::Matrix4d::Identity(), 1.0);
        
        // Populate grid frame with point cloud data
        int m = 0;
        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_cols; ++j)
            {
                if (filtered_depth_image_cv.at<float>(i, j) > 0.0f)
                {
                    for (int k = 0; k < 3; ++k)
                    {
                        getElement(i, j, k) = pcd->points_[m](k);
                    }
                    ++m;
                }
            }
        }
    }
    else
    {
        // Populate grid frame with point cloud points at valid indices
        for (int i = 0; i < valid_pixels.size(); ++i)
        {
            for (int k = 0; k < 3; ++k)
            {
                getElement(valid_pixels[i].y, valid_pixels[i].x, k) = valid_points[i](k);
            }
        }
    }
}

GridFrame::GridFrame(const std::string& file_path, const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic, const SensorInterface& sensor_interface)
    : DataFrame(p_cam_params, p_extrinsic)
{
    load(file_path, sensor_interface);
}

std::shared_ptr<cv::Mat> GridFrame::asCvMat() const
{
    auto p_mat = std::make_shared<cv::Mat>(m_rows, m_cols, CV_32FC3);
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            for (int k = 0; k < m_channels; ++k)
            {
                p_mat->at<cv::Vec3f>(i, j)[k] = getElement(i, j, k);
            }
        }
    }
    return p_mat;
}

void GridFrame::fromCvMat(const cv::Mat& mat)
{
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            for (int k = 0; k < m_channels; ++k)
            {
                getElement(i, j, k) = mat.at<cv::Vec3f>(i, j)[k];
            }
        }
    }
}

void GridFrame::save(const std::string& file_path)
{
    std::vector<float> grid_vec;
    for (int i = 0; i < m_rows; ++i)
    {
        for (int j = 0; j < m_cols; ++j)
        {
            for (int k = 0; k < m_channels; ++k)
            {
                grid_vec.push_back(getElement(i, j, k));
            }
        }
    }

    npy::npy_data_ptr<float> data;
    data.data_ptr = grid_vec.data();
    data.shape = { static_cast<unsigned long>(m_rows), static_cast<unsigned long>(m_cols), static_cast<unsigned long>(m_channels) };
    data.fortran_order = false;
    npy::write_npy(file_path + ".npy", data);
}

void GridFrame::load(const std::string& file_path, const SensorInterface& sensor_interface)
{
    std::filesystem::path extension = std::filesystem::path(file_path).extension();
    if (extension == ".npy")
    {
        npy::npy_data data = npy::read_npy<float>(file_path);
        std::vector<float> data_vec = data.data;
        std::vector<unsigned long> shape = data.shape;

        initializeTensor(shape[0], shape[1], shape[2]);
        int counter = 0;
        for (int i = 0; i < m_rows; ++i)
        {
            for (int j = 0; j < m_cols; ++j)
            {
                for (int k = 0; k < m_channels; ++k)
                {
                    getElement(i, j, k) = data_vec[counter++];
                }
            }
        }
    }
    else if (extension == ".ply")
    {
        open3d::geometry::PointCloud pcd;
        open3d::io::ReadPointCloudOption option;
        open3d::io::ReadPointCloudFromPLY(file_path, pcd, option);
        try
        {
            const auto& scan_lidar_interface = dynamic_cast<const ScanningLidarInterface&>(sensor_interface);
            scan_lidar_interface.pcdConditioner(pcd);
            organizePcd(pcd.points_, scan_lidar_interface.getFilterSize());
        }
        catch (const std::bad_cast& e)
        {
            throw std::runtime_error("File format requires a 'ScanningLidarInterface' object.");
        }
    }
    else if (extension == ".npz")
    {
        // TODO: Need new npy/npz library if we want to handle loading of .npz files as in SCINet real time case.
        throw std::runtime_error("Invalid file format loading point cloud data.");
    }
    else
    {
        throw std::runtime_error("Invalid file format loading point cloud data.");
    }
}