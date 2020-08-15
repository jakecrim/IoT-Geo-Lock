#include <WiFi.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
/* DEFINES */
//  WiFi & MQTT 
#define WIFI_SSID "test"
#define WIFI_PASS "12345678"
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVER_PORT 1883
#define AIO_USERNAME "JakeCrim"
#define AIO_KEY "b30b1587733f4ab59608b552bf0fed49"

//	WiFi & MQTT Client Variables
// 	-Store the MQTT server, username, and password in flash memory.
// 		This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;


// functions
void WiFi_connect();
void MQTT_loop();