#ifndef SINGLELISTENER_H
#define SINGLELISTENER_H

struct CamParameters;
class SensorInterface;

#include <string>
#include <memory>
#include <any>

#include <Eigen/Dense>

#include "abstract_listeners/GenericListener.h"

/*!
 * @brief Abstract subclass that represents listeners comprising a single physical sensor unit.
 *
 * Inherits GenericListener. Provides additional default functionality for
 * such sensors to save and load their camera parameters and relative extrinsic matrix, and perform chessboard
 * calibration. Subclasses representing actual sensors must implement SingleListener::onNewData, which should receive frames
 * from stream for processing and queueing via GenericListener::addToQueue.
 */
class SingleListener : virtual public GenericListener
{
protected:

	std::shared_ptr<SensorInterface> m_sensorInterfacePtr;

	const float m_resizeFactor;
	std::shared_ptr<CamParameters> m_camParamsPtr;

	std::string m_identityName = "";
	std::shared_ptr<Eigen::Matrix4f> m_extrinsicPtr = std::make_shared<Eigen::Matrix4f>(Eigen::Matrix4f::Identity());

	/*!
	 * @brief Sets the instance's camera parameters, adjusting the internal intrinsic matrix by the object's stored resizing factor
	 * 
	 * @param p_cam_params Pointer to an object storing the sensor's intrinsic matrix and distortion coefficients
	 */
	void setCamParams(std::shared_ptr<CamParameters> p_cam_params);

public:

	/*!
	 * @brief Constructor method to set attributes
	 * 
	 * @param p_sensor_interface Object containing information about a connected or hypothetical physical sensor
	 * @param name Unique identifier for the implemented sensor
	 * @param resize_factor A scaling factor the sensor applies to all produced data
	 * @param param_dir Directory path from which camera parameters will be loaded if present
	 */
	SingleListener(std::shared_ptr<SensorInterface> p_sensor_interface, const std::string& name, const float resize_factor, const std::string& param_dir);

	/*!
	 * @brief Getter for sensor interface object.
	 * 
	 * @return Reference to object containing information about a connected or hypothetical physical sensor
	 */
	const SensorInterface& getSensorInterface() const;

	/*!
	 * @brief Implemented GenericListener::saveParameters that writes sensor's intrinsic matrix and distortion array to a JSON file.
	 * 
	 * Prompts user if file exists. File name determined by sensor's name.
	 * 
	 * @param output_dir Path to directory where '{m_name}_parameters.json' file will be saved
	 */
	void saveParameters(const std::string& output_dir) const override;

	/*!
	 * @brief Implemented GenericListener::saveExtrinsic that writes sensor's extrinsic matrix relative to identity sensor to a JSON file.
	 * 
	 * Prompts user if file exists. File name determined by sensor's name and identity sensor's name.
	 * 
	 * @param output_dir Path to directory where '{m_name}_{m_identityName}_extrinsic.json' file will be saved
	 */
	void saveExtrinsic(const std::string& output_dir) const override;

	/*!
	 * @brief Reads sensor's intrinsic matrix and distortion array from a JSON file. File name determined by sensor's name.
	 * 
	 * @param load_dir Path to directory where loaded '{m_name}_parameters.json' file is saved
	 */
	void loadParameters(const std::string& load_dir);

	/*!
	 * @brief Reads sensor's extrinsic matrix relative to identity sensor from a JSON file.
	 *
	 * File name determined by sensor's name and identity sensor's name.
	 * 
	 * @param load_dir Path to directory where loaded '{m_name}_{m_identityName}_extrinsic.json' file is saved
	 */
	void loadExtrinsic(const std::string& load_dir);

	/*!
	* @brief Pure virtual method subclasses must implement with sensor-specific data processing steps to standardize it and add it to queue.
	*
	* Should contain a call to GenericListener::add_to_queue. If implemented by sensor that is also a ThreadListener,
	* method should be called and given data by ThreadListener::stream_loop.
	*
	* @param data Data object returned by a sensor stream according to its API, see GenericType definition for possible types
	*/
	virtual void onNewData(std::any& data);
};

#endif // SINGLELISTENER_H