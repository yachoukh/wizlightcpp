#include <iostream>
#include <map>
#include "bulb.h"


class WizControl
{
public:

    static WizControl& getInstance();
    bool isCmdSupported(std::string cmd);
    std::string performWizRequest(const std::string& cmd);

private:

    WizControl();
    ~WizControl();

    enum WIZCMD {
        on, off, status, reboot,
        deviceinfo, wificonfig, userconfig, systemconfig
    };
    int m_bCastSock;
    Bulb m_bulb;
    std::map<std::string, WIZCMD> g_cmdMap;
};
