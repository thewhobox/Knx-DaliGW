#include <Arduino.h>
#include "OpenKNX.h"
#include "DaliModule.h"
#include "TimerModule.h"
#include "FileTransferModule.h"


void daliCallback(uint8_t *data, uint8_t len)
{
	logHexInfo("Dali In", data, len);
}

bool setup0ready = false;
bool setup1ready = false;

void setup()
{
	const uint8_t firmwareRevision = 0;
	openknx.init(firmwareRevision);
	//openknxDaliModule.setCallback(daliCallback);
	openknx.addModule(1, openknxDaliModule);
	openknx.addModule(2, openknxTimerModule);
	openknx.addModule(3, openknxFileTransferModule);
	openknx.setup();

#ifdef ARDUINO_ARCH_RP2040
	setup0ready = true;

	while(!setup1ready)
		delay(1);
#endif
}

void loop()
{
	openknx.loop();
}

#ifdef ARDUINO_ARCH_RP2040
void setup1()
{
	while(!setup0ready)
		delay(1);
	openknxDaliModule.setup1(knx.configured());
	setup1ready = true;
}

void loop1()
{
	openknxDaliModule.loop1(knx.configured());
}
#elif defined(ARDUINO_ARCH_ESP32)
void setup1()
{
	openknx.setup1();
}

void loop1()
{
	openknx.loop1();
}
#endif