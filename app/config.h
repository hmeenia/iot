// Physical device information for board and sensor
#define DEVICE_ID "Feather HUZZAH ESP8266 WiFi"


// Pin layout configuration
#define LED_PIN 0
#define PULSE_ANALOG_READ_PIN 17


#define MIN_THRESHOLD 500
#define MAX_THRESHOLD 570

#define BPM_ALERT 200

// Interval time(ms) for sending message to IoT Hub
#define INTERVAL 0

// If don't have a physical DHT sensor, can send simulated data to IoT hub
#define SIMULATED_DATA false

// EEPROM address configuration
#define EEPROM_SIZE 512

// SSID and SSID password's length should < 32 bytes
// http://serverfault.com/a/45509
#define SSID_LEN 32
#define PASS_LEN 32
#define CONNECTION_STRING_LEN 256

#define MESSAGE_MAX_LEN 256
