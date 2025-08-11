#include "FlexxListener.h"

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <stdexcept>
#include <iostream>

#include <Eigen/Dense>
#include <unsupported/Eigen/CXX11/Tensor>
#include <royale.hpp>

#include "listener_utils/general_utils.hpp"
#include "listener_utils/CamParameters.hpp"
#include "listener_frames/GrayFrame.h"
#include "listener_frames/RGBFrame.h"
#include "listener_frames/GridFrame.h"
#include "listener_frames/CompositeFrame.h"
#include "sensor_interfaces/FlexxInterface.h"

std::unique_ptr<royale::CameraManager> FlexxListener::managerPtr = std::make_unique<royale::CameraManager>();

royale::Vector<royale::String> FlexxListener::deviceList = managerPtr->getConnectedCameraList();

int FlexxListener::camNumber = 0;

FlexxListener::FlexxListener(std::shared_ptr<FlexxInterface> p_sensor_interface, const std::string& name, const float resize_factor, const std::string& param_dir)
    : GenericListener(name, p_sensor_interface->getFramerate()), SingleListener(p_sensor_interface, name, resize_factor, param_dir)
{
    // Create roypy::ICameraDevice object, connect to physical sensor, set properties, and register listener instance
    if (deviceList.size() == 0)
    {
        std::cerr << "Required FLEXX2 device not found." << std::endl;
        throw std::runtime_error("Required FLEXX2 device not found.");
    }
    m_sensorPtr = managerPtr->createCamera(deviceList[camNumber++]);
    m_sensorPtr->initialize();
    royale::Vector<royale::String> use_cases;
    m_sensorPtr->getUseCases(use_cases);
    m_sensorPtr->setUseCase(use_cases.at(std::static_pointer_cast<FlexxInterface>(m_sensorInterfacePtr)->getMode()));
    m_sensorPtr->registerDataListener(this);

    // Extract camera parameters from royale::ICameraDevice object and set camera parameters attribute
    royale::LensParameters lens_params;
    m_sensorPtr->getLensParameters(lens_params);
    auto p_intrinsics = std::make_shared<Eigen::Matrix3f>();
    (*p_intrinsics) << lens_params.focalLength.first, 0.0f, lens_params.principalPoint.first,
                       0.0f, lens_params.focalLength.second, lens_params.principalPoint.second,
                       0.0f, 0.0f, 1.0;
    auto p_distortion = std::make_shared<std::vector<float>>(std::initializer_list<float>{
        lens_params.distortionRadial[0], lens_params.distortionRadial[1], lens_params.distortionTangential.first, lens_params.distortionTangential.second,lens_params.distortionRadial[2]});
    setCamParams(std::make_shared<CamParameters>(p_intrinsics, p_distortion));
}

void FlexxListener::startStream()
{
    if (m_isStreaming)
    {
        std::cerr << "Sensor " << m_name << " cannot be started while already streaming." << std::endl;
        throw std::runtime_error("Attempted to start an active sensor's stream.");
    }
    m_isStreaming = true;
    m_sensorPtr->startCapture();
}

void FlexxListener::stopStream()
{
    if (m_isStreaming)
    {
        m_isStreaming = false;
        m_sensorPtr->stopCapture();
    }
}

std::unique_ptr<std::map<std::string, std::string>> FlexxListener::getSensorInfo() const
{
    auto info_map = std::make_unique<std::map<std::string, std::string>>();

    royale::String cameraName;
    m_sensorPtr->getCameraName(cameraName);
    const std::string cameraName_std(cameraName.c_str());
    (*info_map)["SENSOR_NAME"] = cameraName_std;

    royale::Vector<royale::Pair<royale::String, royale::String>> camInfo;
    m_sensorPtr->getCameraInfo(camInfo);
    for (const auto& item : camInfo)
    {
        std::string key(item.first.c_str());
        const std::string info(item.second.c_str());
        
        (*info_map)[key] = info;
    }
    return std::move(info_map);
}

