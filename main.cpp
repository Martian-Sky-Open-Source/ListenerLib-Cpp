#include <memory>
#include <iostream>
#include <filesystem>

#include <opencv2/core.hpp>

#include "ListenerLib.h"

void test_sensor(const std::vector<std::shared_ptr<GenericListener>>& sensor_ptr_vec)
{
    const auto p_display_manager = std::make_unique<ListenerDisplayManager>("Sensor(s) Stream");
    std::vector<std::shared_ptr<CompositeFrame>> comp_ptr_vec;
    for (const auto& p_sensor: sensor_ptr_vec)
    {
        comp_ptr_vec.emplace_back();
        p_sensor->startStream();
    }

    while (true)
    {
        try
        {
            for (int i = 0; i < sensor_ptr_vec.size(); ++i)
            {
                comp_ptr_vec[i] = sensor_ptr_vec[i]->getLatestFrame();
            }
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
            continue;
        }
        int rows = comp_ptr_vec[0]->getFrame(FrameID::RGB_IMAGE)->getRows();
        int cols = 0;
        std::vector<cv::Mat> mat_vec;
        for (const auto& p_comp : comp_ptr_vec)
        {
            const std::shared_ptr<GenericDataFrame> p_rgb = p_comp->getFrame(FrameID::RGB_IMAGE);
            const float factor = p_rgb->getRows() / static_cast<float>(rows);
            p_rgb->resize(factor);
            cols += p_rgb->getCols();
            mat_vec.push_back(*p_rgb->asCvMat());
        }
        auto p_image = std::make_shared<cv::Mat>(rows, cols, CV_8UC3);
        cv::hconcat(mat_vec, *p_image);

        if (!p_display_manager->updateDisplay(comp_ptr_vec[0]->getTimestamp(), p_image))
        {
            break;
        }
    }
}

int main()
{
    const std::vector<std::shared_ptr<GenericListener>> sensor_ptr_vec = {
        std::make_shared<RealsenseListener>(std::make_shared<RealsenseInterface>(), "RealSense"),
        // std::make_shared<FlexxListener>(std::make_shared<FlexxInterface>(7), "FLEXX")
    };

    // std::string data_dir_str = "C:/Users/MarcoStracci/Documents/CppProjects/MSI-COVERT-Cpp/COVERT_flight/lab_data/LRO_7a_1m_GT/COVERT_output";
    // const auto p_sensor = std::make_shared<SavedListener>(std::vector{ FrameID::POINTCLOUD_GRID, FrameID::RGB_IMAGE }, data_dir_str, std::make_shared<FlexxInterface>(7), "FLEXX", 1.0f, true);

    test_sensor(sensor_ptr_vec);

    return 0;
}