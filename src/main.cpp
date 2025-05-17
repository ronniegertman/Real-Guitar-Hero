#include <Arduino.h>
#include <ESP8266WiFi.h>   // ✅ ESP8266-compatible
#include <WiFiUdp.h>

void serial_print(int16_t sample);

// const char* ssid = "Dira7ShelHasmachot";
// const char* password = "TechnionIsFun7";
// const char* ssid = "pragnet";
// const char* password = "0542560198";
const char* ssid = "weefy";
const char* password = "weefy123456";

WiFiUDP udp;
const int udpPort = 1234;

const int bufferSize = 1024;
char udpBuffer[bufferSize];

// Threshold for detecting a clap (tune this)
const int16_t CLAP_THRESHOLD = 5000;

void setup() {
  Serial.begin(9600);

  // Start WiFi connection
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Start UDP on port 1234
  udp.begin(1234);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (packetSize > 0 && packetSize <= bufferSize) {
    udp.read(udpBuffer, packetSize);
    
    // Treat buffer as 16-bit PCM samples
    int16_t* samples = (int16_t*)udpBuffer;
    int numSamples = packetSize / 2;

    for (int i = 0; i < numSamples; i++) {
      int16_t sample = samples[i];
      // if (abs(sample) > CLAP_THRESHOLD) {
      //   Serial.println("👏 Clap detected!");
      //   delay(300);  // Basic debounce
      //   break;
      // }
      if( i % 16 == 0){
        serial_print(sample);
      }
    }
  }
}

void serial_print(int16_t sample) {
  Serial.printf("%d\n", sample);
}


