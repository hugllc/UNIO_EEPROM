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
    FCT_TEST_BGN(MIN_NETWORK_ID can not be zero) {
        //fct_xchk(MIN_NETWORK_ID != 0, "MIN_NETWORK_ID can not be zero");
    }
    FCT_TEST_END()
}
FCTMF_FIXTURE_SUITE_END();