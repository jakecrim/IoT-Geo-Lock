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
	WiFi_connect();

	servos_Open();
	
	// Go to MQTT_loop and stay there forevor waiting for updates from the cloud.
	MQTT_loop();

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





