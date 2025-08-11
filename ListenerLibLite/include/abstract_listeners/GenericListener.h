#ifndef GENERICLISTENER_H
#define GENERICLISTENER_H

class CompositeFrame;

#include <string>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <memory>
#include <chrono>
#include <mutex>
#include <condition_variable>

/*!
 * @brief Abstract base class that represents a general template for sensor listeners.
 * 
 * Provides default functionality to track the names of all child instances, add and get frames to and from a queue,
 * print sensor information and status, and provide a sample display of the sensor stream. Subclasses representing
 * actual sensors must implement GenericListener::startStream and GenericListener::stopStream to start and stop sensor-specific
 * streaming and should override dummy behavior of GenericListener::getSensorInfo and GenericListener::getSensorStatus to get
 * sensor-specific info and status.
 */
class GenericListener
{
	static std::set<std::string> activeSensors;

	std::queue<std::shared_ptr<CompositeFrame>> m_queue;
	std::chrono::milliseconds m_timeoutDuration{5000};

	bool m_newFrame = false;
	std::mutex m_queueMutex;
	std::condition_variable m_queueEvent;

protected:

	const std::string m_name;
	const int m_framerate;

	bool m_isStreaming = false;
	
	/*!
	 * @brief Protected method used by an implemented subclasses to add the given CompositeFrame object to its frame queue.
	 * 
	 * @param p_composite_frame Pointer to an object containing individual data frames returned by the sensor in standardized format
	 */
	void addToQueue(std::shared_ptr<CompositeFrame> p_composite_frame);

public:

	/*!
	 * @brief Constructor method that adds sensor's name to the static list of created sensor names.
	 * 
	 * @param name Unique identifier for the implemented sensor. Will be tracked by an internal static list
	 * @param framerate Nominal framerate for the implemented sensor
	 */
	GenericListener(const std::string& name, int framerate);

	/*!
	 * @brief Destructor method that removes sensor's name from the static list of created sensor names.
	 */
	virtual ~GenericListener();

	/*!
	 * @brief Getter for unique sensor identifier, which is used to find external files related to the sensor.
	 * 
	 * @return Name of sensor, acting as unique identifier
	 */
	const std::string& getName() const;

	/*!
	 * @brief Getter for nominal sensor framerate, which can be displayed alongside an actual, calculated framerate.
	 * 
	 * @return Nominal sensor framerate
	 */
	const int& getFramerate() const;

	/*!
	 * @brief Getter for boolean informing whether sensor is actively streaming.
	 * 
	 * @return True if listener is actively streaming, False if not
	 */
	const bool& getIsStreaming() const;

	/*!
	 * @brief Getter for frame queue timeout duration in milliseconds.
	 * 
	 * @return Current queue timeout duration
	 */
	const std::chrono::milliseconds& getTimeoutDuration() const;

	/*!
	 * @brief Setter for frame queue timeout duration in milliseconds.
	 * 
	 * @param timeout_duration New queue timeout duration to set
	 */
	void setTimeoutDuration(const std::chrono::milliseconds& timeout_duration);

	/*!
	 * @brief Abstract method that subclasses must implement with sensor-specific API calls to start the sensor stream.
	 * 
	 * Must set the protected GenericListener::m_isStreaming attribute to 'true'.
	 */
	virtual void startStream() = 0;

	/*!
	 * @brief Abstract method that subclasses must implement with sensor-specific API calls to stop the sensor stream.
	 * 
	 * Must set the protected GenericListener::m_isStreaming attribute to 'false'.
	 */
	virtual void stopStream() = 0;

	/*!
	 * @brief Abstract method that writes any sensor intrinsic matrix and distortion array data to file in JSON format.
	 * 
	 * @param output_dir Path to directory where intrinsic matrix and distortion coefficient data will be saved
	 */
	virtual void saveParameters(const std::string& output_dir) const = 0;

	/*!
	 * @brief Abstract method that writes any sensor extrinsic matrix data to file in JSON format.
	 * 
	 * @param output_dir Path to directory where extrinsic matrix data will be saved
	 */
	virtual void saveExtrinsic(const std::string& output_dir) const = 0;

	/*!
	 * @brief Returns a map containing information about the specific sensor's model, firmware, etc.
	 * 
	 * @return Pointer to a map with string keys and values corresponding to information fields and messages respectively
	 */
	virtual std::unique_ptr<std::map<std::string, std::string>> getSensorInfo() const;

	/*!
	 * @brief Returns a map containing the specific sensor's current settings or other reported metrics.
	 * 
	 * @return Pointer to a map with string keys and values corresponding to status fields and messages respectively
	 */
	virtual std::unique_ptr<std::map<std::string, std::vector<std::string>>> getSensorStatus() const;

	/*!
	 * @brief Prints specific sensor info and status returned by GenericListener::getSensorInfo and GenericListener::getSensorStatus.
	 */
	void printInfoAndStatus() const;

	/*!
	 * @brief Gets the oldest frame in the queue.
	 *
	 * Can be used to get each frame in the queue sequentially without skipping.
	 * 
	 * @return Pointer to an object containing individual data frames returned by the sensor in standardized format
	 */
	std::shared_ptr<CompositeFrame> getNextFrame();

	/*!
	 * @brief Gets the most recent frame in the queue.
	 * 
	 * Clears all older frames and can be used for real-time processing.
	 * 
	 * @return Pointer to an object containing individual data frames returned by the sensor in standardized format
	 */
	std::shared_ptr<CompositeFrame> getLatestFrame();

	/*!
	 * @brief Outputs live view of point cloud, RGB, and temperature frames from the raw sensor stream for test purposes.
	 * 
	 * Uses ListenerDisplayManager to visualize point cloud and RGB frames.
	 */
	void displayStream();

	/*!
	 * @brief Writes live sensor stream to file in provided directory.
	 * 
	 * @param dump_dir Path to directory where frames will be saved
	 */
	void dumpStream(const std::string& dump_dir);
};

#endif // GENERICLISTENER_H