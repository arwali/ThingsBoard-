// NOTE: Change the DHT pin connceted to data pin of NodeMCU accordingly when required.

#include <DHT.h>
#include <DHT_U.h>
#include "ThingsBoard.h"

#include <ESP8266WiFi.h>

#define ledPin D2

#define WIFI_AP             "Adi"
#define WIFI_PASSWORD       "A1234567"

// See https://thingsboard.io/docs/getting-started-guides/helloworld/
// to understand how to obtain an access token
#define TOKEN               "CvScXe7iZPshG7tZWxYP"
#define THINGSBOARD_SERVER  "demo.thingsboard.io"

// Baud rate for debug serial
#define SERIAL_DEBUG_BAUD   115200

#define dht_pin D6
#define DHTTYPE DHT11


// Initialize ThingsBoard client
WiFiClient espClient;
// Initialize ThingsBoard instance
ThingsBoard tb(espClient);
// the Wifi radio's status
int status = WL_IDLE_STATUS;
DHT dht(dht_pin, DHTTYPE);

void setup() {
  pinMode(ledPin, OUTPUT);
  // initialize serial for debugging
  Serial.begin(SERIAL_DEBUG_BAUD);
  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  InitWiFi();
  // Initialize DHT sensor.
DHT dht(dht_pin, DHTTYPE);
}

void loop() {
  delay(1000);

  if (WiFi.status() != WL_CONNECTED) {
    reconnect();
  }

  if (!tb.connected()) {
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN)) {
      Serial.println("Failed to connect");
      return;
    }
  }

  Serial.println("Sending data...");

  // The below function is to blink blue-LED on the module 
  // in order to show the connection of NodeMCU to ThingsBoard is successful
    digitalWrite(ledPin, HIGH);
      delay(200);
       digitalWrite(ledPin, LOW);
       delay(200);

  // Uploads new telemetry to ThingsBoard using MQTT.
  // See https://thingsboard.io/docs/reference/mqtt-api/#telemetry-upload-api
  // for more details
  // Reading temperature or humidity takes about 250 milliseconds!
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

 // read the input on analog pin 0 for MQ2 Gas Sensor:
  int sensorValue = analogRead(A0);
  
  Serial.println("Sending data to ThingsBoard:");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" *C ");
  Serial.println("CO2: ");
  // print out the value you read from gas sensor:
  Serial.print(sensorValue);
  tb.sendTelemetryInt("Temperature",33);
  tb.sendTelemetryFloat("Humidity", 77.1);
  tb.sendTelemetryInt("CO2", sensorValue);

  tb.loop();
}

void InitWiFi()
{
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  status = WiFi.status();
  if ( status != WL_CONNECTED) {
    WiFi.begin(WIFI_AP, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("Connected to AP");
  }
}
