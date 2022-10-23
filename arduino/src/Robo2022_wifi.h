#ifndef ROBO_2022_WIFI
#define ROBO_2022_WIFI

// Only for ESP8266-based board
#ifdef ESP8266

// Makes a connection to a particular WiFi network.
void SetupWiFiClient(const char* ssid, const char* password);

// Scans for available WiFi networks and prints information on serial output.
// Useful for debugging network connections (in particular, their availability).
void LogAvailableNetworks();

// Class for managing the sensor sample logging.
class RemoteDataCollector {
 public:
  RemoteDataCollector(const char* producer_key, const char* sensor_name, const char* server_addr);

  bool LogInteger(int i);
  bool LogFloat(float f);

 private:
  bool LogSample(const char* params);

  const char* producer_key;
  const char* sensor_name;
  const char* server_addr;
};

#endif // ifdef ESP8266

#endif
