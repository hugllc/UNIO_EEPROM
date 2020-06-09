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

#ifndef PAGE_SIZE
#define PAGE_SIZE 16
#endif

#define DIRTY_BIT(page) ((1 << (page & 0x7)) & 0xFF)
#define DIRTY_BYTE(page) (page >> 3)

class UNIOEEPROMClass {
public:
    UNIOEEPROMClass(UNIO *unio, size_t size, uint8_t blockSize = 0);
    ~UNIOEEPROMClass();

    void begin(void);
    uint8_t read(int address);
    void write(int address, uint8_t val);
    bool commit(void);
    bool flush(void);
    void end(void);

    bool readBlock(int block, uint8_t *buffer);
    bool writeBlock(int block, uint8_t *data);
    bool copyBlock(int dest, int src);

    size_t size() {
        return _size;
    }
    size_t blockSize() {
        return _blockSize;
    }
    template<typename T> 
    T &get(int address, T &t) {
        if (!_goodAddress(address, sizeof(T))) {
        return t;
        }

        memcpy((uint8_t*) &t, _buffer + address, sizeof(T));
        return t;
    }

    template<typename T> 
    const T &put(int address, const T &t) {
        if (!_goodAddress(address, sizeof(T))) {
        return t;
        }
        memcpy(_buffer + address, (const uint8_t*) &t, sizeof(T));
        _setDirty(_addressPage(address));
        return t;
    }

protected:
    UNIO *_unio;
    uint8_t* _buffer = NULL;
    uint8_t* _dirty;
    size_t _size;
    uint8_t _blockSize;
    uint16_t _pages;
    uint8_t _dirtySize;
    uint16_t _writePage;

    bool _goodAddress(int address, size_t size = 0)
    {
        int addr = address + size;
        return !((address < 0) || ((size_t)addr >= _size) || !_buffer);
    }

    int _blockAddress(int block)
    {
        return block * _blockSize;
    }

    int _addressPage(int address)
    {
        return address / PAGE_SIZE;
    }
    int _pageAddress(int page)
    {
        return page * PAGE_SIZE;
    }

    bool _isDirty(uint16_t page)
    {
        uint8_t index = DIRTY_BYTE(page);
        if (index > _dirtySize) {
            return false;
        }
        return _dirty[index] & DIRTY_BIT(page);
    }
    void _setDirty(uint16_t page)
    {
        uint8_t index = DIRTY_BYTE(page);
        if (index < _dirtySize) {
            _dirty[index] |= DIRTY_BIT(page);
        }
    }
    void _clearDirty(uint16_t page)
    {
        uint8_t index = DIRTY_BYTE(page);
        if (index < _dirtySize) {
            _dirty[index] &= DIRTY_BIT(page);
        }
    }

};

#endif // UNIO_EEPROM_H

