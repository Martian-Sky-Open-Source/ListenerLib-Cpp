#ifndef THREADLISTENER_H
#define THREADLISTENER_H

#include <string>
#include <thread>

#include "abstract_listeners/GenericListener.h"

/*!
 * @brief Abstract subclass representing listeners that must manually create a new thread to wait for sensor stream data.
 * 
 * Inherits GenericListener and provides additional default functionality for such sensors by implementing
 * GenericListener::startStream and GenericListener::stopStream to start and stop their streams.
 * Subclasses representing actual sensors must implement ThreadListener::streamLoop, which will be called in a new thread upon
 * starting the stream and acts as a loop to acquire data from the sensor-specific API.
 */
class ThreadListener : virtual public GenericListener
{
	std::thread m_streamThread;

public:

	/*!
	 * @brief Constructor method, initializes superclass members.
	 * 
	 * @param name Unique identifier for the implemented sensor
	 * @param framerate Nominal framerate for the implemented sensor
	 */
	ThreadListener(const std::string& name, const int framerate);

	/*!
	 * @brief Implemented GenericListener::startStream method that starts the sensor stream thread.
	 *
	 * Passes ThreadListener::streamLoop to a new thread and starts it.
	 */
	void startStream() override;

	/*!
	 * @brief Implemented GenericListener::stopStream method that stops the sensor stream thread.
	 *
	 * Sets GenericListener::m_isStreaming to 'false' and joins the thread.
	 */
	void stopStream() override;

	/*!
	 * @brief Abstract method that subclasses must implement with sensor-specific API calls.
	 *
	 * Initializes the sensor stream and loop to receive its data. Will be called in a new thread by
	 * ThreadListener::startStream. Loop must check GenericListener::m_isStreaming and cease if its value is 'false'.
	 */
	virtual void streamLoop() = 0;
};

#endif // THREADLISTENER_H