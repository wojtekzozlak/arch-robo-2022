#include "Robo2022_sd.h"

#include <Arduino.h>
#include <SD.h>
#include <DS1307.h>

namespace {

DS1307 clock;

void GetTime(char* buffer) {
  clock.begin();
  clock.getTime();
  sprintf(buffer, "%04d/%02d/%02d %02d:%02d:%02d",
          (int)clock.year+2000, clock.month, clock.dayOfMonth,
          clock.hour, clock.minute, clock.second);

  return buffer;
}

size_t GetFileName(char* name) {
  clock.begin();
  clock.getTime();
  return sprintf(name, "%02d%02d%02d.csv", clock.month, clock.dayOfMonth, clock.hour);
}

}  // namespace

void InitializeSDCard() {
  Serial.print(F("Initializing SD card..."));
  if (!SD.begin()) {
    Serial.println(F("initialization failed!"));
    while (1);
  }
  Serial.println(F("initialization done."));
}

LocalDataCollector::LocalDataCollector(const char* sensor_name)
  : sensor_name(sensor_name) {}

bool LocalDataCollector::LogSample(const char* params) {
  char file_name[12];
  GetFileName(file_name);

  char date_time[32];
  GetTime(date_time);

  File f = SD.open(file_name, FILE_WRITE);
  if (!f) {
    Serial.println(F("[LocalDataCollector] Could not open the file on the SD card"));
    return false;
  }

  char buffer[128];
  size_t log_size = sprintf(buffer, "%s,%s,%s\n", date_time, sensor_name, params);
  size_t written = f.write(buffer, log_size);
  Serial.print(F("[LocalDataCollector] Written "));
  Serial.print(written);
  Serial.println(F(" bytes to the SD card."));
  f.close();
  return true;
}

bool LocalDataCollector::LogInteger(int i) {
  char buffer[32];
  sprintf(buffer, "%d,", i);
  return LogSample(buffer);
}

bool LocalDataCollector::LogFloat(float f) {
  char strf[16];
  char buffer[32];
  dtostrf(f, 1, 6, strf);
  sprintf(buffer, ",%s", strf);
  return LogSample(buffer);
}
