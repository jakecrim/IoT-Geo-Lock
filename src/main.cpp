/* main.cpp */


#include "AutoLocker.h"

using namespace std;
// ************************************* //
//			Hardware Definitions		 //
//	Description: Pins Mainly			 //
// ************************************* //
#define ANALOG_PIN_0 34
#define SERVO_PIN_0 23

// ************************************* //
//  WiFi & MQTT Definitions              //
//	Description: Usernames, passwords,	 //
//	addresses, keys, secret formulas...	 //
// ************************************* //
#define WIFI_SSID "test"
#define WIFI_PASS "12345678"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVER_PORT 1883
#define AIO_USERNAME "JakeCrim"
#define AIO_KEY "b30b1587733f4ab59608b552bf0fed49"

// ************************************* //
//			WiFi & MQTT Client Variables //
//	Description: Variables to be used	 //
//	in setting up WiFi and MQTT.		 //
//	P.S. will eventually move to header	 //
// ************************************* //
// Store the MQTT server, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

//Creating a WiFi Client instance
WiFiClient client;
// Setup MQTT For Adafruit IO
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVER_PORT, AIO_USERNAME, AIO_KEY);
// Subscribe information, named after the feed in AiO we will be listening to.			  *MQTT Topic: again named for the feed we are listening to
Adafruit_MQTT_Subscribe ifttt_notification = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ifttt-location");

// ************************************* //
//				CLASS: Servo		     //
//	Description: Class for the servo obj //	 
//	contains variables with information	 //
//	and settings used in controlling the //
//	servo.								 //
// ************************************* //
class Servo{
public:
	int testVar;


	int analog_val;		//value to be read in from the analog controller
	double freq;		
	uint8_t channel;
	uint8_t resolution;
	int dutyCycle;

	void servo_init(); //initializes the servo position

	/** Default Constructor **/
	Servo(){
		delay(1000);
		testVar = 0;		//var to see if outside functions have access.
		analog_val = 0; 	
		freq = 50;			//can be set to other frequencies such as ______
		channel = 0;
		resolution = 8;		//8 bits for calculations	
		dutyCycle = 16;		// about 90 deg or halfway point (set ~ 10-35 high to low )
		
		Serial.println("Servo Control Beginning:");
		ledcSetup(channel, freq, resolution);
		ledcAttachPin(SERVO_PIN_0, channel);
		ledcWrite(channel, dutyCycle);	//set	
	}
	/** Default Destructor **/
	~Servo(){
		ledcDetachPin(SERVO_PIN_0);	//turning off power to servo
	}
};

// ************************************* //
//				Servo Init		         //
//	Description: Method of Servo Class	 //
//	that initialzes servo and set to     //
//	to a beginning center position.		 //
// ************************************* //
void Servo :: servo_init() {
	delay(1000);
	Serial.println("Servo Control Beginning:");
	ledcSetup(channel, freq, resolution);
	ledcAttachPin(SERVO_PIN_0, channel);
	ledcWrite(channel, dutyCycle);	//set
}

// ************************************* //
//			Function Declarations		 //
// ************************************* //
void servo_control_loop(Servo &servo);
void lock_servo_set(Servo &servo, int state);
void MQTT_connect();
void MQTT_loop(Servo &servo);
void WiFi_connect();

// ************************************* //
//			   WiFi Connect		   		 //
//	Description: Attempts to Connect	 //
//	to WiFi until successful.	 		 //
// ************************************* //
void WiFi_connect(){
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
void MQTT_connect(){
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
void MQTT_loop(Servo &servo){
	while(1){
		// Check to see if MQTT is connected, if not, it will connect
		MQTT_connect();
		Adafruit_MQTT_Subscribe *subscription;

		// Start unlocked, update later with publish information (0 = UNLOCKED, 1 = LOCKED)
		int lock_state = 0; 

		// Check subscription information to see if any actions need completed.
		while ((subscription = mqtt.readSubscription(5000))){
			if (subscription == &ifttt_notification){
				Serial.print(F("Got: "));
				Serial.println((char *)ifttt_notification.lastread);
				if (!strcmp((char*) ifttt_notification.lastread, "departed")){
					Serial.println("User has left.");
					// Set state to equal 1 since we have "left home" and want to set servo to LOCKED
					lock_state = 1;
					// Send the servo obj and lock state to the position set function
					lock_servo_set(servo, lock_state);
				}
				else if (!strcmp((char*) ifttt_notification.lastread, "arrived")){
					Serial.println("User has arrived");
					// Set state to equal 0 since we have "arrived home" and want to set servo to UNLOCKED
					lock_state = 0;
					// Send the servo obj and lock state to the position set function
					lock_servo_set(servo, lock_state);					
				}
				else {
					Serial.println("ERROR: Data value sent from iPhone not valid!");
					delay(1000);
				}
			}
		}	
	}
}

// ************************************* //
//			Servo Position Set   		 //
//	Description: Allows for servo 		 //
//	position to be set directly digitally// 
// ************************************* //
void lock_servo_set(Servo &servo, int state){
	// Begin by making sure the pin is enabled and configured
	ledcAttachPin(SERVO_PIN_0, servo.channel);
	int lockPosition = 16; // Initialized to 20 which is the UNLOCKED position
	if (state == 0){
		lockPosition = 16;	//Lock only rotates 90 degrees, so set unlocked servo state to the neutral position
	}
	if (state == 1){
		lockPosition = 30;  //Lock only rotates 90 degrees, so set locked servo state to the upright position
	}
	Serial.println("Setting duty cycle...");
	delay(250);
	servo.dutyCycle = lockPosition;
	Serial.println("Duty Cycle Set to --> ");
	Serial.println(servo.dutyCycle);
	ledcWrite(servo.channel, servo.dutyCycle); //writes current duty cycle to servo


	// Detaching Pin when finished to save on power
	delay(1000);
	Serial.println("Detaching pin...");
	ledcDetachPin(SERVO_PIN_0);
}

// ************************************* //
//			Servo Control Loop   		 //
//	Description: Allows for servo 		 //
//	control by hand with petentiometer.	 // 
// ************************************* //
void servo_control_loop(Servo &servo){
	
	Serial.println("Servo Control Loop has begun:");
	while(1){
		servo.analog_val = analogRead(ANALOG_PIN_0);
		Serial.println(servo.analog_val);
		Serial.println("Duty Cycle --> ");
		Serial.println(servo.dutyCycle);

		// 0-4095 resolution something??? then 10-33 is ~ range of servo position
		servo.dutyCycle = map(servo.analog_val, 0, 4095, 10, 33);  
		ledcWrite(servo.channel, servo.dutyCycle); //writes current duty cycle to servo
		// wait 50ms
		delay(50);
	}
	
}

void setup() {
	Serial.begin(115200);
	//wait for serial to be setup properly
	delay( 1000 );

	// Connect to the defined WiFi Network
	WiFi_connect();

	// Setup the MQTT Subscribe 
	mqtt.subscribe(&ifttt_notification);

	// 1st servo being controlled by this project
	Servo servo1;

	// Initializing position and settings of servo1
	servo1.servo_init();
	
	// Go to MQTT_loop and stay there forevor waiting for updates from the cloud.
	MQTT_loop(servo1);

	// NOT USING ANYMORE, CAN BE USED TO CONTROL SERVO BY HAND ... old info: Beginning the loop of controlling servo from manual analog input.
	//servo_control_loop(servo1);
}


// ************************************* //
//				LOOP UNUSED			     //
// ************************************* //
void loop() {       
	Serial.println("Arrived in Loop");
	delay(1000);
}





