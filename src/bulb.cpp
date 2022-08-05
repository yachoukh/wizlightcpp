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

const int UDP_WIZ_BROADCAST_BULB_PORT           = 38899;
const std::string ERROR_INVALID_REQUEST         = "Invalid_Request";


    /*
SCENES = {
    1: "Ocean",
    2: "Romance",
    3: "Sunset",
    4: "Party",
    5: "Fireplace",
    6: "Cozy",
    7: "Forest",
    8: "Pastel Colors",
    9: "Wake up",
    10: "Bedtime",
    11: "Warm White",
    12: "Daylight",
    13: "Cool white",
    14: "Night light",
    15: "Focus",
    16: "Relax",
    17: "True colors",
    18: "TV time",
    19: "Plantgrowth",
    20: "Spring",
    21: "Summer",
    22: "Fall",
    23: "Deepdive",
    24: "Jungle",
    25: "Mojito",
    26: "Club",
    27: "Christmas",
    28: "Halloween",
    29: "Candlelight",
    30: "Golden white",
    31: "Pulse",
    32: "Steampunk",
    1000: "Rhythm",
}*/

Bulb::Bulb()
    : m_port(UDP_WIZ_BROADCAST_BULB_PORT)
{
    m_paramMap = {
    };
}

Bulb::~Bulb()
{
}

void Bulb::setDeviceIP(const std::string& ip)
{
    m_devIP = ip;
}

std::string Bulb::getStatus()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getPilot"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz getStatus request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

std::string Bulb::getDeviceInfo()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getDevInfo"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz getDeviceInfo request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

std::string Bulb::getWifiConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getWifiConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

std::string Bulb::getSystemConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getSystemConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

std::string Bulb::getUserConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getUserConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
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
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

std::string Bulb::reboot()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("reboot"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

/*Sample Request
    {"id":1,"method":"setPilot","params":{"r":0,"g":0,"b":255,"dimming":100}}
*/
std::string Bulb::setBrightness(ushort brightness)
{
    if (brightness > 100)
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "dimming", json_integer(brightness));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setBrightness request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

std::string Bulb::setRGBColor(ushort r, ushort g, ushort b)
{
    if (r > 255 || g > 255 || b > 255)
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "r", json_integer(r));
    json_object_set_new(data, "g", json_integer(g));
    json_object_set_new(data, "b", json_integer(b));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setRGBColor request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

std::string Bulb::setSpeed(int speed)
{
    if (speed < 10 || speed > 200) 
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "speed", json_integer(speed));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setColorTemp request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

/*White LED - Set the kelvin color temperature 
min(10000, max(1000, kelvin))*/
std::string Bulb::setColorTemp(int temp)
{
    //temp
    if (temp < 1000 || temp > 2000) //TODO
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "temp", json_integer(temp));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setColorTemp request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}

/* Scenes from 1 to 32 are supported*/
std::string Bulb::setScene(ushort scene) 
{
    if (scene < 1 || scene > 32)
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "sceneId", json_integer(scene));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setScene request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port);
}
