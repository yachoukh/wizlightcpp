#include <iostream>
#include <map>
#include "udp.h"

class Bulb
{

public:
    Bulb(const std::string&, const int&);
    ~Bulb();
    std::string setState(std::string, std::string);
    std::string getState(std::string);

private:
    std::string m_deviceIp;
    u_int16_t m_port;
    std::map<std::string, std::string> m_methodMap;
    std::map<std::string, std::string> m_paramMap;

    UDPSocket m_sock;
};