//#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>

#define PIN 12
#define NUM_LEDS 192
#define BRIGHTNESS 255

WiFiClient espClient;
PubSubClient client(espClient);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);


const char* ssid     = "sybren";
const char* password = "1111111111";
const char* mqtt_server = "192.168.11.6";
const char* TOPIC1 = "sybren/bed";


//############################################### SETUP ###############################################

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(12, OUTPUT);
  strip.show();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(TOPIC1, "hello world");
      // ... and resubscribe
      client.subscribe(TOPIC1);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//############################################### CALLBACK ###############################################

void callback(char* p_topic, byte* p_payload, unsigned int p_length) {
  // concat the payload into a string
  String payload;

  for (uint8_t i = 0; i < p_length; i++) {
    payload.concat((char)p_payload[i]);
  }
  Serial.println(payload);

  if (payload.equals(String("Off"))) {
    colorWipe(strip.Color(0, 0, 0), 0);
  }
  else if (payload.equals(String("On"))) {
    colorWipe(strip.Color(255, 255, 255, 255), 0);
  }
  else if (payload.equals(String("red"))) {
    colorWipe(strip.Color(247, 64, 64), 0);
  }
  else if (payload.equals(String("groen"))) {
    colorWipe(strip.Color(167, 247, 64), 0);
  }
  else if (payload.equals(String("blauw"))) {
    colorWipe(strip.Color(64, 247, 237), 0);
  }
  else if (payload.equals(String("paars"))) {
    colorWipe(strip.Color(121, 94, 255), 0);
  }
  else if (payload.equals(String("lichtgroen"))) {
    colorWipe(strip.Color(94, 255, 190), 0);
  }
  else if (payload.equals(String("geel"))) {
    colorWipe(strip.Color(249, 255, 94), 0);
  }
  else if (payload.equals(String("green"))) {
    colorWipe(strip.Color(255, 0, 0, 0), 0);
  }

  else if (payload.equals(String("sleep"))) {
    sleep(strip.Color(255, 255, 255, 255), 0);
  }

}

//############################################### functies ###############################################

void colorWipe(uint32_t c, uint8_t wait) {
  strip.show();
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.setBrightness(BRIGHTNESS);
    strip.show();
    delay(wait);
  }
}
void sleep(uint32_t c, uint8_t wait) {
  strip.show();
  for (uint16_t i = 0; i < strip.numPixels(); i++) {

    strip.show();
    strip.setPixelColor(i * 10, c);
    strip.setBrightness(1);
    strip.show();
    delay(wait);
  }
}

