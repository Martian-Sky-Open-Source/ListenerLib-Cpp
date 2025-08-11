#ifndef CAMPARAMETERS_HPP
#define CAMPARAMETERS_HPP

#include <vector>
#include <memory>

#include <Eigen/Dense>

/*!
 * @brief Container for a set of camera parameters comprising an intrinsic matrix and distortion coefficients.
 */
struct CamParameters
{
	std::shared_ptr<Eigen::Matrix3f> m_intrinsicPtr;
	std::shared_ptr<std::vector<float>> m_distortionPtr;

	/*!
	 * @brief Default constructor method that sets intrinsic matrix to identity and distortion array to vector of 1.0f
	 */
	CamParameters()
		: m_intrinsicPtr(std::make_shared<Eigen::Matrix3f>(Eigen::Matrix3f::Identity())),
		  m_distortionPtr(std::make_shared<std::vector<float>>(5, 1.0f))
	{
	}

	/*!
	 * @brief Constructor method to set intrinsic matrix and distortion coefficients.
	 *
	 * @param p_intrinsic Pointer to 3x3 intrinsic matrix of a sensor
	 * @param p_distortion Pointer to 5 element vector of a sensor's distortion coefficients
	 */
	CamParameters(std::shared_ptr<Eigen::Matrix3f> p_intrinsic, std::shared_ptr<std::vector<float>> p_distortion)
		: m_intrinsicPtr(p_intrinsic), m_distortionPtr(p_distortion)
	{
	}
};

#endif // CAMPARAMETERS_HPP