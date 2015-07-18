void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message for topic ");
  Serial.print(topic);
  Serial.print("with length ");
  Serial.println(length);
  Serial.println("Message:");
  Serial.write(payload, length);
  Serial.println();
  if (YPHI_VAR == 1) {
    RATE_ERGO = *payload;
  }
  if (YPHI_VAR == 2) {
    RATE_STAND = *payload;
  }
}

WiFiClient wifiClient;
PubSubClient client(server, 1883, callback, wifiClient);

void setupYPhi()
{
  // no setup
}

void loopYPhi()
{
  myEvent.waitFor();                               // NOT SURE WHERE TO PUT THIS (setup, loop, top) ********
  
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
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an IP address");
  
  while (WiFi.localIP() == INADDR_NONE)
  {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  // We are connected and have an IP address.
  // Print the WiFi status.
  printWifiStatus();
  
  // Reconnect if the connection was lost
  if (!client.connected())
  {
    Serial.println("Disconnected. Reconnecting....");

    if(!client.connect("jasonClient"))
    {
      Serial.println("Connection failed");
    }
    else
    {
      Serial.println("Connection success");
      
      if (client.publish("ergoAlert", alert))
      {
        Serial.println("Publish success");
          if (alert[0] = 2)                              // if it was the stand alert, these loops force person to stand for 30000 for-loop cycles
          {
            for (int i = 0; i < 30000; i++)
            {
              DIST_TOP = ultrasonic.MeasureInCentimeters();
              DIST_BOT = ultratwo.MeasureInCentimeters();
              if ((DIST_TOP < 7) || (DIST_BOT < 7)) {
                // buzzer? ********
              }
            }
          }
        alert[0] = 0;
      }
      else
      {
        Serial.println("Publish failed");
      }
    }
  }
  
  // Reconnect if the connection was lost
  if (!client.connected())
  {
    Serial.println("Disconnected. Reconnecting....");

    if(!client.connect("jasonClient"))
    {
      Serial.println("Connection failed");
    }
    else
    {
      Serial.println("Connection success");
      if(client.subscribe("configPosture"))
      {
        YPHI_VAR = 1;      // so callback knows which topic it was from
        Serial.println("Sub posture successfull");
      }
    }
  }

  // Check if any message were received
  // on the topic we subsrcived to
  client.loop();
  
  // Reconnect if the connection was lost
  if (!client.connected())
  {
    Serial.println("Disconnected. Reconnecting....");

    if(!client.connect("jasonClient"))
    {
      Serial.println("Connection failed");
    }
    else
    {
      Serial.println("Connection success");
      if(client.subscribe("configMovement"))
      {
        YPHI_VAR = 2;      // so callback knows which topic it was from
        Serial.println("Sub movement successfull");
      }
    }
  }

  // Check if any message were received
  // on the topic we subsrcived to
  client.loop();
  
  WiFi.disconnect();

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
