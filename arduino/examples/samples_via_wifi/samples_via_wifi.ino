// Based on https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiClient/WiFiClient.ino

#include <ESP8266WiFi.h>
#include <Robo2022.h>

// Replace with actual values
const char* ssid = "NETWORK_SSID";
const char* password = "NETWORK_PASSWORD"

// Create object representing data collector for a particular sensor.
// Each sensor requires separate object.
RemoteDataCollector collector(
  /*producer_key=*/"key",
  /*sensor_name=*/"sensor_name",
  /*server_addr=*/"10.0.0.127");

void setup() {
  Serial.begin(115200);
  Serial.println("-----\n\n\n\n");
  // List what networks are available.
  LogAvailableNetworks();
  // Try to connect to the network.
  SetupWiFiClient(ssid, password);
}

int x = 1;
void loop() {
  // Each iteration, log an increasing number.
  collector.LogInteger(x);
  collector.LogFloat(x * 1.23);
  x++;
  delay(5000);
}
