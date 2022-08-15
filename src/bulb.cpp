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
std::string EMPTY_STRING;

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

std::string Bulb::getDeviceIp() 
{
    return m_devIP;
}

std::string Bulb::discover(const std::string& ip)
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getDevInfo"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz discover request %s to Wiz", msg.c_str());
    string devIp = "yes";
    auto resp = m_sock.sendUDPCommand(msg, ip, m_port, devIp);
    return parseResponse(resp, devIp);
}

std::string Bulb::getStatus()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getPilot"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz getStatus request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::getDeviceInfo()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getDevInfo"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz getDeviceInfo request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::getWifiConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getWifiConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    return m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    // return parseResponse(resp); //Currently Wiz light response for Wifi Config unable to parse. Returning actual results
}

std::string Bulb::getSystemConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getSystemConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::getUserConfig()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("getUserConfig"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::toggleLight(bool state)
{
    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setState"));

    json_t *data = json_object();
    json_object_set_new(data, "state", json_boolean(state));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    std::cout << "Turning light " << (state ? "ON" : "OFF") << std::endl;
    LOG_D("Wiz toggleLight request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::reboot()
{
    json_t* root = json_object();
    json_object_set_new(root, "method", json_string("reboot"));

    std::string msg = json_dumps(root, JSON_COMPACT);
    std::cout << "Rebooting..." << std::endl;
    LOG_D("Wiz reboot request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::setBrightness(ushort brightness)
{
    if (brightness < 0 || brightness > 100)
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "dimming", json_integer(brightness));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setBrightness request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
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
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::setSpeed(int speed)
{
    if (speed < 0 || speed > 100) 
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "speed", json_integer(speed));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setSpeed request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::setColorTemp(int temp)
{
    if (temp < 1000 || temp > 8000)
        return ERROR_INVALID_REQUEST;

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string("setPilot"));

    json_t *data = json_object();
    json_object_set_new(data, "temp", json_integer(temp));
    json_object_set_new(root, "params", data);

    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz setColorTemp request %s to Wiz", msg.c_str());
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
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
    auto resp = m_sock.sendUDPCommand(msg, m_devIP, m_port, EMPTY_STRING);
    return parseResponse(resp);
}

std::string Bulb::parseResponse(std::string jsonStr, std::string addlParams) {

    if (jsonStr.empty())
        return jsonStr;
    
    json_t* root = json_object();
    json_error_t error;
    json_t *data = json_loads(jsonStr.c_str(), 0, &error);
    if (!data) {
        LOG_E("JSON error. Parsing error on line %d : %s", error.line, error.text);
        return "";
    }

    if (!json_is_object(data)) {
        LOG_E("JSON error. Parsing error. data is not a object");
        return "";
    }

    json_t* result = json_object_get(data, "result");
    if (!result || !json_is_object(result)) {
	LOG_E("JSON error. Parsing error. result is not a object");
	return "";
    }

    json_t* dataObj = json_object();
    json_t* value;
    for (void *itr = json_object_iter(result); itr != NULL; itr = json_object_iter_next(result, itr)) {
        const char* key = json_object_iter_key(itr);
        value = json_object_iter_value(itr);
        json_object_set(dataObj, key, value);
    }

    json_object_del(dataObj, "method");
    json_object_del(dataObj, "id");
    json_object_del(dataObj, "env");

    if (!addlParams.empty()) 
        json_object_set_new(dataObj, "ip", json_string(addlParams.c_str()));

    json_object_set_new(root, "bulb_response", dataObj);
    std::string output = json_dumps(root, JSON_INDENT(4));
    LOG_D("%s", output.c_str());
	return output;
}
