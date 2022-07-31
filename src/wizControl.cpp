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
 * @file wizControl.cpp
 * 
 ***************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
// #include <thread>
#include <string>
#include <algorithm>
#include <cctype>
#include <clocale>
#include "wizControl.h"
#include "log.h"
using namespace std;

const std::string UDP_WIZ_CONTROL_BULB_IP       = "192.168.1.152";
const int UDP_WIZ_BROADCAST_BULB_PORT           = 38899;

WizControl& WizControl::getInstance() {
    static WizControl instance;
    return instance;
}

WizControl::WizControl() 
    :   m_bCastSock(-1),
        m_bulb(UDP_WIZ_CONTROL_BULB_IP, UDP_WIZ_BROADCAST_BULB_PORT)
{
    g_cmdMap = {
        {"on", on},
        {"off", off},
        {"status", status},
        {"reboot", reboot},
        {"deviceinfo", deviceinfo},
        {"wificonfig", wificonfig},
        {"userconfig", userconfig},
        {"systemconfig", systemconfig}
    };
}

WizControl::~WizControl() {
}

bool WizControl::isCmdSupported(std::string cmd) {
    auto it = g_cmdMap.find(cmd);

    if (it != g_cmdMap.end())
        return true;
    else
        return false;
}

std::string WizControl::performWizRequest(const std::string& cmd)
{
    std::string ret;
    auto eCmd = g_cmdMap.at(cmd);

    switch (eCmd) {
        case on:
            ret = m_bulb.toggleLight(true);
        break;

        case off:
            ret = m_bulb.toggleLight(false);
        break;

        case status:
            ret = m_bulb.getStatus();
        break;

        case reboot:
            ret = m_bulb.reboot();
        break;

        case deviceinfo:
            ret = m_bulb.getDeviceInfo();
        break;

        case userconfig:
            ret = m_bulb.getUserConfig();
        break;

        case wificonfig:
            ret = m_bulb.getWifiConfig();
        break;

        case systemconfig:
            ret = m_bulb.getSystemConfig();
        break;

        default:
        break;
    }
    LOG_I(ret.c_str());
    return ret; 
}

void printUsage()
{
    std::cout << "\n"
                "Usage: wizcontrol [Options] [Commands] [Args]\n"
                "Options:\n"
                "  --help       Show the usage menu of this app\n"
                << std::endl;

    std::cout << "Commands:\n"
                "  off              Turn Off Wiz bulb\n"
                "  on               Turn On Wiz bulb\n"
                "  status           Get the current status of Wiz bulb\n"
                "  reboot           Reboot Wiz bulb\n"
                "  deviceinfo       Get the device information\n"
                "  wificonfig       Get the Wifi Configuration\n"
                "  userconfig       Get the User Configuration\n"
                "  systemconfig     Get the System Configuration\n"
                << std::endl;
}

void printArgsUsage(const std::string& cmd) 
{
    std::cout << "Missed required option: [Args] ipaddress\n"
                 << std::endl;
}

int main(int argc, char *argv[])
{
    WizControl& wiz = WizControl::getInstance();
    if (argc == 1 || !wiz.isCmdSupported(argv[1])) {
		printUsage();
		return 1;
	}

    if (argc == 2) {
        printArgsUsage(argv[1]);
        return 1;
    }

    wiz.performWizRequest(argv[1]);
}   
