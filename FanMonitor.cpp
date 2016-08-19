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
#include "FanMonitor.h"

FanMonitor::FanMonitor(uint8_t monitorPin, uint8_t fanType)
{
  this->monitorPin = monitorPin;
  this->fanType = fanType;
}

FanMonitor::~FanMonitor()
{
}

void FanMonitor::begin()
{
  pinMode(this->monitorPin, INPUT);

  // ***
  // *** The number of pulses each time
  // *** the fan rotates once. The default
  // *** is 2 for most fans.
  // ***
  this->pulsesPerRotation = 2;

  // ***
  // *** Maximum should be 10 otherwise the
  // *** reading will take a while. The length
  // *** of time for a reading is dependant on
  // *** the speed of the fan and the number
  // *** of samples.
  // ***
  this->numberOfSamples = 5;
}

uint16_t FanMonitor::getSpeed()
{
  uint16_t returnValue = 0;

  // ***
  // *** Check one pulse to see if the fan is running. If not, it
  // *** will return 0. There should be a pull-up resistor on the data
  // *** pin which means the value is HIGH when the motor is stopped.
  // ***
  uint32_t timeCheck = pulseIn(this->monitorPin, LOW);

  if (timeCheck != 0)
  {
    uint32_t time = 0;

    // ***
    // *** A typical PC fan generates two pulses for each
    // *** rotation. In this case, the signal spends one
    // *** quater of the time high, then a quarter of the
    // *** time low, then a quarter of the time high and
    // *** finally a quarter of the time low.
    // ***
    for (uint8_t i = 0; i < this->numberOfSamples; i++)
    {
      // ***
      // *** This measures the total length of time the
      // *** signal is LOW.
      // ***
      time += pulseIn(this->monitorPin, LOW);
    }

    if (time > 0)
    {
      // ***
      // *** Calculate the average length of a LOW pulse.  For this
      // *** reason we will measure the LOW pulse length.
      // ***
      uint32_t averageLowPulseLength = time / this->numberOfSamples;

      // ***
      // *** Calculate the average time for a full turn. If there are two pulses per
      // *** rotation (the default), then the average pulse length should be multplied
      // *** by 4 (2 x 2). The library always takes the average pulse length times the number
      // *** of pulses per rotation times 2.
      // ***
      uint32_t averageTimeFullRotation = averageLowPulseLength * this->pulsesPerRotation * 2;

      // ***
      // *** Number of turns per microsecond
      // ***
      float turnsPerMicrosecond = 1.0 / averageTimeFullRotation;

      // ***
      // *** Convert time to seconds (x 1000 to get milliseconds, x 1000
      // *** again to get seconds).
      // ***
      float turnsPerSecond = turnsPerMicrosecond * 1000.0 * 1000.0;

      // ***
      // *** Number of turns per minute
      // ***
      int16_t rpm = turnsPerSecond * 60;

      // ***
      // *** If the hall effect sensor is a bipole type the reading
      // *** will be doubled.
      // ***
      returnValue = rpm / this->fanType;
    }
  }

  return returnValue;
}

