#include "transmission.h"
#include "Servo.h"

//Creating a WiFi Client instance
WiFiClient client;
// Setup MQTT For Adafruit IO
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVER_PORT, AIO_USERNAME, AIO_KEY);
// Subscribe information, named after the feed in AiO we will be listening to.			  *MQTT Topic: again named for the feed we are listening to
Adafruit_MQTT_Subscribe ifttt_notification = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ifttt-location");


// WiFi Connect	

void WiFi_connect()
{
	Serial.println("WiFi Disconnected...");
	WiFi.begin(WIFI_SSID, WIFI_PASS);
	WiFi.setSleep(false);
	while (WiFi.status() != WL_CONNECTED){
		delay(1000);
		Serial.println("Connecting to WiFi...");
	}
	Serial.println("Connected Successfully!");

	Serial.println("IP:"); Serial.println(WiFi.localIP());
}



// ************************************* //
//			   MQTT Connect		   		 //
//	Description: Attempts to Connect	 //
//	to MQTT for set number of tries.	 //
//  Also checks to see if connection	 //
// 	is still valid, returns if true.	 //
// ************************************* //
void MQTT_connect()
{
	int8_t ret;
	if (mqtt.connected()){
		return;
	}
	Serial.println("Connecting to MQTT...");
	uint8_t retries = 3;
	while ((ret = mqtt.connect()) !=0 ){
		Serial.println(mqtt.connectErrorString(ret));
		Serial.println("Retrynig MQTT connection in 5 seconds...");
		mqtt.disconnect();
		delay(5000);
		retries --;
		if (retries == 0){
			while(1);
		}
	}
	Serial.println("MQTT Connected!");
}

// ************************************* //
//			   MQTT Loop		   		 //
//	Description: Function we stay in 	 //
//	forever waiting for publishes from 	 //
//	AiO.								 //
// ************************************* //
void MQTT_loop()
{
	printf("IN LOOP? \n");
	Servo servo1;
	servo1.servo_init();
	// Setup the MQTT Subscribe 
	mqtt.subscribe(&ifttt_notification);

	Adafruit_MQTT_Subscribe *subscription;

	// Start unlocked, update later with publish information (0 = UNLOCKED, 1 = LOCKED)
	int lock_state = 0; 
	
	while(1)
	{
		// Check to see if MQTT is connected, if not, it will connec
		MQTT_connect();
		// Check subscription information to see if any actions need completed.
		while ((subscription = mqtt.readSubscription(5000)))
		{
			if (subscription == &ifttt_notification)
			{
				Serial.print(F("Got: "));
				Serial.println((char *)ifttt_notification.lastread);
				if (!strcmp((char*) ifttt_notification.lastread, "departed"))
				{
					Serial.println("User has left.");
					// Set state to equal 1 since we have "left home" and want to set servo to LOCKED
					lock_state = 1;
					// Send the servo obj and lock state to the position set function
					// lock_servo_set(servo1, lock_state);
					lock_servo_set(servo1, lock_state);
				}
				else if (!strcmp((char*) ifttt_notification.lastread, "arrived"))
				{
					Serial.println("User has arrived");
					// Set state to equal 0 since we have "arrived home" and want to set servo to UNLOCKED
					lock_state = 0;
					// Send the servo obj and lock state to the position set function
					lock_servo_set(servo1, lock_state);					
				}
				else 
				{
					Serial.println("ERROR: Data value sent from iPhone not valid!");
					delay(1000);
				}
			}
		}	
	}	
}