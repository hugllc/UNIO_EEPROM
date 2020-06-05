/*
  EEPROM.cpp - esp8266 EEPROM emulation

  Copyright (c) 2014 Ivan Grokhotkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "Arduino.h"
#include "UNIO_EEPROM.h"

UNIOEEPROMClass::UNIOEEPROMClass(uint16_t size)
 : _size(size), _dirty(false)
{
    _buffer = new uint8_t(size);
}

UNIOEEPROMClass::~UNIOEEPROMClass(void)
{
    delete _buffer;
}

void UNIOEEPROMClass::begin(void) {
}

void UNIOEEPROMClass::end() {
    delete _buffer;
}


uint8_t UNIOEEPROMClass::read(int address) {
    if (address < 0 || (size_t)address >= _size)
        return 0;
    if(!_buffer)
        return 0;

    return _buffer[address];
}

void UNIOEEPROMClass::write(int address, uint8_t value) {
    if (address < 0 || (size_t)address >= _size)
        return;
    if(!_buffer)
        return;

    // Optimise _dirty. Only flagged if data written is different.
    uint8_t* data = &_buffer[address];
    if (*data != value)
    {
        *data = value;
        _dirty = true;
    }
}

bool UNIOEEPROMClass::commit() {
    bool ret = false;
    if (!_size)
        return false;
    if(!_dirty)
        return true;
    if(!_buffer)
        return false;

    noInterrupts();

    interrupts();

    return ret;
}

uint8_t * UNIOEEPROMClass::getDataPtr() {
    _dirty = true;
    return &_buffer[0];
}
