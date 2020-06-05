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

UNIOEEPROMClass::UNIOEEPROMClass(UNIO *unio, uint16_t size)
 : _unio(unio), _size(size), _dirty(false)
{
    if (size > 0) {
        _buffer = new uint8_t[size];
    }
}

UNIOEEPROMClass::~UNIOEEPROMClass()
{
    end();
    delete [] _buffer;
}

void UNIOEEPROMClass::begin(void) {
    // Read out the E2
    if (_size > 0) {
        _unio->read(_buffer, 0, _size);
    }
}

void UNIOEEPROMClass::end(void) {
    // Commit any changes before we end
    commit();
}


uint8_t UNIOEEPROMClass::read(int address) {
    if (!_goodAddress(address)) {
        return 0;
    }
    return _buffer[address];
}

void UNIOEEPROMClass::write(int address, uint8_t value) {
    if (!_goodAddress(address)) {
        return;
    }

    // Optimise _dirty. Only flagged if data written is different.
    uint8_t* data = &_buffer[address];
    if (*data != value)
    {
        *data = value;
        _dirty = true;
    }
}

bool UNIOEEPROMClass::commit(void) {
    bool ret = false;
    if(!_buffer)
        return false;
    if(!_dirty)
        return true;

    _unio->simple_write(_buffer, 0, _size);
    _dirty = false;
    return ret;
}

bool UNIOEEPROMClass::_goodAddress(int address) {
    return !((address < 0) || ((size_t)address >= _size) || !_buffer);
}
