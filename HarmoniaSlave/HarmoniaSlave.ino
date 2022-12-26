/*
 Name:		HarmoniaSlave
 Created:	12/18/2022 7:09:49 PM
 Author:	eugene




  https://www.arduino.cc/reference/en/libraries/spl06-007/


*/


#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <SPL06-007.h>
#include <Wire.h>


#define serialToMega Serial1

#define pinRPM 11

unsigned int revolutions;
unsigned int rpm;
unsigned int timeold;

void setup() {
	Wire.begin();    // begin Wire(I2C)
	Serial.begin(9600); // begin Serial
	
	serialToMega.begin(9600);

	SPL_init(); // Setup initial SPL chip registers - default i2c address 0x76  
	// SPL_init(0x77); // Uncomment for alternate I2C address 0x77

	//setup for hall effect sensor - uses pinchangeinterupps rather than standard interupts (the standard interupt pins on the leonardo
	//are not available on this partical board)
	pinMode(pinRPM, INPUT_PULLUP);
	attachPCINT(digitalPinToPCINT(pinRPM), rpm_interupt, RISING);
	revolutions = 0;
	rpm = 0;
	timeold = 0;
}

//triggered by hall effect sensor connected to leonardo
void rpm_interupt(void) {
	revolutions++;
}

void loop() {


	//if (revolutions >= 20){
	long nume = 60000;
	int denom = millis() - timeold;
	int fact = nume / denom;
	rpm = revolutions * fact;
	//rpm = 60 * (revolutions/(millis()-timeold));
	timeold = millis();
	revolutions = 0;
	//}

		// Serial.print("Temperature: ");
		// Serial.print(get_temp_c());
		// Serial.println(" C");

		//Serial.print("Measured Air Pressure: ");
		//Serial.println(get_pressure(), 2);
	  //Serial1.print(get_pressure(), 2);


	serialToMega.print(String(rpm) + "," + String(get_pressure()));
	//Serial.println(" mb");

	//Serial.println("\n");
	delay(1000);

	Serial.println(rpm);
}



