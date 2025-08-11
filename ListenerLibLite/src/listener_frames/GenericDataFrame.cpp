#include "listener_frames/GenericDataFrame.h"

struct CamParameters;

#include <memory>
#include <chrono>

#include <Eigen/Dense>

GenericDataFrame::GenericDataFrame(const int rows, const int cols, const int channels, const std::shared_ptr<CamParameters> p_cam_params, const std::shared_ptr<Eigen::Matrix4f> p_extrinsic)
    : m_rows(rows), m_cols(cols), m_channels(channels), m_camParamsPtr(p_cam_params), m_extrinsicPtr(p_extrinsic)
{
}

GenericDataFrame::~GenericDataFrame()
{
}

const int& GenericDataFrame::getRows() const
{
    return m_rows;
}

const int& GenericDataFrame::getCols() const
{
    return m_cols;
}

const int& GenericDataFrame::getChannels() const
{
    return m_channels;
}

std::shared_ptr<CamParameters> GenericDataFrame::getCamParams() const
{
    return m_camParamsPtr;
}

std::shared_ptr<Eigen::Matrix4f> GenericDataFrame::getExtrinsic() const
{
    return m_extrinsicPtr;
}

const std::chrono::microseconds& GenericDataFrame::getLoadedTimestamp() const
{
    return m_loadedTimestamp;
}