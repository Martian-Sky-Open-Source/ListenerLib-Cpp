#include "listener_frames/CompositeFrame.h"

#include <chrono>
#include <memory>
#include <stdexcept>
#include <filesystem>

#include <open3d/Open3D.h>

#include "listener_utils/general_utils.hpp"
#include "listener_frames/GenericDataFrame.h"
#include "listener_frames/MaskFrame.h"
#include "listener_frames/RGBFrame.h"
#include "listener_frames/GridFrame.h"

CompositeFrame::CompositeFrame(const std::chrono::microseconds& timestamp, const bool mappable)
	: m_timestamp(timestamp), m_rgbMappable(mappable)
{
}

const std::chrono::microseconds& CompositeFrame::getTimestamp() const
{
	return m_timestamp;
}

void CompositeFrame::addFrame(const FrameID frame_id, const std::shared_ptr<GenericDataFrame> p_data_frame)
{
	m_dataMap[frame_id] = p_data_frame;
	if (p_data_frame->getLoadedTimestamp() != std::chrono::microseconds::zero())
	{
		m_timestamp = p_data_frame->getLoadedTimestamp();
	}
	if (frame_id == FrameID::POINTCLOUD_GRID && !has(FrameID::POINTCLOUD_MASK))
	{
		const auto p_mask_frame = std::make_shared<MaskFrame>(p_data_frame->getNonzeroMask(), p_data_frame->getCamParams(), p_data_frame->getExtrinsic());
		addFrame(FrameID::POINTCLOUD_MASK, p_mask_frame);
	}
}

std::shared_ptr<GenericDataFrame> CompositeFrame::getFrame(const FrameID frame_id) const
{
	if (!has(frame_id))
	{
		std::cerr << "Tried to access a CompositeFrame's " << FrameIDUtils::toString(frame_id) << " when it does not have one." << std::endl;
		throw std::runtime_error("Frame ID not found in CompositeFrame.");
	}
	return m_dataMap.at(frame_id);
}

std::shared_ptr<GenericDataFrame> CompositeFrame::getFrame(const FrameID frame_id)
{
	if (!has(frame_id))
	{
		std::cerr << "Tried to access a CompositeFrame's " << FrameIDUtils::toString(frame_id) << " when it does not have one." << std::endl;
		throw std::runtime_error("Frame ID not found in CompositeFrame.");
	}
	return m_dataMap.at(frame_id);
}

bool CompositeFrame::has(const FrameID frame_id) const
{
	if (m_dataMap.find(frame_id) == m_dataMap.end())
	{
		return false;
	}
	return true;
}

// ReSharper disable once CppMemberFunctionMayBeConst
void CompositeFrame::resizeAll(const float factor)
{
	if (factor != 1.0)
	{
		for (const auto& pair : m_dataMap)
		{
			if (pair.first == FrameID::POINTCLOUD_MASK)
			{
				continue;
			}
			pair.second->resize(factor);
		}
	}
}

void CompositeFrame::saveAll(const std::string& save_dir, const unsigned int frame_number) const
{
	for (const auto& pair : m_dataMap)
	{
		std::string frame_dir = save_dir + "/" + FrameIDUtils::toString(pair.first);
		if (!std::filesystem::is_directory(frame_dir))
		{
			std::filesystem::create_directory(frame_dir);
		}
		std::string file_path = frame_dir + "/" + FrameIDUtils::toString(pair.first) + std::to_string(frame_number);
		pair.second->save(file_path);
	}
}

void CompositeFrame::toPointCloud(open3d::geometry::PointCloud& pcd) const
{
	pcd.Clear();
	Eigen::Vector3d point;
	Eigen::Vector3d color;
	for (int i = 0; i < m_dataMap.at(FrameID::POINTCLOUD_GRID)->getRows(); ++i)
	{
		for (int j = 0; j < m_dataMap.at(FrameID::POINTCLOUD_GRID)->getCols(); ++j)
		{
			if (std::static_pointer_cast<MaskFrame>(getFrame(FrameID::POINTCLOUD_MASK))->getElement(i, j, 0))
			{
				for (int k = 0; k < 3; ++k)
				{
					point(k) = std::static_pointer_cast<GridFrame>(getFrame(FrameID::POINTCLOUD_GRID))->getElement(i, j, k);
					if (m_rgbMappable)
					{
						color(k) = std::static_pointer_cast<RGBFrame>(getFrame(FrameID::RGB_IMAGE))->getElement(i, j, k);
					}
				}
				pcd.points_.push_back(point);
				if (m_rgbMappable)
				{
					pcd.colors_.push_back(color);
				}
			}
		}
	}
}