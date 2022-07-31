#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <netinet/in.h>
#include <jansson.h>
#include "udp.h"
#include "log.h"

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
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    if (setsockopt(m_bCastSock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        LOG_E("UDP broadcast socket setsockopt SO_RCVTIMEO error %s", strerror(errno));
        return false;
    }

    LOG_D("UDP socket initialied");
    return true;
}

std::string UDPSocket::sendUDPCommand(const std::string& msg, const std::string& targetIp, 
    const u_int16_t port) {
    
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

    socklen_t len;
    const int MAXLINE = 1024;
    char resp[MAXLINE] = {};
    int n = recvfrom(m_bCastSock, (char *)resp, MAXLINE, MSG_WAITALL, 
        (struct sockaddr *) &ipAddr, 
                &len); 

    if (n < 0) {
        LOG_W("sendUDPCommand response from device timedout");
        return resp;
    }

    resp[n] = '\0'; 
    LOG_D("sendUDPCommand device response: %s", resp);
    return parseResponse(resp);
}


std::string UDPSocket::parseResponse(std::string jsonStr) {

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

    std::string output = json_dumps(root, JSON_COMPACT);
    LOG_D("parseResponse output %s", output.c_str());
	return output;
}