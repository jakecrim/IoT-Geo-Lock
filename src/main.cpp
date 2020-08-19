/* main.cpp */
#include "AutoLocker.h"
#include "Servo.h"
#include "transmission.h"

using namespace std;

int main(void)
{
	Serial.begin(115200);
	//wait for serial to be setup properly
	delay( 1000 );

	// Connect to the defined WiFi Network
	wifi_Open();

	servos_Open();
	
	// Go to mqtt_Task() and stay there forevor waiting for updates from the cloud.
	// as much as I'd like to run all this freeRTOS style it just isn't necessary yet
	mqtt_Task();

	// if all goes well, we will never end up in loop()
}



// Unused Except to Call Main
void setup() 
{
	main();
}

void loop() 
{       
	Serial.println("Arrived in Loop should NOT be here... \n");
	delay(1000);
}





