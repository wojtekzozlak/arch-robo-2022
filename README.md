# ROBOStudio 2022

Logging libraries for the ROBOStudio 2022 course.

## Installation

TODO

## What's included?

Package contains two libraries:

- `Robo2022_sd.h` for writing logs to the SD card using a standard Arduino
  board.
- `Robot2022_wifi.h` for logging to a remote server using WiFi. The library
  requires usage of the ESP8266-based board with WiFi module (e.g. NodeMCU).

You can find example code in the `arduino/examples` subdirectory.

### Logging to SD card

The SD-card logging library relies on the standard `SD.h` from Arudino core,
as well as the `Grove DS1307` real time clock.

#### RTC setup

The clock uses the `I2C` protocol. `I2C` typically has a hardware support on the
board. In case of *Arudino UNO* it means that [`SCL` and `SDA` pins are located
right past the `13`, `GND` and `AREF` pins](
https://docs.arduino.cc/static/5fa18e7fb884a8f6381a54dae4d87551/29114/uno-i2c.png)).
If you have more `I2C` devices (e.g. other sensors) just connect them to the
same pins in parallel.

The RTC module has to be initialized

#### Card reader setup

The SD Card reader uses the `SPI` protocol. In Arduino Uno the `SPI` ports are
following:
- `SCK` - 13
- `MISO` - 12
- `MOSI` - 11
- `CS` (referred to `SS` in the past) - 10 by default

If you have more `SPI` devices, you connect the `SCK` / `MISO` / `MOSI` pins in
parallel. However, each device must have its `CS` connected to a separate pin
(meaning that only one device can use a default of pin `10` and the rest must
specify it explicitly.

Important: In addition to the wiring, the SD card must be initialized in the
`setup()` function by calling the `InitializeSDCard()` function. For example:

```
void setup() {
  // Open serial communications.
  Serial.begin(9600);
  // Initialize card read.
  InitializeSDCard();
}
```

#### Logging samples

In order to actually log sample, one must first create an instance of the
`LocalDataCollector` class. The string passed indicates the "name" of the sensor
for which the data is logged:

```
LocalDataCollector temp_collector("temperature");
```

afterwards, it is a matter of calling either `LogInteger` or `LogFloat` method of the object:

```
temp_collector.LogInteger(5);
temp_collector.LogFloat(3.21);
```

#### Data format

Samples are written to the `CSV` files, with names denoting current (from the
RTC module perspective) date. Each line is in form of either:
- `{date_time},{sensor_name},{integer},`
or
- `{date_time},{sensor_name},,float`

meaning that integer and float values uses separate CSV columns.

When dealing with multiple sensors, the easiest way to separate samples is to
load file into e.g. Excel and sort by sensor name.


### Logging via WiFi

The WiFi logging library relies on the ESP8266 built-in WiFi support and the
existence of a remote, HTTP server to consume the data (the `server`
subdirectory). There is no additional physical setup of the board required.

#### Producers and sensors

Data collection server has a concept of `producers` and `sensors`:

- `producer` is an identity which owns potentially multiple `sensors`
- `sensor` is a source of data, which groups multiple data samples.

Producer is authorized using a `key`, which should be only known to the producer
and the sensor. Without a proper key in the request, sensor is unable to
recognize the producer and returns `403` error.

Producers can be managed in the web interface of the server, after logging
in as an admin.

#### Program setup

In order to log data, you must know:

- `SSID` and password of you WiFi network.
- IP of the data collection server.
- A valid producer `key`.

Library exposes a helper function for connecting to the WiFi -
`SetupWiFiClient()`.

#### Logging a sample

Logging a sample requires first creating an instance of the
`RemoteDataCollector` class. The constructor takes a key, name of the sensor
(server will automatically add a new sensor if name is not recognized)
and address of the server:

```
RemoteDataCollector humidity_collector(
  /*producer_key=*/"{PUT_YOUR_KEY_HERE}",
  /*sensor_name=*/"humidity",
  /*server_addr=*/"{PUT_SERVER_ADDRESS_HERE}");
```

Afterwards, it is a matter of calling the `LogInteger` or `LogFloat` method,
similarly to the SD card library.

Check the examples directory for more details.
