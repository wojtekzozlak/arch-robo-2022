#ifndef ROBO_2022
#define ROBO_2022

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

#endif
