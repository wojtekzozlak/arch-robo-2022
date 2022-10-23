// Based on https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/examples/WiFiClient/WiFiClient.ino

#include <Robo2022_wifi.h>
#include <SimpleDHT.h>

// Replace with actual values
const char* ssid = "{PUT_YOUR_WIFI_NETWORK_NAME_HERE}";
const char* password = "{PUT_YOUR_WIFI_NETWORK_PASSWORD_HERE}";

const int DHT_PIN = D5;
SimpleDHT11 dht11(DHT_PIN);

// Create object representing data collector for a particular sensor.
// Each sensor requires separate object.
RemoteDataCollector temp_collector(
  /*producer_key=*/"{PUT_YOUR_KEY_HERE}",
  /*sensor_name=*/"temperature",
  /*server_addr=*/"{PUT_SERVER_ADDRESS_HERE}");

RemoteDataCollector humidity_collector(
  /*producer_key=*/"{PUT_YOUR_KEY_HERE}",
  /*sensor_name=*/"humidity",
  /*server_addr=*/"{PUT_SERVER_ADDRESS_HERE}");

void setup() {
  Serial.begin(115200);
  Serial.println("-----\n\n\n\n");
  // List what networks are available.
  LogAvailableNetworks();
  // Try to connect to the network.
  SetupWiFiClient(ssid, password);
}

void loop() {
  Serial.println(F("Sampling DHT11"));

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  err = dht11.read(&temperature, &humidity, NULL);

  if (err != SimpleDHTErrSuccess) {
    Serial.print(F("Read DHT11 failed, err=")); Serial.print(SimpleDHTErrCode(err));
    Serial.print(F(",")); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  Serial.print(F("Success!\n"));

  temp_collector.LogInteger((int)temperature);
  humidity_collector.LogFloat((int)humidity);
  delay(5000);
}
