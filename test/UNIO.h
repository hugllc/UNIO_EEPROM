/* Copyright (C) 2011 by Stephen Early <steve@greenend.org.uk>

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

/**
 * This is a mock that stores everything in memory and tries to mock the
 * behavior of the real UNIO.  This is for testing.
 */

#ifndef _UNIO_LIB_H
#define _UNIO_LIB_H

#include <cstring>
#include <cstdint>
#include <cstdio>

class UNIO {
    private:
    uint8_t *_buffer;
    uint8_t _addr;
    bool _wenable;
    uint8_t _protect;
    int16_t _wtimer;
    uint32_t _size;
    
    public:
    uint32_t writecounter;
    bool enable_write_ret = true;
    bool start_write_ret = true;

    /**
     * @brief Constructor for UNIO library
     * 
     * Takes the address for the UNIO device - 0xa0 
     * and the port pin number for the serial data.  
     * For Arduino UNO or Nano the pin must be a Port D Pin number
     * For Arduino SAMC the pin must be a Port A pin number
     */
    UNIO(uint8_t address = 0, uint32_t size = 128)
    :_addr(address), _wenable(false), _protect(0), _wtimer(0), _size(size), writecounter(0)
    {
        _buffer = new uint8_t[size];
        // Clear the memory
        clear();
    }

    ~UNIO()
    {
        delete [] _buffer;
    }


    /* All the following calls return true for success and false for
        failure. */

    /* Read from memory into the buffer, starting at 'address' in the
        device, for 'length' uint8_ts.  Note that on failure the buffer may
        still have been overwritten. */
    bool read(uint8_t *buffer, uint16_t address, uint16_t length)
    {
        if ((address + length) <= EEPROM_SIZE) {
            memcpy(buffer, &_buffer[address], length);
            return true;
        }
        return false;
    }

    /* Write data to memory.  The write must not overlap a page
        boundary; pages are 16 uint8_ts long, starting at zero.  Will return
        false if this condition is not met, or if there is a problem
        communicating with the device.  If the write enable bit is not
        set, this call will appear to succeed but will do nothing.
        
        This call returns as soon as the data has been sent to the
        device.  The write proceeds in the background.  You must check
        the status register to find out when the write has completed,
        before setting the write enable bit and writing more data.  Call
        await_write_complete() if you want to block until the write is
        finished. */
    bool start_write(const uint8_t *buffer, uint16_t address, uint16_t length)
    {
        if (start_write_ret == false) {
            return false;
        }
        if ((address + length) <= EEPROM_SIZE) {
            memcpy(&_buffer[address], buffer, length);
            _wtimer = length + 1;
            disable_write();
            writecounter++;
            return true;
        }
        return false;
    }

    /* Set the write enable bit.  This must be done before EVERY write;
        the bit is cleared on a successful write. */
    bool enable_write(void)
    {
        _wenable = enable_write_ret;
        return enable_write_ret;
    }
    
    /* Clear the write enable bit. */
    bool disable_write(void)
    {
        _wenable = false;
        return true;
    }

    /* Read the status register into *status.  The bits in this register are:
        0x01 - write in progress
        0x02 - write enable
        0x04 - block protect 0
        0x08 - block protect 1 */
    bool read_status(uint8_t *status) 
    {
        *status = 0;
        if (_wtimer > 0) {
            _wtimer--;
            *status |= 0x01;
        }
        if (_wenable) {
            *status |= 0x02;
        }
        *status |= (_protect & 0x3) << 2;
        return true;
    }

    /* Write to the status register.  Bits are as shown above; only bits
        BP0 and BP1 may be written.  Values that may be written are:
        0x00 - entire device may be written
        0x04 - upper quarter of device is write-protected
        0x08 - upper half of device is write-protected
        0x0c - whole device is write-protected

        The MAC address chip on the Nanode is shipped with the upper quarter
        of the device write-protected.  If you disable write-protection,
        it is possible to overwrite the MAC address pre-programmed into the
        device; this is stored in the last 6 uint8_ts (at address 0x00fa).
        Be careful!

        The write enable bit must be set before a write to the status
        register will succeed.  The bit will be cleared on a successful
        write.  You must wait for the write in progress bit to be clear
        before continuing (call await_write_complete()).  */
    bool write_status(uint8_t status)
    {
        switch (status) {
            case 0x00:
                _protect = 0;
                break;
            case 0x04:
                _protect = 1;
                break;
            case 0x08:
                _protect = 2;
                break;
            case 0x0C:
                _protect = 3;
                break;
            default:
                return false;
                break;
        }
        return true;
    }
    /* check to see if the write is complete. */
    bool is_writing(void) {
        uint8_t status;
        if (!read_status(&status)) {
            return false;
        }
        return (status & 0x1);
    }

    /* Wait until there is no write operation in progress. */
    bool await_write_complete(void) {
        uint8_t status;
        do {
            if (!read_status(&status)) {
                return false;
            }
        } while (status & 0x1);
        return true;
    }

    /* Write to the device, dealing properly with setting the write
        enable bit, avoiding writing over page boundaries, and waiting
        for the write to complete.  Note that this function may take a
        long time to complete - approximately 5ms per 16 uint8_ts or part
        thereof.  Will NOT alter the write-protect bits, so will not
        write to write-protected parts of the device - although the
        return code will not indicate that this has failed. */
    bool simple_write(const uint8_t *buffer,uint16_t address,uint16_t length)
    {
        if (!enable_write()) {
            return false;
        }
        if (!start_write(buffer, address, length)) {
            return false;
        }
        return await_write_complete();
    }
    bool set(uint16_t addr, uint8_t value)
    {
        if (addr < EEPROM_SIZE) {
            _buffer[addr] = value;
            return true;
        }
        return false;
    }
    uint8_t get(uint16_t addr)
    {
        if (addr < EEPROM_SIZE) {
            return _buffer[addr];
        }
        return 0;
    }
    void incrementPattern(void)
    {
        uint32_t index;
        for (index = 0; index < EEPROM_SIZE; index++) {
            set(index, index & 0xFF);
        }
    }
    void clear(void)
    {
        memset(_buffer, 0xff, EEPROM_SIZE);
    }

};

#endif /* _UNIO_LIB_H */
