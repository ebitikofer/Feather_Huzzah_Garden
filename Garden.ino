/***************************************************
  Adafruit ESP8266 Sensor Module
  
  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino
  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

// Libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

// DHT 11 sensor
#define DHTPIN 13
#define DHTTYPE DHT22

// Light sensor

// Soil Moisture sensor

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "e_bitikofer"
#define AIO_KEY         "219b4afd0a80433198b614dfdcb8c521"

// WiFi parameters
char ssid[] = "Lil Wayne's World";
char pass[] = "trukfit69";

// DHT sensor
DHT dht(DHTPIN, DHTTYPE, 15);

// Functions
void connect();

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
const char MQTT_SERVER[] = AIO_SERVER;

// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] = AIO_KEY __DATE__ __TIME__;
const char MQTT_USERNAME[] = AIO_USERNAME;
const char MQTT_PASSWORD[] = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);/****************************** Feeds ***************************************/

// Setup feeds for temperature & humidity
const char TEMPERATURE_FEED[] = AIO_USERNAME "/feeds/temperature";
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, TEMPERATURE_FEED);

const char HUMIDITY_FEED[] = AIO_USERNAME "/feeds/humidity";
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, HUMIDITY_FEED);

const char LIGHT_FEED[] = AIO_USERNAME "/feeds/light";
Adafruit_MQTT_Publish light = Adafruit_MQTT_Publish(&mqtt, LIGHT_FEED);

const char MOISTURE_FEED[] = AIO_USERNAME "/feeds/moisture";
Adafruit_MQTT_Publish moisture = Adafruit_MQTT_Publish(&mqtt, MOISTURE_FEED);

/*************************** Sketch Code ************************************/

void setup() {

//  WiFi.setPins(8,7,4,2);

  // Init sensor
  dht.begin();

  Serial.begin(115200);
  Serial.println(F("Adafruit IO Example"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());

  // connect to adafruit io
  connect();

}

void loop() {

  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }

  // Grab the current state of the sensor
  //uint32_t humidity_data = (int)dht.readHumidity();
  //uint32_t temperature_data = (int)dht.readTemperature();
  //uint32_t light_data = analogRead(A0);
  //uint32_t moisture_data = analogRead(A1);

  

  uint32_t humidity_data = 54;
  uint32_t temperature_data = 19;
  uint32_t light_data = 78;
  uint32_t moisture_data = 762;

  if(random(300)%7 == 0){
  humidity_data--;
  temperature_data++;
  }

  if(random(300)%11 == 0){
  moisture_data--;
  }

  if(random(300)%8 == 0){
  humidity_data++;
  temperature_data--;
  }

  Serial.println(moisture_data);

  // Publish data
  if (! temperature.publish(temperature_data))
    Serial.println(F("Failed to publish temperature"));
  else
    Serial.println(F("Temperature published!"));

  if (! humidity.publish(humidity_data))
    Serial.println(F("Failed to publish humidity"));
  else
    Serial.println(F("Humidity published!"));

  if (! light.publish(light_data))
    Serial.println(F("Failed to publish light"));
  else
    Serial.println(F("Light published!"));

  if (! moisture.publish(moisture_data))
    Serial.println(F("Failed to publish moisture"));
  else
    Serial.println(F("Moisture published!"));

  // Repeat every 10 seconds
  delay(10000);

}

// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}
