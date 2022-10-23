#include "Robo2022_wifi.h"

// Only for ESP8266-based board
#ifdef ESP8266

#include <Arduino.h>
#include <cstdio>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>


void LogAvailableNetworks() {
  String ssid;
  int32_t rssi;
  uint8_t encryptionType;
  uint8_t* bssid;
  int32_t channel;
  bool isHidden;

  Serial.println("[LogAvailableNetworks] Scanning WIFI networks");
  int scanResult = WiFi.scanNetworks(/*async=*/false, /*hidden=*/false);

  if (scanResult == 0) {
    Serial.println("[LogAvailableNetworks] No networks found");
  } else if (scanResult > 0) {
    Serial.printf("[LogAvailableNetworks] %d networks found:\n", scanResult);

    for (int8_t i = 0; i < scanResult; i++) {
      WiFi.getNetworkInfo(i, ssid, encryptionType, rssi, bssid, channel, isHidden);
      Serial.printf("[LogAvailableNetworks] %02d: [CH %02d] %ddBm %s %s\n", i, channel, rssi, (encryptionType == ENC_TYPE_NONE) ? ' ' : 'encrypted', ssid.c_str());
      yield();
    }
  } else {
    Serial.printf("[LogAvailableNetworks] WiFi scan error %d", scanResult);
  }
}


void SetupWiFiClient(const char* ssid, const char* password) {
  // Set mode to just being a client.
  WiFi.mode(WIFI_STA);
  Serial.print("[SetupWiFiClient] Connecting to WiFi network");
  WiFi.begin(ssid, password);

  // Wait until connection is made.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("[SetupWiFiClient] WiFi connected");
  Serial.print("[SetupWiFiClient] IP address: ");
  Serial.println(WiFi.localIP());
}


RemoteDataCollector::RemoteDataCollector(const char* producer_key, const char* sensor_name, const char* server_addr)
  : producer_key(producer_key), sensor_name(sensor_name), server_addr(server_addr) {}

bool RemoteDataCollector::LogSample(const char* params) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[RemoteDataCollector] WiFi network not connected - dropping sample.");
  }

  char buffer[1024];
  sprintf(buffer, "http://%s/sensors/add_sample?key=%s&name=%s&%s", server_addr, producer_key, sensor_name, params);

  Serial.println("[RemoteDataCollector] Making a request to the samples server.");
  WiFiClient client;
  HTTPClient http;
  http.begin(client, buffer);
  int httpCode = http.GET();
  String payload = http.getString();
  http.end();

  if (httpCode == 200) {
    Serial.println("[RemoteDataCollector] Success!");
  } else {
    Serial.println("[RemoteDataCollector] Server returned error:");
    Serial.println(payload);
  }
}

bool RemoteDataCollector::LogInteger(int i) {
  char buffer[64];
  sprintf(buffer, "int=%d", i);
  Serial.printf("[RemoteDataCollector] Logging sample '%i' for sensor %s\n", i, sensor_name);
  return LogSample(buffer);
}

bool RemoteDataCollector::LogFloat(float f) {
  char buffer[64];
  sprintf(buffer, "float=%f", f);
  Serial.printf("[RemoteDataCollector] Logging sample '%f' for sensor %s\n", f, sensor_name);
  return LogSample(buffer);
}

#endif // ifdef ESP8266
