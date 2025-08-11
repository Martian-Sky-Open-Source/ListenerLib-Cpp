#include "listener_utils/ListenerDisplayManager.h"

#include <string>
#include <memory>
#include <chrono>
#include <fstream>

#include <Eigen/Dense>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <open3d/Open3D.h>

ListenerDisplayManager::ListenerDisplayManager(const std::string& sensor_name, const int sensor_framerate, const std::shared_ptr<open3d::geometry::PointCloud> p_tracked_pcd)
	: m_sensorName(sensor_name), m_nominalFps(sensor_framerate), m_pointCloudPtr(p_tracked_pcd)
{
	// Check if platform is RaspberryPi, which does not support Open3D visualization
	bool m_raspiFlag = false;
	#if defined(__linux__)
		std::ifstream cpuinfo("/proc/cpuinfo");
		std::string line;
		while (std::getline(cpuinfo, line))
		{
			if (line.find("Raspberry Pi") != std::string::npos)
			{
				m_raspiFlag = true;
			}
		}
	#endif

	// Initialize real-time framerate tracking
	const auto now = std::chrono::system_clock::now();
	m_prevTimestamp = std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch());

	// # Create OpenCV and Open3D windows if applicable
	cv::namedWindow(m_windowName, cv::WINDOW_AUTOSIZE);
	if (m_pointCloudPtr != nullptr && !m_raspiFlag)
	{
		m_visPtr = std::make_unique<open3d::visualization::Visualizer>();
	}
}

ListenerDisplayManager::~ListenerDisplayManager()
{
}


void ListenerDisplayManager::checkActive(const int key)
{
	if (key == 27 || cv::getWindowProperty(m_windowName, cv::WND_PROP_VISIBLE) == 0)
	{
		cv::destroyWindow(m_windowName);
		if (m_visPtr != nullptr)
		{
			m_visPtr->DestroyVisualizerWindow();
		}
		m_displayActive = false;
	}
}

void ListenerDisplayManager::checkEvents(const int key)
{
}

bool& ListenerDisplayManager::updateDisplay(const std::chrono::microseconds& timestamp, std::shared_ptr<cv::Mat> p_disp_image)
{
	// Calculate actual framerate based on timestamps
	float dt =(timestamp - m_prevTimestamp).count() / 1000000.0f;
	if (dt == 0)
	{
		dt = 0.01f;
	}
	int fps = static_cast<int>(1 / dt);
	m_prevTimestamp = timestamp;

	// Update the Open3D display window
	if (m_visPtr != nullptr)
	{
		if ( m_firstFlag && m_pointCloudPtr->points_.size() > 500 )
		{
			m_visPtr->CreateVisualizerWindow("Point Cloud", 300, 300);
			open3d::visualization::ViewControl& m_viewControl = m_visPtr->GetViewControl();
			m_visPtr->AddGeometry(m_pointCloudPtr);
			m_viewControl.SetFront(Eigen::Vector3d(0.0, 0.0, -1.0));
			m_viewControl.SetUp(Eigen::Vector3d(0.0, -1.0, 0.0));
			m_viewControl.SetLookat(Eigen::Vector3d(0.0, 0.0, 0.0));
			m_viewControl.SetZoom(0.1);
			m_firstFlag = false;
		}
		else if (!m_firstFlag)
		{
			m_visPtr->UpdateGeometry(m_pointCloudPtr);
			m_visPtr->PollEvents();
			m_visPtr->UpdateRender();
		}
	}

	// Update calculated framerate in OpenCV window title and show window
	m_dispImagePtr = p_disp_image;
	cv::setWindowTitle(m_windowName,
		"Quit stream: 'esc' | " + m_sensorName + " | Nominal FPS: " + std::to_string(m_nominalFps) +" | Actual FPS: " + std::to_string(fps));
	cv::cvtColor(*m_dispImagePtr, *m_dispImageBGRPtr, cv::COLOR_RGB2BGR);
	cv::imshow(m_windowName, *m_dispImageBGRPtr);

	// Check for key inputs on OpenCV window and check key press events
	const int key = cv::pollKey();
	checkEvents(key);
	checkActive(key);
	return m_displayActive;
}