/*
 Name:		HarmoniaSlave
 Created:	12/18/2022 7:09:49 PM
 Author:	eugene lamnek


link to the libraries for the jaycar pressure/temp sensor:
 https://www.arduino.cc/reference/en/libraries/spl06-007/

link to the libraries for the adafruit pressure sensor:
https://github.com/adafruit/Adafruit_MPRLS

link to the pinchangeinterrupt libraries on github (used for hall effect RPM sensor)
https://github.com/NicoHood/PinChangeInterrupt


*/


#include <Adafruit_MPRLS.h>
#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <SPL06-007.h>
#include <Wire.h>

//this serial will be used to transfer data to the serial2 on the mega
#define serialToMega Serial1

//the hall effect sensor is connected to this digital pin on the leonardo
#define pinRPM 11

unsigned int intRevolutions;
unsigned int intRPM;
unsigned int intTimeold;

//pressure sensor for closed loop with air bag
Adafruit_MPRLS mpr = Adafruit_MPRLS(-1, -1);

void setup() {
	Wire.begin();    // begin Wire(I2C)
	Serial.begin(9600); // begin Serial
	
	serialToMega.begin(9600);

	//init air bag pressure sensor
	if (!mpr.begin()) {
		Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
		while (1) {
			delay(10);
		}
	}
	Serial.println("Found MPRLS sensor");


	//initialise the pressure/temp sensor using the default address
	SPL_init(); // Setup initial SPL chip registers - default i2c address 0x76  
	// SPL_init(0x77); // Uncomment for alternate I2C address 0x77

	//setup for hall effect sensor - uses pinchangeinterupps rather than standard interupts (the standard interupt pins on the leonardo
	//are not available on this particular board)
	pinMode(pinRPM, INPUT_PULLUP);
	attachPCINT(digitalPinToPCINT(pinRPM), rpm_interupt, RISING);
	intRevolutions = 0;
	intRPM = 0;
	intTimeold = 0;
}

//triggered by hall effect sensor connected to leonardo
void rpm_interupt(void) {
	intRevolutions++;
}

void loop() {

	//calc new RPM (be carfeul with changing long/int types - strange results occur when doing as a single line calculation)
	long lng1minInSecs = 60000;
	int intDeltaTime = millis() - intTimeold;
	int intMultiplier = lng1minInSecs / intDeltaTime;
	intRPM = intRevolutions * intMultiplier;
	
	//reset values
	intTimeold = millis();
	intRevolutions = 0;

	float pressure_hPa = mpr.readPressure();
	float pressure_PSI = pressure_hPa / 68.947572932;
	
	//send latest data to the mega (format RPM,pressure,temp)
	serialToMega.print(String(intRPM) + "," + String(get_pressure()) + "," + String(get_temp_c()) + "," + String(pressure_PSI));

	//display revolutions in debug mode - this is a good indicator of a working/not working sensor
	Serial.println(String(intRevolutions) + "," + String(intRPM) + "," + String(get_pressure()) + "," + String(get_temp_c()) + "," + String(pressure_PSI));

	//0.5 second pause - ensure that data is send roughly every half a second
	delay(1000);

}



