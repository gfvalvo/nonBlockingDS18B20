// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _nonBlockingDS18B20_H_
#define _nonBlockingDS18B20_H_
#include "Arduino.h"
#include <DallasTemperature.h>

struct tempSensorInfo;

class nonBlockingDS18B20: public DallasTemperature {
public:
	nonBlockingDS18B20(OneWire *w) :
			DallasTemperature(w) {
	}
	void getAddressFromTempSensorIndex(DeviceAddress, uint8_t);
	boolean startConvertion(void);
	boolean startConvertion(uint8_t);
	boolean isConversionDone(void);
	uint8_t begin(uint8_t);
	uint8_t getOneWireIndexFromTempSensorIndex(uint8_t);
	int16_t getLatestTempRaw(uint8_t);
	float getLatestTempC(uint8_t);
	float getLatestTempF(uint8_t);

private:
	boolean parasiteMode;
	boolean conversionInProcess;
	uint8_t numTempSensors;
	uint32_t waitTime;
	uint32_t conversionStartTime;
	tempSensorInfo *infoPtr;
	void updateTemps(void);
};


#endif /* _nonBlockingDS18B20_H_ */
