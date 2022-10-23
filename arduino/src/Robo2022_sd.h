#ifndef ROBO_2022
#define ROBO_2022

// Ignore if this is an ESP8266-based board
#ifndef ESP8266

// Initializes the SD card. Hangs forever if the initialization failed.
void InitializeSDCard();

// Class for managing the sensor sample logging.
class LocalDataCollector {
 public:
  LocalDataCollector(const char* sensor_name);

  bool LogInteger(int i);
  bool LogFloat(float f);

 private:
  bool LogSample(const char* params);

  const char* sensor_name;
};

#endif  // ifndef ESP8266

#endif
