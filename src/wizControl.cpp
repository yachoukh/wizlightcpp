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
#include <thread>
#include <string>
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
}

WizControl::~WizControl() {
}

void WizControl::start(const std::string& cmd, std::string arg) {
    // performWizRequest(cmd, arg);

}

