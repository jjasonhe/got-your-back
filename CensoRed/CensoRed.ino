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

#define UPIN_TOP    40
#define UPIN_BOT    36
#define BUZZER_PIN  39

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
  Serial.begin(115200);
  myEvent.begin();
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop()
{
  delay(RATE_ERGO);
  
  Serial.print("delayed the ergo rate\n");
  delay(1000);
  
  DIST_TOP = ultrasonic.MeasureInCentimeters();
  
  Serial.print("distance variables updated\n");
  Serial.print(DIST_TOP);
  Serial.print("\n");
  delay(1000);
  
  DIST_BOT = ultratwo.MeasureInCentimeters();
  
  delay(1000);
  
  Serial.print(DIST_BOT);
  Serial.print("\n");
  
  if ((DIST_TOP < 7) && (DIST_BOT < 7))
  {
    CNT_YES++;
    CNT_STAND = CNT_STAND + 5;
    
    Serial.print(CNT_STAND);
    Serial.print("\n");
    
    if (CNT_STAND == RATE_STAND)
    {
      Serial.print("time to stand; send to YPhi\n");
         
      alert[0] = 0x32;
      
      Serial.print(alert[0]);
      Serial.print("\n");
      
      myEvent.send();
      myEvent.waitFor();          // NOT SURE IF THIS IS NECESSARY ********
    }
  }
  else
  {
    CNT_NOP++;
    CNT_STAND = CNT_STAND + 5;
    buzz(30);
    alert[0] = 0x31;                   // alert[] = "01" when bad posture
    
    Serial.print("bad posture\n");
    Serial.print(alert[0]);
    Serial.print("\n");
    
    myEvent.send();
    myEvent.waitFor();            // NOT SURE IF THIS IS NECESSARY ********
  }
}

void buzz(int x)
{
  for (int OP = 0; OP < 2; OP++)
  {
    for (int i = 0; i < x; i++)
    {
      digitalWrite(BUZZER_PIN, HIGH);
      delayMicroseconds(1915);
      digitalWrite(BUZZER_PIN, LOW);
      delayMicroseconds(1915);
    }
    delay(100);
  }
}
