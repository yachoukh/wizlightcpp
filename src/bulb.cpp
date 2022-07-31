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
    m_methodMap = {
        {"on", "setState"},
        {"off", "setState"},
        {"status", "getPilot"}
    }; 

    m_paramMap = {
    };
}

Bulb::~Bulb()
{
}

std::string Bulb::setState(std::string cmd, std::string params)
{
    std::string method = m_methodMap.at(cmd);
    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string(method.c_str()));

    if (!params.empty()) {
        json_t *data = json_loads(params.c_str(), 0, NULL);
        json_object_set_new(root, "params", data);
    }

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setState request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}

std::string Bulb::getState(std::string cmd)
{
    std::string method = m_methodMap.at(cmd);

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
   
    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz getState request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_deviceIp, m_port);
}