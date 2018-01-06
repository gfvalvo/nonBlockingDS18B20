# Non-Blocking Temperature Sensor Library

Wrapper library for the "Arduino-Temperature-Control-Library":
https://github.com/milesburton/Arduino-Temperature-Control-Library

* Catalogs and handles multiple DS18B20-type devices on the OneWire bus.

* Provides a non-blocking interface so sketch can perform other functions while temperature sensors
  are doing measurement and conversion.
  
* See https://github.com/milesburton/Arduino-Temperature-Control-Library/blob/master/README.md for further details.

## Prerequisites

The Arduino-Temperature-Control-Library must be installed.

## Example Sketch

The provided example will locate all DS18B20-type devices on the OneWire bus and prepare them for operation.
It will then initiate a temperature measurement on all devices once per second. While the conversion is in process,
the main loop() function will continue to execute (non-blocking) while polling for completion. Once finished, the
measured temperatures are read and reported on the Serial Monitor along with conversion time and how many
times the loop() function executed while the measurement was in process.

## Methods:

### Class Constructor
	nonBlockingDS18B20(OneWire *w)

Arguments:

	w - pointer to an instance of a OneWire bus object
  
Returns:

	N/A
	
### begin - Search for temp sensors and initialize data structures
	uint8_t begin(uint8_t defaultResolution);
	
Arguments:

	defaultResolution - Default bits resolution for temp sensors: 9, 10, 11, 12
	
Returns:

	Number of temp sensor devices found on OneWire bus (numTempSensor). Even if other OneWire
	devices are found on the bus, The DS18xx-type sensors will be indexed by a tempSensorIndex
	value between 0 and numTempSensor-1.
	
### startConvertion - Start temp measurment processos on all sensors
	bool startConvertion();
	
Arguments:

	None
	
Returns:

	false if measurement already in progress, true otherwise
	
### startConvertion - Start temp measurment processos on one temp sensor
	bool startConvertion(uint8_t tempSensorIndex)
	
Arguments:

	tempSensorIndex - Sensor number: 0 to numTempSensor-1
	
Returns:

	false if measurement already in progress, true otherwise
	
### Get the most recent temperature reading in raw device units:
	int16_t getLatestTempRaw(uint8_t tempSensorIndex)

Arguments:

	tempSensorIndex - Sensor number: 0 to numTempSensor-1
	
Returns:

	Raw temperature reading from selected sensor (1/16 degC steps for DS18B20)
	
### Get the most recent temperature reading in degrees C:
	float getLatestTempC(uint8_t tempSensorIndex)
	
Arguments:

	tempSensorIndex - Sensor number: 0 to numTempSensor-1
	
Returns:

	Temperature reading from selected sensor scaled to degress C
	
### Get the most recent temperature reading in degrees F:
	float getLatestTempC(uint8_t tempSensorIndex)
	
Arguments:

	tempSensorIndex - Sensor number: 0 to numTempSensor-1
	
Returns:

	Temperature reading from selected sensor scaled to degress F
	
### Check if all temperature conversions have completed
	boolean isConversionDone()
	
Arguments:

	None
	
Returns:

	true if measurements have completed, false otherwise
	
### Get OneWire address for a device:
	void getAddressFromTempSensorIndex(DeviceAddress addr, uint8_t tempSensorIndex)
	
Arguments:

	addr - buffer to receive DeviceAddress (see definition of DeviceAddress in DallasTemperature.h)
	tempSensorIndex - Sensor number: 0 to numTempSensor-1
	
Returns:

	void
	
### Get OneWire index for a device:
	uint8_t getOneWireIndexFromTempSensorIndex(uint8_t tempSensorIndex)
	
Arguments:

	tempSensorIndex - Sensor number: 0 to numTempSensor-1
	
Returns:

	The device's OneWire index - it will be different from tempSensorIndex if there are non-DS18xx type devices on the bus.
	
### Set how the end of conversion cycle will be detected:
	void setUseConversionTimer(boolean state)
	
Arguments:

	state - If true, the end of cylce will be determined by time specified in DS18B20 datasheet. If false, the end of cylce will be determined by the OneWire data line going high (not applicable in parasite power mode).
	
Returns:

	void
	
### Retrieve method for determining end of conversion cycle:
	boolean getUseConversionTimer()
	
Returns:

	none

Returns:

	true if timing method	in use, otherwise false.
	

	
## Credits:
This code is simply a wrapper for the well-written Arduino-Temperature-Control-Library. See:
https://github.com/milesburton/Arduino-Temperature-Control-Library/blob/master/README.md#credits
