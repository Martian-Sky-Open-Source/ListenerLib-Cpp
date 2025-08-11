#ifndef LISTENERDISPLAYMANAGER_H
#define LISTENERDISPLAYMANAGER_H

#include <string>
#include <memory>
#include <chrono>

#include <opencv2/core.hpp>
#include <open3d/Open3D.h>

/*!
 * @brief General display manager for a sensor's output data stream of image data and/or point clouds.
 * 
 * Used in GenericListener::displayStream to show raw sensor stream. Handles
 * initialization and supervision of an OpenCV and Open3D display window. Useful for any display applications
 * involving the ListenerLib.
 */
class ListenerDisplayManager
{
	const cv::String m_windowName = "Listener Display Window";

	const std::string m_sensorName;
	const int m_nominalFps;
	
	std::chrono::microseconds m_prevTimestamp;

	bool m_displayActive = true;

	std::shared_ptr<cv::Mat> m_dispImageBGRPtr = std::make_shared<cv::Mat>();

	std::unique_ptr<open3d::visualization::Visualizer> m_visPtr;
	bool m_firstFlag = true;
	bool m_raspiFlag = false;

	/*!
	 * @brief Checks for 'esc' key press on OpenCV window or closing of window with the 'X' icon, called in ListenerDisplayManager::updateDisplay.
	 * 
	 * @param key ASCII integer keycode returned by OpenCV 'waitKey()' function
	 */
	void checkActive(int key);

protected:

	std::shared_ptr<cv::Mat> m_dispImagePtr;
	const std::shared_ptr<open3d::geometry::PointCloud> m_pointCloudPtr;

	/*!
	 * @brief Virtual method to be overridden for custom OpenCV window event checking when it is called in ListenerDisplayManager::updateDisplay.
	 *
	 * Event for closing window is unnecessary as this is handled in ListenerDisplayManager::checkActive.
	 *
	 * @param key ASCII integer keycode returned by OpenCV 'waitKey()' function
	 */
	virtual void checkEvents(int key);

public:

	/*!
	 * @brief Constructor method that initializes relevant OpenCV and Open3D displays.
	 * 
	 * @param sensor_name Name of the sensor whose data is displayed, used to title OpenCV window
	 * @param sensor_framerate Nominal framerate of the sensor whose data is displayed, used to title OpenCV window
	 * @param p_tracked_pcd Pointer to Open3D PointCloud object whose data is updated outside of instance
	 */
	explicit ListenerDisplayManager(const std::string& sensor_name, int sensor_framerate = 0, std::shared_ptr<open3d::geometry::PointCloud> p_tracked_pcd = nullptr);

	/*!
	 * @brief Virtual default constructor.
	 */
	virtual ~ListenerDisplayManager();

	/*!
	 * @brief Updates the OpenCV and Open3D display windows with new input image data and internal reference to externally modified Open3D PointCloud.
	 * 
	 * @param timestamp Epoch time in microseconds at which currently displayed frame data was acquired
	 * @param p_disp_image New image data with which to update the OpenCV window's display
	 * @return 'true' if display is still active, 'false' if it has beem terminated by the user
	 */
	bool& updateDisplay(const std::chrono::microseconds& timestamp, std::shared_ptr<cv::Mat> p_disp_image);
};

#endif // LISTENERDISPLAYMANAGER_H