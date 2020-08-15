#include "Servo.h"

Servo::Servo()
{
	delay(1000);
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

Servo::~Servo()
{

	ledcDetachPin(SERVO_PIN_0);	//turning off power to servo
}

// ************************************* //
//				Servo Init		         //
//	Description: Method of Servo Class	 //
//	that initialzes servo and set to     //
//	to a beginning center position.		 //
// ************************************* //
void Servo :: servo_init() 
{
	delay(1000);
	Serial.println("Servo Control Beginning:");
	ledcSetup(channel, freq, resolution);
	ledcAttachPin(SERVO_PIN_0, channel);
	ledcWrite(channel, dutyCycle);	//set
}

void servos_Open()
{
	// Servo servo1;
	// servo1.servo_init();
	printf("DOES NOTHING RN \n");
}

// ************************************* //
//			Servo Position Set   		 //
//	Description: Allows for servo 		 //
//	position to be set directly digitally// 
// ************************************* //
void lock_servo_set(Servo servo1, int state){
	// Begin by making sure the pin is enabled and configured
	ledcAttachPin(SERVO_PIN_0, servo1.channel);
	int lockPosition = 16; // Initialized to 20 which is the UNLOCKED position
	if (state == 0){
		lockPosition = 16;	//Lock only rotates 90 degrees, so set unlocked servo state to the neutral position
	}
	if (state == 1){
		lockPosition = 30;  //Lock only rotates 90 degrees, so set locked servo state to the upright position
	}
	Serial.println("Setting duty cycle...");
	delay(250);
	servo1.dutyCycle = lockPosition;
	Serial.println("Duty Cycle Set to --> ");
	Serial.println(servo1.dutyCycle);
	ledcWrite(servo1.channel, servo1.dutyCycle); //writes current duty cycle to servo


	// Detaching Pin when finished to save on power
	delay(1000);
	Serial.println("Detaching pin...");
	ledcDetachPin(SERVO_PIN_0);
}

// ************************************* //
//			Servo Control Loop   		 //
//	Description: Allows for servo 		 //
//	control by hand with petentiometer.	 // 
// CURRENTLY UNUSED BUT USEFUL AS 	     //
//  REFERENCE FOR SERVO CONTROL			 //
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