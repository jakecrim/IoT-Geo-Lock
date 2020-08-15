#include "AutoLocker.h"
/* DEFINES */
#define ANALOG_PIN_0 34
#define SERVO_PIN_0 23

// ************************************* //
//				CLASS: Servo		     //
//	Description: Class for the servo obj //	 
//	contains variables with information	 //
//	and settings used in controlling the //
//	servo.								 //
// ************************************* //
class Servo{
public:
	int analog_val;		//value to be read in from the analog controller
	double freq;		
	uint8_t channel;
	uint8_t resolution;
	int dutyCycle;

	void servo_init(); //initializes the servo position

	/** Default Constructor **/
	Servo();
	/** Default Destructor **/
	~Servo();
};


// // Variables
// extern Servo servo1;

// functions
void lock_servo_set(Servo, int);
void servos_Open(void);