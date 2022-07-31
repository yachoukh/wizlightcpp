#include <iostream>
#include <map>
#include "udp.h"

class Bulb
{

public:
    Bulb(const std::string&, const int&);
    ~Bulb();
    std::string getStatus();
    std::string getDeviceInfo();
    std::string getWifiConfig();
    std::string getSystemConfig();
    std::string getUserConfig();
    std::string reboot();

    std::string toggleLight(bool state);


private:
    std::string m_deviceIp;
    u_int16_t m_port;
    std::map<std::string, std::string> m_paramMap;

    UDPSocket m_sock;
};