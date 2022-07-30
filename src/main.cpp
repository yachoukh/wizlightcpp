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
 * @file main.cpp
 * 
 ***************************************************************************/

#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <clocale>
#include "wiz_control.h"

std::vector<std::string> g_cmd = {"on", "off", "status"};

void printUsage()
{
    std::cout << "\n"
                "Usage: wizcontrol [Options] [Commands] [Args]\n"
                "Options:\n"
                "  --help       Show the usage menu of this app\n"
                << std::endl;

    std::cout << "Commands:\n"
                "  off          Turn Off Wiz bulb\n"
                "  on           Turn On Wiz bulb\n"
                "  status       Get the current status of Wiz bulb\n"
                << std::endl;
}

void printArgsUsage(const std::string& cmd) 
{
    std::cout << "Missed required option: <args>\n"
                 "Try 'wizcontrol " << cmd << " --help' for more information\n"
                 << std::endl;
}

int main(int argc, char *argv[])
{
    if (argc == 1 || (std::find(g_cmd.begin(), g_cmd.end(), argv[1]) == g_cmd.end())) {
		printUsage();
		return 1;
	}

    if (argc == 2) {
        printArgsUsage(argv[1]);
        return 1;
    }

    WizControl& wiz = WizControl::getInstance();
    wiz.start(argv[1], argv[2]);
}   
