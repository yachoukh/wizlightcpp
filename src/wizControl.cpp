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

WizControl& WizControl::getInstance() {
    static WizControl instance;
    return instance;
}

WizControl::WizControl() 
    :   m_bCastSock(-1)
{
    g_cmdMap = {
        {"on", on},
        {"off", off},
        {"status", status},
        {"reboot", reboot},
        {"getdeviceinfo", getdeviceinfo},
        {"getwificonfig", getwificonfig},
        {"getuserconfig", getuserconfig},
        {"getsystemconfig", getsystemconfig},
        {"setbrightness", setbrightness},
        {"setrgbcolor", setrgbcolor},
        {"setspeed", setspeed},
        {"setcolor", setcolor},
        {"setscene", setscene}
    };
}

WizControl::~WizControl() {
}

bool WizControl::isCmdSupported(const std::string& cmd) {
    auto it = g_cmdMap.find(cmd);

    if (it != g_cmdMap.end())
        return true;
    else
        return false;
}

std::string WizControl::performWizRequest(const std::string& cmd, const std::string& ip)
{
    std::string ret;
    auto eCmd = g_cmdMap.at(cmd);

    m_bulb.setDeviceIP(ip);
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

        case getdeviceinfo:
            ret = m_bulb.getDeviceInfo();
        break;

        case getuserconfig:
            ret = m_bulb.getUserConfig();
        break;

        case getwificonfig:
            ret = m_bulb.getWifiConfig();
        break;

        case getsystemconfig:
            ret = m_bulb.getSystemConfig();
        break;

	case setbrightness:
            ushort level;
            cout << "Enter the brightness level[0 to 100]:";
            cin >> level;
            ret = m_bulb.setBrightness(level);
        break;

        case setrgbcolor:
            ushort r, g, b;
            cout << "Enter the Red color range[0 to 255]:";
            cin >> r;
            cout << "Enter the Green color range[0 to 255]:";
            cin >> g;
            cout << "Enter the Blue color range[0 to 255]:";
            cin >> b;
            ret = m_bulb.setRGBColor(r, g, b);
        break;

        case setspeed:
            ushort speed;
            cout << "Enter the speed level[0 to 100]:";
            cin >> speed;
            ret = m_bulb.setSpeed(speed);
        break;

        case setcolor:
            ushort color;
            cout << "Enter the color temperature[0 to 100]:";
            cin >> color;
            ret = m_bulb.setColorTemp(color);
        break;

        case setscene:
            ushort scene;
            printSceneUsage();
            cout << "Enter the scene id[0 to 32]:";
            cin >> scene;
            ret = m_bulb.setScene(scene);
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
                "  --help\t\t\tShow the usage menu of this app\n"
                << std::endl;

    std::cout << "Commands:\n"
                "  off\t\t\t\tTurn Off Wiz bulb\n"
                "  on\t\t\t\tTurn On Wiz bulb\n"
                "  status\t\t\tGet the current status of Wiz bulb\n"
                "  reboot\t\t\tReboot Wiz bulb\n"
                "  getdeviceinfo\t\t\tGet the device information\n"
                "  getwificonfig\t\t\tGet the Wifi Configuration\n"
                "  getuserconfig\t\t\tGet the User Configuration\n"
                "  getsystemconfig\t\tGet the System Configuration\n"
                "  setbrightness\t\t\tSet the brightness in the bulb\n"
                "  setrgbcolor\t\t\tSet the R G B color in the bulb\n"
                "  setspeed\t\t\tSet the Speed of light flickering\n"
		        "  setcolor\t\t\tSet the color temperature in the bulb\n"
                "  setscene\t\t\tSelect the scene id\n"
                << std::endl;
}

void WizControl::printSceneUsage()
{
    std::cout << "Scene:\n"
                "  1\tOcean\n"
                "  2\tRomance\n"
                "  3\tSunset\n"
                "  4\tParty\n"
                "  5\tFireplace\n"
                "  6\tCozy\n"
                "  7\tForest\n"
                "  8\tPastel Colors\n"
                "  9\tWake up\n"
                "  10\tBedtime\n"
                "  11\tWarm White\n"
                "  12\tDaylight\n"
                "  13\tCool white\n"
                "  14\tNight light\n"
                "  15\tFocus\n"
                "  16\tRelax\n"
                "  17\tTrue colors\n"
                "  18\tTV time\n"
                "  19\tPlantgrowth\n"
                "  20\tSpring\n"
                "  21\tSummer\n"
                "  22\tFall\n"
                "  23\tDeepdive\n"
                "  24\tJungle\n"
                "  25\tMojito\n"
                "  26\tClub\n"
                "  27\tChristmas\n"
                "  28\tHalloween\n"
                "  29\tCandlelight\n"
                "  30\tGolden white\n"
                "  31\tPulse\n"
                "  32\tSteampunk\n"
                "  1000\tRhythm\n"
                << std::endl;
}

void WizControl::printArgsUsage(const std::string& cmd) 
{
    auto eCmd = g_cmdMap.at(cmd);
    switch (eCmd)
    {		
	case on:
	case off:
	case status:
	case reboot:
	case getdeviceinfo:
	case getuserconfig:
	case getsystemconfig:
	case setbrightness:
	case setrgbcolor:
	case setspeed:
	case setcolor:
	case setscene:
	{
	    std::cout << "Missed required option: [Args] <ipaddress>\n" << std::endl;
	}
	break;

	/*
	case setbrightness:
	    std::cout << "Missed required option: [Args] <brightness level>\n"
		    	"  Brightness level range [0 to 100]\n"
			<<std:: endl;
	break;
	case setrgbcolor:
	    std::cout << "Missed required option: [Args] <temperature>\n"
                        "  Temperature range []\n"
                        <<std:: endl;
	break;
	case setspeed:
            std::cout << "Missed required option: [Args] <speed>\n"
                        " Speed range []\n"
                        <<std:: endl;
        break;
	case setcolor:
	     std::cout << "Missed required option: [Args] <temperature>\n"
                        "  Temperature range []\n"
                        <<std:: endl;
	break;
	case setscene:
	    std::cout << "Missed required option: [Args] <scene id>\n"
                        "  Choose any from the following scenes:\n"
                        <<std:: endl;
	break;
	*/

	default:
	break;
    }
}

int main(int argc, char *argv[])
{
    WizControl& wiz = WizControl::getInstance();
    if (argc == 1 || !wiz.isCmdSupported(argv[1])) {
		printUsage();
		return 1;
	}

    else if (argc == 2) {
        wiz.printArgsUsage(argv[1]);
        return 1;
    }

    wiz.performWizRequest(argv[1], argv[2]);
}   
