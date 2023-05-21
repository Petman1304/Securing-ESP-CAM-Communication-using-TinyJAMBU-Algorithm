#include <Arduino.h>
#include <string.h>
#include "tinyjambu_crypto_aead.h"
#include <PubSubClient.h>
//#include "read_image.c"
#include <SPIFFS.h>
#include "WiFi.h"
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <FS.h>
#include "Base64.h"
#include <stdio.h>
#include <stdlib.h>
#include <Wire.h>

#define KEY_SIZE 16
#define NONCE_SIZE 16
#define TAG_SIZE 16
#define BLOCK_SIZE 64


#define KEYBYTES 32
#define NONCEBYTES 12
#define TAGBYTES 8

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

const char* ssid = "Edulag";
const char* password = "98765432";
const char* mqtt_server = "broker.mqtt-dashboard.com";

unsigned char key[KEY_SIZE] = { 0 };      // use a random key instead of all zeros
unsigned char nonce[NONCEBYTES] = { 0 };  // use a random nonce instead of all zeros
unsigned char ad[] = { 0 };               // no associated data
unsigned long adlen = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  unsigned long long ciphertext_len = length;
  unsigned char* ciphertext = (unsigned char*) malloc (ciphertext_len);

  unsigned long long plaintext_len = 0;

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  // Mengisi array ciphertext dengan data dari payload
  for (int i = 0; i < length; i += 2) {
    char hex[3] = {payload[i], payload[i+1], '\0'};
    unsigned char value = strtol(hex, NULL, 16);
    ciphertext[i/2] = value;
  }

  // Dekripsi ciphertext dan mencetak decrypted_plaintext sebagai string
  unsigned char* decrypt = (unsigned char*) malloc (ciphertext_len);
  int inv = crypto_aead_decrypt(decrypt, &plaintext_len, NULL, ciphertext, ciphertext_len, ad, adlen, nonce, key);
  
  if (inv != 0) {
    Serial.printf("Decrypt failed \n");
    Serial.println(inv);
    free(ciphertext);
    free(decrypt);
    return;
  }

  Serial.printf("%s", decrypt);
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("peemce", "hi steve");
      // ... and resubscribe
      client.subscribe("peemce");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);



}

void loop() {
  // put your main code here, to run repeatedly:
    if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
