/* main.cpp */
#include "AutoLocker.h"
#include "Servo.h"
#include "transmission.h"

using namespace std;



// ************************************* //
//			Function Declarations		 //
// ************************************* //
// void servo_control_loop(Servo &servo);
// void lock_servo_set(Servo &servo, int state);
// void MQTT_connect();
// void MQTT_loop(Servo &servo);
// void WiFi_connect();

int main(void)
{
	Serial.begin(115200);
	//wait for serial to be setup properly
	delay( 1000 );

	// Connect to the defined WiFi Network
	WiFi_connect();

	servos_Open();
	
	// Go to MQTT_loop and stay there forevor waiting for updates from the cloud.
	MQTT_loop();

	// NOT USING ANYMORE, CAN BE USED TO CONTROL SERVO BY HAND ... old info: Beginning the loop of controlling servo from manual analog input.
	//servo_control_loop(servo1);
}



// Unused Except to Call Main
void setup() 
{
	main();
}

void loop() 
{       
	Serial.println("Arrived in Loop");
	delay(1000);
}





