/**
 * @file       test/suite/Core/mmu/test_mmu.c
 * @author     Scott L. Price <prices@hugllc.com>
 * @copyright  © 2016 Hunt Utilities Group, LLC
 * @brief   The test file for mmu.c
 * @details
 *
 *
 */
/*
 *
 */
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <cmath>
#include "main.h"

FCTMF_FIXTURE_SUITE_BGN(test_unio_eeprom)
{
    /**
    * @brief This sets up this suite
    *
    * @return 0 success, otherwise failure
    */
    FCT_SETUP_BGN() {
        TestInit();
    }
    FCT_SETUP_END();
    /**
    * @brief This tears down this suite
    *
    * @return 0 success, otherwise failure
    */
    FCT_TEARDOWN_BGN() {
    } FCT_TEARDOWN_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(size() is accurate) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint16_t value;
        uint16_t expect = EEPROM_SIZE;
        unio.incrementPattern();
        UNIOEEPROMClass EEPROM(&unio, expect);
        value = EEPROM.size();
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(blockSize() is accurate) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint16_t value;
        uint16_t expect = 8;
        unio.incrementPattern();
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, expect);
        value = EEPROM.blockSize();
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(blockSize() defaults to 0) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint16_t value;
        uint16_t expect = 0;
        unio.incrementPattern();
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        value = EEPROM.blockSize();
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(blockSize() limited to EEPROM size) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint16_t value;
        uint16_t expect = EEPROM_SIZE;
        unio.incrementPattern();
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, EEPROM_SIZE + 10);
        value = EEPROM.blockSize();
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(Reads the UNIO device properly on begin) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t value, expect;
        uint16_t index;
        unio.incrementPattern();
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        for (index = 0; index < EEPROM_SIZE; index++) {
            value = EEPROM.read(index);
            expect = index & 0xFF;
            fct_xchk(value == expect, "Address: %u Expected %u got %u", index, expect, value);
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(Writes data to UNIO device on object descruction) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t value, expect;
        uint16_t index;
        UNIOEEPROMClass *EEPROM = new UNIOEEPROMClass(&unio, EEPROM_SIZE);
        EEPROM->begin();
        for (index = 0; index < EEPROM_SIZE; index++) {
            EEPROM->write(index, index & 0xFF);
        }
        delete EEPROM;
        for (index = 0; index < EEPROM_SIZE; index++) {
            value = unio.get(index);
            expect = index & 0xFF;
            fct_xchk(value == expect, "Address: %u Expected %u got %u", index, expect, value);
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(get returns properly) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t value;
        int32_t expect = -1;
        int32_t have = 1234;
        int16_t addr = 34;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        value = EEPROM.get(addr, have);
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(get what it was given with a negative address) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t value;
        int32_t expect = 682024;
        int16_t addr = -1;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        value = EEPROM.get(addr, expect);
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(get what it was given with an out of range address) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t value;
        int32_t expect = 682024;
        int16_t addr = EEPROM_SIZE + 1;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        value = EEPROM.get(addr, expect);
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()

    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(put returns what it is given with an out of range address) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t value;
        int32_t expect = -4135690;
        int32_t have = expect;
        int16_t addr = EEPROM_SIZE + 1;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        value = EEPROM.put(addr, have);
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(put returns what it is given with a negative address) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t value;
        int32_t expect = -4135690;
        int32_t have = expect;
        int16_t addr = -1;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        value = EEPROM.put(addr, have);
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()

    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(put returns what it is given) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t value;
        int32_t expect = -4135690;
        int32_t have = expect;
        int16_t addr = 34;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        value = EEPROM.put(addr, have);
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()

    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(get and put work together) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t value;
        int32_t expect = -4135690;
        int16_t addr = 34;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        EEPROM.put(addr, expect);
        value = EEPROM.get(addr, value);
        fct_xchk(value == expect, "Expected %u got %u", expect, value);
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(write() does not crash when given a negative address) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t expect = 682024;
        int16_t addr = -1000;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        EEPROM.write(addr, expect);
        // This will segfault if it fails
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(write() does not crash when given an address out of range) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int32_t expect = 682024;
        int16_t addr = EEPROM_SIZE * 5;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        EEPROM.write(addr, expect);
        // This will segfault if it fails
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(read() does not crash when given a negative address) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int16_t addr = -1000;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        EEPROM.read(addr);
        // This will segfault if it fails
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(read() does not crash when given an address out of range) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        int16_t addr = EEPROM_SIZE * 5;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE);
        EEPROM.begin();
        EEPROM.read(addr);
        // This will segfault if it fails
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(commit() when size is 0) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        UNIOEEPROMClass EEPROM(&unio, 0);
        EEPROM.begin();
        EEPROM.commit();
        // This will segfault if it fails
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(read() when size is 0) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint16_t address = 0;
        UNIOEEPROMClass EEPROM(&unio, 0);
        EEPROM.begin();
        EEPROM.read(address);
        // This will segfault if it fails
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(write() when size is 0) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint16_t address = 0;
        uint8_t value = 15;
        UNIOEEPROMClass EEPROM(&unio, 0);
        EEPROM.begin();
        EEPROM.write(address, value);
        // This will segfault if it fails
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(writeBlock() returns false when block size is 0) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 0;
        uint16_t block = 0;
        uint8_t buffer[8] = { 0 };
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.writeBlock(block, buffer);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(writeBlock() returns false when buffer is NULL) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 8;
        uint16_t block = 0;
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.writeBlock(block, NULL);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(writeBlock() returns false when block is too big) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 8;
        uint16_t block = EEPROM_SIZE;
        uint8_t buffer[blocksize] = { 0 };
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.writeBlock(block, buffer);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(writeBlock() writes a block (block 8)) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t value;
        uint8_t blocksize = 8;
        uint8_t buffer[blocksize] = { 0xFD, 0xFC, 0xFB, 0xFA, 0xF9, 0xF8, 0xF7, 0xF6 };
        uint16_t index;
        uint16_t block = 8;
        uint16_t address = block * blocksize;
        bool ret;
        bool retExpect = true;
        uint8_t expect;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.writeBlock(block, buffer);
        EEPROM.commit();
        fct_xchk(ret == retExpect, "Expected %s got %s", retExpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        for (index = 0; index < sizeof(buffer); index++) {
            value = unio.get(address + index);
            expect = buffer[index];
            fct_xchk(value == expect, "index: %u  Address: %u Expected %u got %u", index, address + index, expect, value);
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(readBlock() returns false when block size is 0) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 0;
        uint16_t block = 0;
        uint8_t buffer[8] = { 0 };
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.readBlock(block, buffer);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(readBlock() returns false when buffer is NULL) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 8;
        uint16_t block = 0;
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.readBlock(block, NULL);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(readBlock() returns false when block is too big) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 8;
        uint16_t block = EEPROM_SIZE;
        uint8_t buffer[blocksize] = { 0 };
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.readBlock(block, buffer);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(readBlock() reads a block (block 8)) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        unio.incrementPattern();
        uint8_t value;
        uint8_t blocksize = 8;
        uint16_t block = 8;
        uint16_t address = block * blocksize;
        uint8_t buffer[blocksize];
        uint16_t index;
        bool ret;
        bool retExpect = true;
        uint8_t expect;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.readBlock(block, buffer);
        EEPROM.commit();
        fct_xchk(ret == retExpect, "Expected %s got %s", retExpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        for (index = 0; index < sizeof(buffer); index++) {
            value = buffer[index];
            expect = (address + index) & 0xFF;
            fct_xchk(value == expect, "index: %u  Address: %u Expected %u got %u", index, address + index, expect, value);
        }
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(copyBlock() returns false when src block is too big) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 8;
        uint16_t src = EEPROM_SIZE;
        uint16_t dest = 0;
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.copyBlock(dest, src);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(copyBlock() returns false when dest block is too big) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t blocksize = 8;
        uint16_t src = 0;
        uint16_t dest = EEPROM_SIZE;
        bool ret;
        bool expect = false;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.copyBlock(dest, src);
        fct_xchk(ret == expect, "Expected %s got %s", expect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
    }
    FCT_TEST_END()
    /**
     * @brief Test
     *
     * @return void
     */
    FCT_TEST_BGN(copyBlock() writes a block (block 0 -> 8)) {
        UNIO unio = UNIO(0, EEPROM_SIZE);
        uint8_t value;
        uint8_t blocksize = 8;
        uint16_t index;
        uint16_t src = 0;
        uint16_t dest = 8;
        uint16_t srcAddr = src * blocksize;
        uint16_t destAddr = src * blocksize;
        bool ret;
        bool retExpect = true;
        uint8_t expect;
        UNIOEEPROMClass EEPROM(&unio, EEPROM_SIZE, blocksize);
        EEPROM.begin();
        ret = EEPROM.copyBlock(dest, src);
        EEPROM.commit();
        fct_xchk(ret == retExpect, "Expected %s got %s", retExpect ? "TRUE" : "FALSE", ret ? "TRUE" : "FALSE");
        for (index = 0; index < blocksize; index++) {
            value = unio.get(destAddr + index);
            expect = unio.get(srcAddr + index);
            fct_xchk(value == expect, "index: %u  Address: %u Expected %u got %u", index, destAddr + index, expect, value);
        }
    }
    FCT_TEST_END()

}
FCTMF_FIXTURE_SUITE_END();
