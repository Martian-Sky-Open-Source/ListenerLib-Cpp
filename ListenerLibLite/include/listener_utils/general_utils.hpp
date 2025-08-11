#ifndef LISTENERUTILS_HPP
#define LISTENERUTILS_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <fstream>
#include <filesystem>

#include <json.hpp>

namespace listener_utils
{
    /*!
     * @brief Helper function to load a data dictionary from JSON file path and detect if the path does not exist.
     *
     * @param file_path Path to the JSON file
     * @param json_data JSON data object to be filled
     */
    inline void loadJson(const std::string& file_path, nlohmann::json& json_data)
    {
        if (!std::filesystem::exists(file_path))
        {
            throw std::runtime_error("JSON file '" + file_path + "' not found.");
        }
        std::ifstream json_file(file_path);
        if (!json_file.is_open())
        {
            throw std::runtime_error("Could not open JSON file '" + file_path + "'.");
        }
        json_file >> json_data;
        json_file.close();
    }
}

/*!
 * @brief Enum class providing a consistent way to refer to the types of data frames in a composite returned by a listener.
 *
 * Used to get frame data or objects from a composite frame with CompositeFrame::operator[] and CompositeFrame::getFrame respectively
 * (ex. 'composite_frame[FrameID::POINTCLOUD_GRID]', 'composite_frame.getFrame(FrameID::POINTCLOUD_GRID)').
 */
enum class FrameID
{
    POINTCLOUD_GRID,
    GRAYSCALE_IMAGE,
    RGB_IMAGE,
    TEMPERATURE_GRID,
    POINTCLOUD_MASK
};

namespace FrameIDUtils
{
    /*!
     * @brief Function that maps a FrameID enum to a string representation for I/O purposes.
     *
     * @param frame_id Frame type identifier enum
     * @return String representation of frame type identifier
     */
    inline const std::string& toString(const FrameID frame_id) {
        static const std::unordered_map<FrameID, std::string> frameStringMap = {
            {FrameID::POINTCLOUD_GRID, "ptclGrid"},
            {FrameID::GRAYSCALE_IMAGE, "imgGray"},
            {FrameID::RGB_IMAGE, "imgRGB"},
            {FrameID::TEMPERATURE_GRID, "tempGrid"},
            {FrameID::POINTCLOUD_MASK, "imgDepthMask"}
        };
        if (frameStringMap.find(frame_id) == frameStringMap.end()) {
            throw std::runtime_error("Tried to covert invalid FrameID to string.");
        }
        return frameStringMap.at(frame_id);
    }
}

/*!
 * @brief Enum class providing a consistent way to refer to physical sensor types while using ListenerLib interfaces.
 *
 * Can be used to check the sensor type of : class :`~ListenerLib.abstract_listeners.SingleListener` using the equality
 * operator (ex. 'if sensor.sensor_id == SensorID.CEPTON').
 */
enum class SensorID
{
    FLEXX,
    REALSENSE,
    CEPTON,
    XIMEA,
    BOSON,
    MOVIA,
    DUMMY,
    NONE
};

namespace SensorIDUtils
{
    /*!
     * @brief Function that maps a SensorID enum to a string representation for I/O purposes.
     *
     * @param sensor_id Sensor type identifier enum
     * @return String representation of sensor type identifier
     */
    inline const std::string& toString(const SensorID sensor_id) {
        static const std::unordered_map<SensorID, std::string> sensorStringMap = {
            {SensorID::FLEXX, "FLEXX"},
            {SensorID::REALSENSE, "RealSense"},
            {SensorID::CEPTON, "Cepton"},
            {SensorID::XIMEA, "Ximea"},
            {SensorID::BOSON, "Boson"},
            {SensorID::DUMMY, "Dummy"},
            {SensorID::MOVIA, "Movia"}
        };
        if (sensorStringMap.find(sensor_id) == sensorStringMap.end()) {
            throw std::runtime_error("Tried to covert invalid SensorID to string.");
        }
        return sensorStringMap.at(sensor_id);
    }

    /*!
     * @brief Function that maps a sensor string representation to a SensorID enum for GUI purposes.
     *
     * @param sensor_name String representation of sensor type identifier
     * @return Sensor type identifier enum
     */
    inline const SensorID& toId(const std::string& sensor_name) {
        static const std::unordered_map<std::string, SensorID> stringSensorMap = {
            {"FLEXX", SensorID::FLEXX},
            {"RealSense", SensorID::REALSENSE},
            {"Cepton", SensorID::CEPTON},
            {"Ximea", SensorID::XIMEA},
            {"Boson", SensorID::BOSON},
            {"None", SensorID::NONE},
            {"Movia", SensorID::MOVIA},
        };
        if (stringSensorMap.find(sensor_name) == stringSensorMap.end()) {
            throw std::runtime_error("Tried to covert invalid string to SensorID.");
        }
        return stringSensorMap.at(sensor_name);
    }
}

#endif // LISTENERUTILS_HPP