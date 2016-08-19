// Copyright © 2016 Daniel Porrey. All Rights Reserved.
//
// This file is part of the FanMonitor project.
//
// The FanMonitor project is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// The FanMonitor project is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with the FanMonitor project. If not,
// see http://www.gnu.org/licenses/.
//
#ifndef FAN_MONITOR_H
#define FAN_MONITOR_H

#include <Arduino.h>

#define FAN_TYPE_UNIPOLE  1
#define FAN_TYPE_BIPOLE   2

class FanMonitor
{
  public:
    FanMonitor(uint8_t monitorPin, uint8_t fanType);
    ~FanMonitor();

    void begin();
    uint8_t fanType;
    uint16_t getSpeed();
    uint8_t pulsesPerRotation;
    uint8_t numberOfSamples;
    uint8_t monitorPin;
};
#endif

