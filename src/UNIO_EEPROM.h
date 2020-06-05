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

#ifndef UNIO_EEPROM_h
#define UNIO_EEPROM_h

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <cstdio>

#include <UNIO.h>

class UNIOEEPROMClass {
public:
  UNIOEEPROMClass(UNIO *unio, uint16_t size);
  ~UNIOEEPROMClass();

  void begin(void);
  uint8_t read(int address);
  void write(int address, uint8_t val);
  bool commit(void);
  void end(void);

  template<typename T> 
  T &get(int address, T &t) {
    if (!_goodAddress(address) || address + sizeof(T) > _size)
      return t;

    memcpy((uint8_t*) &t, _buffer + address, sizeof(T));
    return t;
  }

  template<typename T> 
  const T &put(int address, const T &t) {
    if (!_goodAddress(address) || address + sizeof(T) > _size)
      return t;

    memcpy(_buffer + address, (const uint8_t*) &t, sizeof(T));
    _dirty = true;
    return t;
  }

protected:
  UNIO *_unio;
  uint8_t* _buffer = NULL;
  size_t _size;
  bool _dirty;
  bool _goodAddress(int address);
};

#endif // UNIO_EEPROM_H

