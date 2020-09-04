/*
 * PointCloudVisualization.cpp
 *
 *  Created on: Sep 11, 2014
 *      Author: Péter Fankhauser
 *   Institute: ETH Zurich, ANYbotics
 */

#include "grid_map_visualization/visualizations/PointCloudVisualization.hpp"
#include <grid_map_ros/GridMapRosConverter.hpp>

#include <string>

namespace grid_map_visualization
{

PointCloudVisualization::PointCloudVisualization(const std::string & name)
: VisualizationBase(name)
{
}

PointCloudVisualization::~PointCloudVisualization()
{
}

bool PointCloudVisualization::readParameters()
{
  this->declare_parameter(
    std::string(this->get_name()) + ".params.layer",
    std::string("elevation"));
  if (!this->get_parameter(std::string(this->get_name()) + ".params.layer", layer_)) {
    RCLCPP_ERROR(
      this->get_logger(),
      "PointCloudVisualization with name '%s' did not find a 'layer' parameter.",
      this->get_name());
    return false;
  }
  return true;
}

bool PointCloudVisualization::initialize()
{
  publisher_ = this->create_publisher<sensor_msgs::msg::PointCloud2>(
    this->get_name(),
    rclcpp::QoS(1).transient_local());
  return true;
}

bool PointCloudVisualization::visualize(const grid_map::GridMap & map)
{
  if (!isActive()) {return false;}
  if (!map.exists(layer_)) {
    RCLCPP_WARN_STREAM(
      this->get_logger(),
      "PointCloudVisualization::visualize: No grid map layer with name '" << layer_ <<
        "' found.");
    return false;
  }
  sensor_msgs::msg::PointCloud2 pointCloud;
  grid_map::GridMapRosConverter::toPointCloud(map, layer_, pointCloud);
  publisher_->publish(pointCloud);
  return true;
}

}  // namespace grid_map_visualization
