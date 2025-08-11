#include "abstract_listeners/ThreadListener.h"

#include <string>
#include <thread>
#include <stdexcept>

#include "abstract_listeners/GenericListener.h"

ThreadListener::ThreadListener(const std::string& name, const int framerate)
	: GenericListener(name, framerate)
{
}

void ThreadListener::startStream()
{
    if (m_isStreaming)
    {
        throw std::runtime_error("Attempted to start an active sensor's stream.");
    }
    m_isStreaming = true;
    m_streamThread = std::thread([this]() { streamLoop(); });
}

void ThreadListener::stopStream()
{
    m_isStreaming = false;
    if (m_streamThread.joinable())
    {
        m_streamThread.join();
    }
}