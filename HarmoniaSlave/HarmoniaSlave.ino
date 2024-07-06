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

#include <PinChangeInterrupt.h>
#include <PinChangeInterruptBoards.h>
#include <PinChangeInterruptPins.h>
#include <PinChangeInterruptSettings.h>
#include <Wire.h>

//this serial will be used to transfer data to the serial2 on the mega
//#define serialToMega Serial1

//pressure sensor for closed loop with air bag
//Adafruit_MPRLS mpr = Adafruit_MPRLS(-1, -1);
//boolean blnMPR_OK = false;
// 
//DFRobot_INA219_IIC ina219(&Wire, INA219_I2C_ADDRESS4);
// Revise the following two paramters according to actual reading of the INA219 and the multimeter
// for linearly calibration
//float ina219Reading_mA = 1000;
//float extMeterReading_mA = 1000;
//boolean blnIna219_OK = false;

#define I2C_ADDR 12

//the hall effect sensor is connected to this digital pin on the leonardo
#define pinRPM 11

unsigned int intRevolutions;
unsigned int intRPM;
unsigned int intTimeold;
unsigned int intTimePrev;

long lng1minInMSecs = 60000;

void setup() {
	Wire.begin(I2C_ADDR);
	Wire.onRequest(readingRequested);

	Serial.begin(9600); // begin Serial
	
	//serialToMega.begin(9600);

	//allow sensors to initialise
	delay(2000);

	//setup for hall effect sensor - uses pinchangeinterupps rather than standard interupts (the standard interupt pins on the leonardo
	//are not available on this particular board)
	pinMode(pinRPM, INPUT_PULLUP);
	attachPCINT(digitalPinToPCINT(pinRPM), rpm_interupt, RISING);
	intRevolutions = 0;
	intRPM = 0;
	intTimeold = 0;
	intTimePrev = millis();

	//init power sesnor
	//if (!ina219.begin()) {
	//	Serial.println("ina219 power sensor failed to initialise");
	//}
	//else {
	//	blnIna219_OK = true;
	//	ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
	//}
	
	//init air bag pressure sensor
	//if (!mpr.begin()) {
	//	Serial.println("Failed to communicate with MPRLS sensor, check wiring?");
	//	
	//	/*while (1) {
	//		delay(10);
	//	}*/
	//}
	//else {
	//	blnMPR_OK = true;
	//	Serial.println("Found MPRLS sensor");
	//}
	
	//initialise the pressure/temp sensor using the default address
	//SPL_init(); // Setup initial SPL chip registers - default i2c address 0x76  
	// SPL_init(0x77); // Uncomment for alternate I2C address 0x77

}

//triggered by hall effect sensor connected to leonardo
void rpm_interupt(void) {
	//intRevolutions++;

	int intTimeThis = millis();
	int intDeltaTime = intTimeThis - intTimePrev;
	intRPM = lng1minInMSecs / intDeltaTime;
	intTimePrev = intTimeThis;

}
void readingRequested() {
	Wire.write(byte((intRPM & 0x000000FF)));
	Wire.write(byte((intRPM & 0x0000FF00) >> 8));
	Wire.write(byte((intRPM & 0x00FF0000) >> 16));
	Wire.write(byte((intRPM & 0xFF000000) >> 24));
}

void loop() {

	//calc new RPM (be carfefull with changing long/int types - strange results occur when doing as a single line calculation)
	
	/*int intDeltaTime = millis() - intTimeold;
	int intMultiplier = lng1minInSecs / intDeltaTime;
	intRPM = intRevolutions * intMultiplier;*/
	
	//reset values
	//intTimeold = millis();
	//intRevolutions = 0;

	//delay(0);


	//float pressure_hPa = -1;
	//float pressure_PSI = -1;
	//if (blnMPR_OK) {
	//	pressure_hPa = mpr.readPressure();
	//	pressure_PSI = pressure_hPa / 68.947572932;
	//}

	//float bus_voltage = -1;
	//float shunt_voltage = -1;
	//float current = -1;
	//float power = -1;
	//if (blnIna219_OK) {
	//	bus_voltage = ina219.getBusVoltage_V();
	//	shunt_voltage = ina219.getShuntVoltage_mV();
	//	current = ina219.getCurrent_mA();
	//	power = ina219.getPower_mW();
	//}
	//String strData = String(intRPM) + "," + String(get_pressure()) + "," + String(get_temp_c()) + "," + String(pressure_hPa) + "," + String(bus_voltage) + "," + String(shunt_voltage) + "," + String(current) + "," + String(power);
	
	
	//send latest data to the mega (format RPM,pressure,temp)
	//serialToMega.print(strData);
	
	//Serial.println(strData);

	//display revolutions in debug mode - this is a good indicator of a working/not working sensor
	//NOTE IF TESTING IN DEBUG MODE ON LEANARDO - RESULTS COMING THROUGH TO REMOTE MIGHT GET CORRUPTED!!
	//CORRECT RESULTS APPEAR IN REMOTE FORM WHEN LEANOARDO IS POWERED BY SUB POWER AND NOT IN DEBUG MODE
	//Serial.println(String(intRevolutions) + "," + String(intRPM) + "," + String(get_pressure()) + "," + String(get_temp_c()) + "," + String(pressure_hPa) + "," + strPower);

}



