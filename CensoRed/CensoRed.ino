#include "Ultrasonic.h"
#include "Ultratwo.h"
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "Event.h"

Event myEvent;

// your network name also called SSID
char ssid[] = "QuadJay";
// your network password
char password[] = "HE4696339711";
// MQTTServer to use
char server[] = "iot.eclipse.org";

uint8_t YPHI_VAR = 0;

#define UPIN_TOP 40
#define UPIN_BOT 36

Ultrasonic ultrasonic(UPIN_TOP);
Ultratwo ultratwo(UPIN_BOT);

uint32_t DIST_TOP = 0;
uint32_t DIST_BOT = 0;

uint32_t RATE_ERGO = 5000;      // Currently set at 5 sec
uint32_t RATE_STAND = 30;         // Currently set at 30 sec
uint32_t CNT_YES = 0;             // Counter for good times
uint32_t CNT_NOP = 0;             // Counter for bad times
uint32_t CNT_STAND = 0;           // Counter to cmp with RATE_STAND

char alert[4] = "0";

void setup()
{
  Serial.begin(9600);
  myEvent.begin();
}

void loop()
{
  delay(RATE_ERGO);
  
  DIST_TOP = ultrasonic.MeasureInCentimeters();
  DIST_BOT = ultratwo.MeasureInCentimeters();
  
  if ((DIST_TOP < 7) && (DIST_BOT < 7))
  {
    CNT_YES++;
    CNT_STAND = CNT_STAND + (RATE_ERGO / 60000);
    if (CNT_STAND >= RATE_STAND)
    {
      // buzzer? ********
      alert[0] += 2;
      myEvent.send();
      myEvent.waitFor();          // NOT SURE IF THIS IS NECESSARY ********
    }
  }
  else
  {
    CNT_NOP++;
    // buzzer? ********
    alert[0]++;                   // alert[] = "01" when bad posture
    myEvent.send();
    myEvent.waitFor();            // NOT SURE IF THIS IS NECESSARY ********
  }
}
