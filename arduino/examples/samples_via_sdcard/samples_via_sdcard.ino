
#include <SPI.h>
#include <SD.h>

#include <Robo2022_sd.h>
#include <SimpleDHT.h>
#include <DS1307.h>

const int CS_PIN = 10;
const int DHT_PIN = 4;

File file;
SimpleDHT11 dht11(DHT_PIN);

DS1307 clock;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
//  while (!Serial) {
//    ; // wait for serial port to connect. Needed for native USB port only
//  }

  Serial.print(F("Initializing SD card..."));
  if (!SD.begin(CS_PIN)) {
    Serial.println(F("initialization failed!"));
    while (1);
  }
  Serial.println(F("initialization done."));
  clock.begin();
}


void loop() {
  Serial.println(F("Sample DHT11..."));
  
  // read without samples.
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  err = dht11.read(&temperature, &humidity, NULL);
  
  if (err != SimpleDHTErrSuccess) {
    Serial.print(F("Read DHT11 failed, err=")); Serial.print(SimpleDHTErrCode(err));
    Serial.print(F(",")); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  
  LocalDataCollector temp_collector("temperature");
  LocalDataCollector humidity_collector("humidity");

  temp_collector.LogInteger((int)temperature);
  humidity_collector.LogInteger((int)humidity);

  delay(1000 * 5);
}
