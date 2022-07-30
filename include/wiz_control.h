#include <iostream>

class WizControl
{
public:
    static WizControl& getInstance();
    void start(const std::string& cmd, std::string arg = "");
    std::string performWizPostRequest(const std::string& method, const std::string& params);
    std::string performWizGetRequest(const std::string& method);

private:
    bool initializeWizSetup();
    std::string sendUDPCommand(const std::string& msg, const std::string& ipAddr, const u_int16_t port);
    std::string getResponse(std::string jsonStr, int statusCode);
    WizControl();
    ~WizControl();

    int m_bCastSock;
};