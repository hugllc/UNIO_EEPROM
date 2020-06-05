# UNIO_EEPROM

## Introduction
This is an Arduino library for working with UNIO EEPROM chips.

## Testing

### Requirements
 * g++
 * gcovr
 * lcov

## Running the suite

Simply run 'make' in the test directory.

```.sh
$ cd test
$ make
```
This will create all of the output in the test/build/ directory.  This includes:

* HTML coverage report (test/build/coverage/index.html)
* junit test result files (test/build/logs/*Result.xml)
* cobertura output file (test/build/logs/cobertura.xml)

## License

This is licensed under the LGPL, as it is a derivative of https://github.com/esp8266/Arduino.
  Which is also licensed under the LGPL.
