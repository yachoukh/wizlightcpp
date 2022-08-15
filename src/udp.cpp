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
 * @file udp.cpp
 * 
 ***************************************************************************/

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <jansson.h>
#include "udp.h"
#include "log.h"

const int UDP_REQ_TIMEOUT       = 2;
const int MAXLINE               = 4096;


UDPSocket::UDPSocket()
{
    initializeUDPSocket();
}

UDPSocket::~UDPSocket()
{
}

bool UDPSocket::initializeUDPSocket() {

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
    tv.tv_sec = UDP_REQ_TIMEOUT;
    tv.tv_usec = 0;
    if (setsockopt(m_bCastSock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        LOG_E("UDP broadcast socket setsockopt SO_RCVTIMEO error %s", strerror(errno));
        return false;
    }

    LOG_D("UDP socket initialied");
    return true;
}

std::string UDPSocket::sendUDPCommand(const std::string& msg, const std::string& targetIp, 
    const u_int16_t port, std::string& broadcastIP) {
    
    if (m_bCastSock < 0) {
        initializeUDPSocket();
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

    socklen_t len = sizeof(ipAddr);
    char resp[MAXLINE] = {};
    int n = recvfrom(m_bCastSock, (char *)resp, MAXLINE, MSG_WAITALL, (struct sockaddr *) &ipAddr, &len); 

    if (n < 0) {
        LOG_E("device response timedout error %s", strerror(errno));
        return resp;
    }

    resp[n] = '\0'; 
    LOG_D("sendUDPCommand device response: %s", resp);

    if (!broadcastIP.empty()) {
        char str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(ipAddr.sin_addr), str, INET_ADDRSTRLEN);
        broadcastIP = str;
        LOG_D("sendUDPCommand broadcastIP: %s", broadcastIP.c_str());
    } 
    return resp;
}

