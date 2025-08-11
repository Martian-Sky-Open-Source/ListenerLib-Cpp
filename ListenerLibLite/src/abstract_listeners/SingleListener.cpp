#include "abstract_listeners/SingleListener.h"

#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <filesystem>

#include <json.hpp>

#include "listener_utils/general_utils.hpp"
#include "listener_utils/CamParameters.hpp"
#include "sensor_interfaces/SensorInterface.h"
#include "abstract_listeners/GenericListener.h"

void SingleListener::setCamParams(std::shared_ptr<CamParameters> p_cam_params)
{
	m_camParamsPtr = p_cam_params;
	if (m_resizeFactor != 1.0f)
	{
		m_camParamsPtr->m_intrinsicPtr->block<2, 3>(0, 0) *= m_resizeFactor;
	}
}

SingleListener::SingleListener(std::shared_ptr<SensorInterface> p_sensor_interface, const std::string& name, const float resize_factor, const std::string& param_dir)
	: GenericListener((name == "" ? SensorIDUtils::toString(p_sensor_interface->getSensorID()) : name), p_sensor_interface->getFramerate()),
	  m_sensorInterfacePtr(p_sensor_interface), m_resizeFactor(resize_factor)
{
	setCamParams(m_sensorInterfacePtr->getDefaultCamParams());

	if (!param_dir.empty())
	{
		try
		{
			loadParameters(param_dir);
		}
		catch (const std::runtime_error& e)
		{
		}
	}
}

const SensorInterface& SingleListener::getSensorInterface() const
{
	return *m_sensorInterfacePtr;
}

void SingleListener::saveParameters(const std::string& output_dir) const
{
	const std::string parameters_path = output_dir + "/" + m_name + "_parameters.json";
	if (std::filesystem::exists(parameters_path))
	{
		std::cout << "Camera parameters file " + parameters_path + " already exists, overwrite? [y/n]" << std::endl;
		char ans;
		std::cin >> ans;
		std::cin.ignore();
		if (!(ans == 'y' || ans == 'Y'))
		{
			std::cout << "File not overwritten." << std::endl;
			return;
		}
		std::cout << "File overwritten." << std::endl;
	}

	nlohmann::json json_data;
	std::vector intrinsic_vect(3, std::vector(3, 1.0f));
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			intrinsic_vect[i][j] = (*(m_camParamsPtr->m_intrinsicPtr))(i, j);
		}
	}
	json_data["intrinsic"] = intrinsic_vect;
	json_data["distortion"] = *(m_camParamsPtr->m_distortionPtr);

	std::ofstream json_file(parameters_path);
	if (!json_file.is_open())
	{
		std::cerr << "Could not open camera parameters file:" << parameters_path << std::endl;
		throw std::runtime_error("Could not open camera parameters file.");
	}
	json_file << json_data.dump(2);
	json_file.close();
}

void SingleListener::saveExtrinsic(const std::string& output_dir) const
{
	if (m_identityName == "")
	{
		throw std::runtime_error("Sensor does not know the identity sensor's name.");
	}
	const std::string extrinsic_path = output_dir + "/" + m_name + "_" + m_identityName + "_extrinsic.json";
	if (std::filesystem::exists(extrinsic_path))
	{
		std::cout << "Extrinsic matrix file " + extrinsic_path + " already exists, overwrite? [y/n]" << std::endl;
		char ans;
		std::cin >> ans;
		std::cin.ignore();
		if (!(ans == 'y' || ans == 'Y'))
		{
			std::cout << "File not overwritten." << std::endl;
			return;
		}
		else
		{
			std::cout << "File overwritten." << std::endl;
		}
	}

	nlohmann::json json_data;
	std::vector extrinsic_vect(4, std::vector(4, 1.0f));
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			extrinsic_vect[i][j] = (*m_extrinsicPtr)(i, j);
		}
	}
	json_data["extrinsic"] = extrinsic_vect;

	std::ofstream json_file(extrinsic_path);
	if (!json_file.is_open())
	{
		std::cerr << "Could not open extrinsic matrix file:" << extrinsic_path << std::endl;
		throw std::runtime_error("Could not open extrinsic matrix file.");
	}
	json_file << json_data.dump(2);
	json_file.close();
}

void SingleListener::loadParameters(const std::string& load_dir)
{
	const std::string parameters_path = load_dir + "/" + m_name + "_parameters.json";
	nlohmann::json json_data;
	try
	{
		listener_utils::loadJson(parameters_path, json_data);
	}
	catch (const std::runtime_error& e)
	{
		throw;
	}
	auto p_intrinsic = std::make_shared<Eigen::Matrix3f>();
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			(*p_intrinsic)(i, j) = json_data["intrinsic"][i][j];
		}
	}
	auto p_distortion = std::make_shared<std::vector<float>>(5, 0.0f);
	for (int i = 0; i < 5; ++i)
	{
		(*p_distortion)[i] = json_data["distortion"][i];
	}
	setCamParams(std::make_shared<CamParameters>(p_intrinsic, p_distortion));
}

void SingleListener::loadExtrinsic(const std::string& load_dir)
{
	const std::string extrinsic_path = load_dir + "/" + m_name + "_" + m_identityName + "_extrinsic.json";
	nlohmann::json json_data;
	try
	{
		listener_utils::loadJson(extrinsic_path, json_data);
	}
	catch (const std::runtime_error& e)
	{
		throw;
	}
	const auto p_extrinsic = std::make_shared<Eigen::Matrix4f>();
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			(*p_extrinsic)(i, j) = json_data["extrinsic"][i][j];
		}
	}
	m_extrinsicPtr = p_extrinsic;
}

void SingleListener::onNewData(std::any& data)
{
}