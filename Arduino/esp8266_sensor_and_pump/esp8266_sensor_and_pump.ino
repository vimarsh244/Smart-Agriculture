#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
boolean state = false;
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "ssid"
#define WLAN_PASS       "pass"

/************************* Adafruit.io Setup *********************************/

#define IO_SERVER      "server ip"
#define IO_SERVERPORT  1887                   // use 8883 for SSL

#define USER ""
#define PSK ""
#define Pump D1 //pump connected to pin
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, USER, PSK);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish MoiR = Adafruit_MQTT_Publish(&mqtt, "SA/Soil");
Adafruit_MQTT_Publish ps = Adafruit_MQTT_Publish(&mqtt, "SA/ps");

// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Subscribe threshold = Adafruit_MQTT_Subscribe(&mqtt, "SA/Slevel");

/*************************** Sketch Code ************************************/
int level = 0;
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
//void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  pinMode(Pump, OUTPUT);
  digitalWrite(Pump, LOW);
  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&threshold);
  state = false;
}

uint32_t x = 0;

void loop() {
  MQTT_connect();

  // this is our 'wait for incoming subscription packets' busy subloop
  // try to spend your time here
  if (state == false || level == 0) {
    Serial.println("Trying to receive value");
    Adafruit_MQTT_Subscribe *subscription;
    while ((subscription = mqtt.readSubscription(5000))) {
      if (subscription == &threshold) {
        Serial.print(F("Got: "));
        level = atoi((char *)threshold.lastread);
        Serial.println(level);
      }
    }
    state = true;
  }
  //mqtt.disconnect();

  x = analogRead(A0);
  // Now we can publish stuff!
  Serial.print(F("\nSending Soil Moisture val "));
  Serial.print(x);
  Serial.print("...");
  if (! MoiR.publish(x)) {
    Serial.println(F("Failed"));
  } else {
    Serial.println(F("OK!"));
  }
  if (x > level) {
    digitalWrite(Pump, HIGH);
    Serial.println("Pump On");
    ps.publish(1);
  }
  else {
    digitalWrite(Pump, LOW);
    Serial.println("Pump Off");
    ps.publish(0);
  }

  mqtt.disconnect();
  delay(5000);
}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
