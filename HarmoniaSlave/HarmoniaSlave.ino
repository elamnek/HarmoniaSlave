#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>



/*
 Name:		LeonardoTest.ino
 Created:	12/18/2022 7:09:49 PM
 Author:	eugen

  https://www.arduino.cc/reference/en/libraries/spl06-007/


*/

#include <SPL06-007.h>
#include <Wire.h>

//#define Serial  SerialUSB

#define pinRPM 11

unsigned int revolutions;
unsigned int rpm;
unsigned int timeold;

void setup() {
	Wire.begin();    // begin Wire(I2C)
	Serial.begin(9600); // begin Serial
	Serial1.begin(9600);

	Serial.println("\nGoertek-SPL06-007 Demo\n");

	SPL_init(); // Setup initial SPL chip registers - default i2c address 0x76  
	// SPL_init(0x77); // Uncomment for alternate I2C address 0x77

	pinMode(pinRPM, INPUT_PULLUP);
	attachPCINT(digitalPinToPCINT(pinRPM), interuptus, RISING);

	revolutions = 0;
	rpm = 0;
	timeold = 0;

}
void interuptus(void) {
	revolutions++;
	// intCounter = intCounter + 1;
	// Serial.println(String(intCounter));
	// return;
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


	Serial1.print(String(rpm) + "," + String(get_pressure()));
	//Serial.println(" mb");

	//Serial.println("\n");
	delay(1000);

	Serial.println(rpm);
}



