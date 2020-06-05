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
    FCT_TEST_BGN(Reads the UNIO device properly on begin) {
        UNIO unio = UNIO();
        uint8_t value, expect;
        uint16_t index;
        for (index = 0; index < EEPROM_SIZE; index++) {
            unio.set(index, index & 0xFF);
        }
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
        UNIO unio = UNIO();
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
        UNIO unio = UNIO();
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
        UNIO unio = UNIO();
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
        UNIO unio = UNIO();
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
        UNIO unio = UNIO();
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
        UNIO unio = UNIO();
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
        UNIO unio = UNIO();
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
        UNIO unio = UNIO();
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
}
FCTMF_FIXTURE_SUITE_END();
