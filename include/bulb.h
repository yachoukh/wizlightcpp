#pragma once

#include <iostream>
#include <map>
#include "udp.h"

class Bulb
{

public:
    Bulb();
    ~Bulb();

    void setDeviceIP(const std::string& ip);
    std::string getDeviceIp();

    /*Get APIs*/
    std::string getStatus();
    std::string getDeviceInfo();
    std::string getWifiConfig();
    std::string getSystemConfig();
    std::string getUserConfig();

    /*Set APIs*/
    std::string reboot();
    std::string toggleLight(bool state);
    std::string setBrightness(ushort brightness);
    std::string setRGBColor(ushort r, ushort g, ushort b);
    std::string setSpeed(int speed);
    std::string setColorTemp(int temp);
    std::string setScene(ushort scene);

private:
    std::string parseResponse(std::string);
    
    std::string m_devIP;
    u_int16_t m_port;
    std::map<std::string, std::string> m_paramMap;

    UDPSocket m_sock;
};
