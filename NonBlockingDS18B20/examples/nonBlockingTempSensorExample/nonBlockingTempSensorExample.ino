#include <Arduino.h>
#include "nonBlockingDS18B20.h"

enum machineStates {
	IDLE, WAITING_FOR_CONVERSION, ABORTED
};

const uint8_t ONE_WIRE_BUS = 4; // Change this to Pin # connected to OneWire bus
const uint8_t RESOLUTION = 9;
const unsigned long maxWaitTime = 5000;
const unsigned long interMeasurementPeriod = 1000;
machineStates currentState;
unsigned long measurementStartTime, measurementStopTime;
unsigned long interMeasurementTimer;

OneWire oneWire(ONE_WIRE_BUS);
nonBlockingDS18B20 tempSensors(&oneWire);

void printAddress(DeviceAddress);

uint8_t numDS18;

void setup() {
	uint8_t index;
	DeviceAddress addr;

	Serial.begin(115200);
	delay(2000);

	// Locate and Enumerate the DS18xx-type Temperature Sensors
	numDS18 = tempSensors.begin(RESOLUTION);
	if (numDS18 == 0) {
		Serial.println(
				F("Did not locate any DS-type Temperature Sensors, aborting"));
		while (1) {
			delay(0);
		}
	}

	// Print what we found
	Serial.print(F("Found "));
	Serial.print(numDS18);
	Serial.print(F(" Temperature Sensors, Parasite Mode "));
	if (tempSensors.isParasitePowerMode()) {
		Serial.println(F("Detected"));
	} else {
		Serial.println(F("Not Detected"));
	}
	for (index = 0; index < numDS18; index++) {
		Serial.print(F("Temp Sensor Index: "));
		Serial.print(index);
		Serial.print(F(", OneWire Bus Index: "));
		Serial.print(tempSensors.getOneWireIndexFromTempSensorIndex(index));
		Serial.print(F(", Address: "));
		tempSensors.getAddressFromTempSensorIndex(addr, index);
		printAddress(addr);
		Serial.println();
	}

	// Setup first temperature measurement
	currentState = WAITING_FOR_CONVERSION;
	measurementStartTime = millis();
	if (!tempSensors.startConvertion()) {
		Serial.println(F("Failed to Start Conversion Cycle, Aborting"));
		currentState = ABORTED;
	}
	Serial.println();
}

void loop() {
	uint8_t index;
	unsigned long currentMillis;
	static uint32_t loopCount = 0;

	switch (currentState) {
	case WAITING_FOR_CONVERSION:
		if (tempSensors.isConversionDone()) {
			// Measurement has finished print results
			measurementStopTime = millis();
			Serial.print(F("Temp Measurement of "));
			Serial.print(numDS18);
			Serial.print(F(" Sensors Took: "));
			Serial.print(measurementStopTime - measurementStartTime);
			Serial.println(F(" ms"));
			Serial.print(loopCount);
			Serial.println(
					F(" Iterations of loop occurred during this period."));
			// Print measurements from all temperature sensors
			for (index = 0; index < numDS18; index++) {
				Serial.print(F("Sensor "));
				Serial.print(index);
				Serial.print(F(" Results:  "));
				Serial.print(tempSensors.getLatestTempRaw(index));
				Serial.print(F(" (Raw), "));
				Serial.print(tempSensors.getLatestTempC(index), 1);
				Serial.print(F(" (C), "));
				Serial.print(tempSensors.getLatestTempF(index), 1);
				Serial.println(F(" (F)"));
			}
			Serial.println();
			// Hang out until time for next measurement
			currentState = IDLE;
			interMeasurementTimer = millis();
		} else if (millis() - measurementStartTime >= maxWaitTime) {
			Serial.println(
					F(
							"Timed Out Waiting for Measurements to Complete -- Aborting"));
			currentState = ABORTED;
		}
		break;

	case IDLE:
		currentMillis = millis();
		// Check if it's time to start another measurement
		if (currentMillis - interMeasurementTimer >= interMeasurementPeriod) {
			currentState = WAITING_FOR_CONVERSION;
			loopCount = 0;
			// Kick off another set of non-blocking sensor readings
			measurementStartTime = currentMillis;
			if (!tempSensors.startConvertion()) {
				Serial.println(F("Failed to Start Conversion Cycle, Aborting"));
				currentState = ABORTED;
			}
		}
		break;

	case ABORTED:
		break;
	}

	// Do useful stuff here since Temperature Sensor code is non-blocking
	loopCount++;
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
	for (uint8_t i = 0; i < 8; i++) {
		// zero pad the address if necessary
		if (deviceAddress[i] < 0x10)
			Serial.print("0");
		Serial.print(deviceAddress[i], HEX);
	}
}
