/**
 * @brief SKPControl plugin
 * @file skp_control.cpp
 * @author Ricardo Marques <ricardo.marques@skypull.com>
 *
 * @addtogroup plugin
 * @{
 */
/*
 * Copyright 2020 Ricardo Marques <ricardo.marques@skypull.com>
 *
 * This file is part of the mavros package and subject to the license terms
 * in the top-level LICENSE file of the mavros repository.
 * https://github.com/mavlink/mavros/tree/master/LICENSE.md
 */

#include <mavros/mavros_plugin.h>

#include <mavros_msgs/SKPControl.h>

namespace mavros
{
namespace extra_plugins
{

    /**
 * @brief SKPControl plugin
 *
 * Sends custom Skypull controls to FCU controller.
 */
    class SKPControlPlugin : public plugin::PluginBase
    {
    public:
        SKPControlPlugin() : PluginBase(),
                             nh("~")
        {
        }

        void initialize(UAS &uas_)
        {
            PluginBase::initialize(uas_);

            skp_control_sub = nh.subscribe("skp_control", 10, &SKPControlPlugin::skp_control_cb, this);
        }

        Subscriptions get_subscriptions()
        {
            return {/* RX disabled */};
        }

    private:
        ros::NodeHandle nh;
        ros::Subscriber skp_control_sub;

        void skp_control_cb(const mavros_msgs::SKPControl::ConstPtr &req)
        {
            mavlink::common::msg::SKP_CONTROL skp_ctrl{};

            skp_ctrl.time_usec = req->header.stamp.toNSec() / 1000;
            skp_ctrl.mode = req->mode;
            skp_ctrl.pitch_command = req->pitch_cmd;
            skp_ctrl.roll_command = req->roll_cmd;
            skp_ctrl.yaw_command = req->yaw_cmd;
            skp_ctrl.flap_command = req->flap_cmd;
            skp_ctrl.generic_command_x = req->gen_cmd_x;
            skp_ctrl.generic_command_y = req->gen_cmd_y;

            UAS_FCU(m_uas)->send_message_ignore_drop(skp_ctrl);
        }
    };
} // namespace extra_plugins
} // namespace mavros

#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(mavros::extra_plugins::SKPControlPlugin, mavros::plugin::PluginBase)