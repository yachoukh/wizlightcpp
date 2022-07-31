/***************************************************************************
 *  Project                WIZLIGHTCPP     
 *
 * Copyright (C) 2022 , Sri Balaji S.
 *
 * This software is licensed as described in the file LICENSE, which
 * you should have received as part of this distribution.
 *
 * You may opt to use, copy, modify, merge, publish, distribute and/or sell
 * copies of the Software, and permit persons to whom the Software is
 * furnished to do so, under the terms of the LICENSE file.
 *
 * This software is distributed on an "AS IS" basis, WITHOUT WARRANTY OF ANY
 * KIND, either express or implied.
 *
 * @file bulb.cpp
 * 
 ***************************************************************************/

#include <jansson.h>
#include "bulb.h"
#include "log.h"

Bulb::Bulb(const std::string& ip, const int& port)
    : m_deviceIp(ip),
      m_port(port)
{
    m_paramMap = {
    };
}

Bulb::~Bulb()
{
}

std::string Bulb::toggleLight(bool state)
{
    // std::string method = m_methodMap.at(cmd);
    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setState"));

    json_t *data = json_object();
    json_object_set_new(data, "state", json_boolean(state));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz toggleLight request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}

std::string Bulb::getStatus()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getPilot"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz getStatus request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}

std::string Bulb::getDeviceInfo()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getDevInfo"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz getDeviceInfo request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}

std::string Bulb::reboot()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("reboot"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}

std::string Bulb::getWifiConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getWifiConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}

std::string Bulb::getSystemConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getSystemConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}

std::string Bulb::getUserConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getUserConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}
