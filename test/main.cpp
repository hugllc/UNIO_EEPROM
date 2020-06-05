/**
 * @file       test/suite/Core/mmu/test.c
 * @author     Scott L. Price <prices@hugllc.com>
 * @copyright  © 2016 Hunt Utilities Group, LLC
 * @brief   The test file for output_fixed.c
 * @details
 *
 *
 */
/*
 *
 */
#include "Arduino.h"
#include "main.h"

FCT_BGN()
{
    FCTMF_SUITE_CALL(test_unio_eeprom);
}
FCT_END();


void TestInit(void)
{
}
