#include <iostream>
#include <map>
#include "bulb.h"


class WizControl
{
public:
    static WizControl& getInstance();
    bool isCmdSupported(const std::string& cmd);
    void printArgsUsage(const std::string& cmd);
    std::string performWizRequest(const std::string& cmd, const std::string& ip);


private:
    void printSceneUsage();

    WizControl();
    ~WizControl();

    enum WIZCMD {
        on, off, status, reboot,
        getdeviceinfo, getwificonfig, getuserconfig, getsystemconfig,
      	setbrightness, setrgbcolor, setspeed, setcolor, setscene
    };
    int m_bCastSock;
    Bulb m_bulb;
    std::map<std::string, WIZCMD> g_cmdMap;
};
