#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
     // Includes the PulseSensorPlayground Library.   

// #define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
// #include <PulseSensorPlayground.h>

// static PulseSensorPlayground psp;
// void initSensor()
// {   
//     psp.analogInput(PULSE_ANALOG_READ_PIN);   
//     psp.blinkOnPulse(LED_PIN);       
//     psp.setThreshold(PULSE_THRESHOLD_SENSOR);       
//     if (psp.begin()) {
//         Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
//     }
// }

// float readBPM()
// {
//     return psp.getBeatsPerMinute();
// }
    int reading = 0;
    float BPM = 0.0;
    bool IgnoreReading = false;
    bool FirstPulseDetected = false;
    unsigned long FirstPulseTime = 0;
    unsigned long SecondPulseTime = 0;
    unsigned long PulseInterval = 0;                               
float readBPM() {
      reading = analogRead(A0);
      // Heart beat leading edge detected.
      if(reading > MAX_THRESHOLD && IgnoreReading == false){
        if(FirstPulseDetected == false){
          FirstPulseTime = millis();
          FirstPulseDetected = true;
        }
        else{
          SecondPulseTime = millis();
          PulseInterval = SecondPulseTime - FirstPulseTime;
          FirstPulseTime = SecondPulseTime;
        }
        IgnoreReading = true;
      }

      // Heart beat trailing edge detected.
      if(reading < MIN_THRESHOLD){
        IgnoreReading = false;
      }  

      BPM = (1.0/PulseInterval) * 60.0 * 1000;    
      return BPM;
}

bool readMessage(int messageId, char *payload)
{
    float bpm = readBPM();
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    root["deviceId"] = DEVICE_ID;
    root["messageId"] = messageId;
    bool BPMAleart = false;

    // NAN is not the valid json, change it to NULL
    if (std::isnan(bpm))
    {
        root["BPM"] = NULL;
    }
    else
    {
        root["BPM"] = bpm;
        if (bpm > BPM_ALERT)
        {
            BPMAleart = true;
        }
    }

    root.printTo(payload, MESSAGE_MAX_LEN);
    return BPMAleart;
}

void parseTwinMessage(char *message)
{
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(message);
    if (!root.success())
    {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }

    if (root["desired"]["interval"].success())
    {
        interval = root["desired"]["interval"];
    }
    else if (root.containsKey("interval"))
    {
        interval = root["interval"];
    }
}
