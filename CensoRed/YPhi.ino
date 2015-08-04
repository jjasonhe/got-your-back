void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print(topic);
  Serial.write(payload, length);
  Serial.println();
}

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setupYPhi()
{
  // no setup
}

void loopYPhi()
{
  myEvent.waitFor();
  
  // Start Ethernet with the build in MAC Address
  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid, password);
  while ( WiFi.status() != WL_CONNECTED)
  {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("You're connected to the network");
  Serial.println("Waiting for an IP address");
  
  while (WiFi.localIP() == INADDR_NONE)
  {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("IP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();
  
  // Reconnect if the connection was lost
  if (!client.connected())
  {
    Serial.println("Disconnected. Reconnecting....");

    if(!client.connect("jasonClient"))
    {
      Serial.println("ergoConnection failed");
    }
    else
    {
      Serial.println("ergoConnection success");
      if (alert[0] == 0x32)
      {
        CNT_STAND = 0;
      }
      
      if (client.publish("ergoAlert", alert))
      {
        Serial.println("Publish success");
        
        if (alert[0] == 0x32)                              // if it was the stand alert, these loops force person to stand for 300 for-loop cycles
        {
          Serial.print("stand or it'll beep\n");
          
          for (int i = 0; i < 200; i++)
          {
            DIST_TOP = ultrasonic.MeasureInCentimeters();
            DIST_BOT = ultratwo.MeasureInCentimeters();
            Serial.print("measured again\n");
            if ((DIST_TOP < 7) || (DIST_BOT < 7))
            {
              buzz(30);
              Serial.print("sat back down ugh\n");
            }
          }
        }
        
        Serial.print("reset alerts\n");
        
        alert[0] = 0x30;
        
        Serial.print(alert[0]);
        Serial.print("\n");
      }
      else
      {
        Serial.println("Publish failed");
      }
    }
  }
  else
  {
    Serial.println("ergoConnection success");
    if (alert[0] == 0x32)
    {
      CNT_STAND = 0;
    }
   
    if (client.publish("ergoAlert", alert))
    {
      Serial.println("Publish success");
      
      if (alert[0] == 0x32)                              // if it was the stand alert, these loops force person to stand for 300 for-loop cycles
      {
        Serial.print("stand or it'll beep\n");
        
        for (int i = 0; i < 150; i++)
        {
          DIST_TOP = ultrasonic.MeasureInCentimeters();
          DIST_BOT = ultratwo.MeasureInCentimeters();
          Serial.print("measured again\n");
          if ((DIST_TOP < 7) || (DIST_BOT < 7))
          {
            buzz(30);
          }
        }
      }

      Serial.print("reset alerts\n");
      
      alert[0] = 0x30;
      
      Serial.print(alert[0]);
      Serial.print("\n");
    }
  }
  
  Serial.print("is this working\n");
  
  WiFi.disconnect();
  Serial.print("wifi disconnected\n");

  myEvent.send();
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
