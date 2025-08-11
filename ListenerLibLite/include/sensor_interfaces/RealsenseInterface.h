#ifndef REALSENSEINTERFACE_H
#define REALSENSEINTERFACE_H

#include "sensor_interfaces/SensorInterface.h"

class RealsenseInterface final : public SensorInterface
{
	const int m_width;
	const int m_height;

public:
	/*!
	 * @brief Constructor method.
	 *
	 * @param width RealSense frame resolution width
	 * @param height RealSense frame resolution height
	 * @param framerate RealSense set framerate
	 */
	explicit RealsenseInterface(int width = 640, int height = 480, int framerate = 15);

	/*!
	 * @brief Getter for RealSense frame resolution width.
	 *
	 * @return RealSense frame resolution width
	 */
	const int& getWidth() const;

	/*!
	 * @brief Getter for RealSense frame resolution height.
	 *
	 * @return RealSense frame resolution height
	 */
	const int& getHeight() const;
};

#endif // REALSENSEINTERFACE_H