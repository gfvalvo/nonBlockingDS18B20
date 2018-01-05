# Non-Blocking Temperature Sensor Library

Wrapper library for the "Arduino-Temperature-Control-Library":
https://github.com/milesburton/Arduino-Temperature-Control-Library

* Catalogs and handles multiple DS18B20-type devices on the OneWire bus.

* Provides a non-blocking interface so sketch can perform other functions while temperature sensors
  are doing measurement and conversion.
  
* See https://github.com/milesburton/Arduino-Temperature-Control-Library/blob/master/README.md for further details.

### Prerequisites

The Arduino-Temperature-Control-Library must be installed.

### Example Sketch

The provided example will locate all DS18B20-type devices on the OneWire bus and prepare them for operation.
It will then initiate a temperature measurement on all devices once per second. While the conversion is in process,
the main loop() function will continue to execute (non-blocking) while polling for completion. Once finished, the
measured temperatures are read and reported on the Serial Monitor along with conversion time and how many
times the loop() executed while the measurement was in process.

### Methods:

Class Constructor

	nonBlockingDS18B20(OneWire *w)

Arguments:
  w - pointer to an instance of a OneWire bus object
  
Returns:

  N/A
