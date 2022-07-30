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
 * @file wiz_control.cpp
 * 
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <string>
#include <jansson.h>
#include "wiz_control.h"
#include "log.h"
using namespace std;

const std::string UDP_WIZ_CONTROL_BULB_IP       = "192.168.1.152";
const int UDP_WIZ_BROADCAST_BULB_PORT           = 38899;


WizControl& WizControl::getInstance() {
    static WizControl instance;
    return instance;
}

WizControl::WizControl() 
    :   m_bCastSock(-1)
{
    m_cmdmap.insert({"on", "{\"id\":1,\"method\":\"setState\",\"params\":{\"state\":true}}"});
    m_cmdmap.insert({"off", "{\"id\":1,\"method\":\"setState\",\"params\":{\"state\":false}}"});
    m_cmdmap.insert({"status", "{\"method\":\"getPilot\",\"params\":{}}"});
}

WizControl::~WizControl() {
}

void WizControl::start(const std::string& cmd, std::string arg) {
    if (!initializeWizSetup()) {
        LOG_E("Failed to send command to target device ");
        return;
    }

    std::string wizcmd = m_cmdmap.at(cmd);
    performWizRequest(cmd, arg);
}

bool WizControl::initializeWizSetup() {

    if ((m_bCastSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        LOG_E("UDP broadcast socket creation failed");
        return false;
    }

    int broadcastPermission = 1;
    if (setsockopt(m_bCastSock, SOL_SOCKET, SO_BROADCAST, (void *) &broadcastPermission,sizeof(broadcastPermission)) < 0){
        LOG_E("UDP broadcast socket setsockopt SO_BROADCAST error %s", strerror(errno));
        return false;
    }

    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(m_bCastSock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        LOG_E("UDP broadcast socket setsockopt SO_RCVTIMEO error %s", strerror(errno));
        return false;
    }

    LOG_D("UDP socket initialied");
    return true;
}

std::string WizControl::sendUDPCommand(const std::string& msg, const std::string& targetIp, 
    const u_int16_t port) {
    
    if (m_bCastSock < 0) {
        initializeWizSetup();
    }

    LOG_D("sendUDPCommand socket ipAddr %s cmd %s", targetIp.c_str(), msg.c_str());

    struct sockaddr_in ipAddr; 
    memset(&ipAddr, 0, sizeof(ipAddr));   
    ipAddr.sin_family = AF_INET;                 
    ipAddr.sin_addr.s_addr = inet_addr(targetIp.c_str());
    ipAddr.sin_port = htons(port); 
    int msgLen = strlen(msg.c_str());

    if (sendto(m_bCastSock, msg.c_str(), msgLen, 0, (struct sockaddr *)&ipAddr, sizeof(ipAddr)) != msgLen){
        LOG_E("sendUDPCommand sendto error %s", strerror(errno));
        return "";
    }

    socklen_t len;
    const int MAXLINE = 1024;
    char resp[MAXLINE] = {};
    int n = recvfrom(m_bCastSock, (char *)resp, MAXLINE, MSG_WAITALL, 
        (struct sockaddr *) &ipAddr, 
                &len); 

    if (n < 0)
        LOG_W("sendUDPCommand response from device timedout");
    else {
        resp[n] = '\0'; 
        LOG_D("sendUDPCommand device response: %s", resp);
    }
    return resp;
}

std::string WizControl::performWizRequest(const std::string& method, const std::string& params) {

    json_t* root = json_object();
    json_object_set_new(root, "id", json_integer(1));
    json_object_set_new(root, "method", json_string(method.c_str()));
    if (!params.empty()) {
        json_t *data = json_loads(params.c_str(), 0, NULL);
        json_object_set_new(root, "params", data);
    }
    std::string msg = json_dumps(root, JSON_COMPACT);
    LOG_D("Wiz Post request %s to Wiz", msg.c_str());

    std::string resp = sendUDPCommand(msg, UDP_WIZ_CONTROL_BULB_IP, UDP_WIZ_BROADCAST_BULB_PORT);
    return getResponse(resp, 200);
}

// std::string WizControl::performWizGetRequest(const std::string& method) {

//     json_t* root = json_object();
//     json_object_set_new(root, "method", json_string(method.c_str()));
//     std::string msg = json_dumps(root, JSON_COMPACT);
//     LOG_D("Wiz Get request %s", msg.c_str());

//     std::string resp = sendUDPCommand(msg, UDP_WIZ_CONTROL_BULB_IP, UDP_WIZ_BROADCAST_BULB_PORT);
//     return getResponse(resp, 200);
// }

std::string WizControl::getResponse(std::string jsonStr, int statusCode) {

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

    json_t* dataObj = json_object();
    json_t* value;
    for (void *itr = json_object_iter(data); itr != NULL; itr = json_object_iter_next(data, itr)) {
        const char* key = json_object_iter_key(itr);
        value = json_object_iter_value(itr);
        json_object_set(dataObj, key, value);
    }

    json_object_del(dataObj, "method");
    json_object_set_new(root, "response", dataObj);
    json_object_set_new(root, "status", json_string(to_string(statusCode).c_str()));
    json_object_set_new(root, "contentType", json_string("application/json"));

    std::string output = json_dumps(root, JSON_COMPACT);
    LOG_D("getResponse output %s", output.c_str());
	return output;
}

