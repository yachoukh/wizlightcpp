#include <iostream>
#include <map>
#include "bulb.h"


class WizControl
{
public:

    static WizControl& getInstance();
    void start(const std::string& cmd, std::string arg = "");
    std::string performWizRequest(const std::string& method, const std::string& params);

private:
    WizControl();
    ~WizControl();

    int m_bCastSock;
    Bulb m_bulb;
};