std::unique_ptr<std::map<std::string, std::vector<std::string>>> FlexxListener::getSensorStatus() const
{
    auto p_status_map = std::make_unique<std::map<std::string, std::vector<std::string>>>();

    royale::ExposureMode exposure_mode;
    m_sensorPtr->getExposureMode(exposure_mode);
    const std::vector exposure_mode_vect = { std::to_string(static_cast<int>(exposure_mode)) };

    royale::Vector<royale::String> use_cases;
    m_sensorPtr->getUseCases(use_cases);
    std::vector<std::string> operation_modes_vect;
    int counter = 0;
    for (const auto& use_case: use_cases)
    {
        std::string operation_mode(use_case.c_str());
        if (counter == std::static_pointer_cast<FlexxInterface>(m_sensorInterfacePtr)->getMode())
        {
            operation_mode += " <------";
        }
        operation_modes_vect.push_back(operation_mode);

        counter++;
    }

    (*p_status_map)["Exposure Mode"] = exposure_mode_vect;
    (*p_status_map)["Operation Modes"] = operation_modes_vect;

    return std::move(p_status_map);
}

void FlexxListener::onNewData(const royale::DepthData* p_data)
{
    const int width = p_data->width;
    const int height = p_data->height;
    const int num_pixels = width * height;

    auto p_grid = std::make_shared<Eigen::Tensor<float, 3>>(height, width, 3);
    auto p_ir = std::make_shared<Eigen::Tensor<unsigned char, 3>>(height, width, 1);
    auto p_rgb = std::make_shared<Eigen::Tensor<unsigned char, 3>>(height, width, 3);
    Eigen::Tensor<float, 3> ir_float(height, width, 1);
    Eigen::Tensor<float, 3> confidence(height, width, 1);

    int row = 0;
    int col = 0;
    for (size_t pixel = 0; pixel < num_pixels; pixel++)
    {
        if (pixel != 0 && pixel % width == 0)
        {
            col = 0;
            row++;
        }

        (*p_grid)(row, col, 0) = p_data->getX(pixel);
        (*p_grid)(row, col, 1) = p_data->getY(pixel);
        (*p_grid)(row, col, 2) = p_data->getZ(pixel);

        confidence(row, col, 0) = static_cast<float>(static_cast<bool>(p_data->getDepthConfidence(pixel)));

        ir_float(row, col, 0) = static_cast<float>(p_data->amplitudes[pixel]);

        col++;
    }

    *p_grid = *p_grid * confidence.broadcast(Eigen::array<Eigen::Index, 3>{1, 1, 3});

    Eigen::Tensor<float, 0> ir_max = ir_float.maximum();
    if (ir_max(0) == 0)
    {
        ir_max(0) = 1.0f;
    }
    ir_float = ir_float * 255.0f / ir_max(0);

    *p_ir = ir_float.cast<unsigned char>();
    *p_rgb = p_ir->broadcast(Eigen::array<Eigen::Index, 3>{1, 1, 3});

    const auto p_composite_frame = std::make_shared<CompositeFrame>(p_data->timeStamp, m_sensorInterfacePtr->getRGBMappable());
    p_composite_frame->addFrame(FrameID::POINTCLOUD_GRID, std::make_shared<GridFrame>(p_grid, m_camParamsPtr, m_extrinsicPtr));
    p_composite_frame->addFrame(FrameID::GRAYSCALE_IMAGE, std::make_shared<GrayFrame>(p_ir, m_camParamsPtr, m_extrinsicPtr));
    p_composite_frame->addFrame(FrameID::RGB_IMAGE, std::make_shared<RGBFrame>(p_rgb, m_camParamsPtr, m_extrinsicPtr));
    p_composite_frame->resizeAll(m_resizeFactor);
    addToQueue(p_composite_frame);
}