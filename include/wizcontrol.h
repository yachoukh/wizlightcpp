#include <iostream>
#include <map>
#include <vector>
#include "bulb.h"


class WizControl
{
public:
    static WizControl& getInstance();
    bool isCmdSupported(const std::string& cmd);
    bool validateArgsUsage(const std::vector<std::string>& args);
    std::string performWizRequest(const std::string& cmd);


private:
    std::string getSceneList();
    bool checkArgOptions(const std::vector<std::string>& args, const std::string& cmd, 
        const std::string& option, std::string& param);
    bool checkArgOptions(const std::vector<std::string>& args, const std::string& cmd, 
        const std::string& option, int& param);

    WizControl();
    ~WizControl();

    enum WIZCMD {
        discover, on, off, status, reboot,
        getdeviceinfo, getwificonfig, getuserconfig, getsystemconfig,
      	setbrightness, setrgbcolor, setspeed, setcolortemp, setscene
    };
    int m_bCastSock;
    Bulb m_bulb;
    std::map<std::string, WIZCMD> g_cmdMap;
};
