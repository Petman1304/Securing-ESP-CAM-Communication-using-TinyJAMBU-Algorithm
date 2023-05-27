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
#include <WiFiClient.h>

#define KEY_SIZE 16
#define NONCE_SIZE 16
#define TAG_SIZE 16
#define BLOCK_SIZE 64


#define KEYBYTES 32
#define NONCEBYTES 12
#define TAGBYTES 8

//WiFiClient espClient;
//PubSubClient client(espClient);
//unsigned long lastMsg = 0;
//#define MSG_BUFFER_SIZE	(50)
//char msg[MSG_BUFFER_SIZE];
//int value = 0;

const char* ssid = "Edulag";
const char* password = "98765432";
//const char* mqtt_server = "broker.mqtt-dashboard.com";

// Port to listen for incoming connections
const int serverPort = 8888;

WiFiServer server(serverPort);
WiFiClient client;

unsigned char key[KEY_SIZE] = "pmckelompok5123";      // use a random key instead of all zeros
unsigned char nonce[NONCEBYTES] = "pmckelompk5";  // use a random nonce instead of all zeros
unsigned char ad[] = { 0 };               // no associated data
unsigned long adlen = 0;

//const int MAX_DATA_SIZE = 15360;
//const int MAX_CHUNK_SIZE = 256;// Chunk size for publishing

//unsigned char receivedData[MAX_DATA_SIZE];  // Variable to hold the complete data
//int receivedSize = 0;  // Size of the received data (incremented as chunks arrive)




//void callback(char* topic, byte* payload, unsigned int length) {
//  // Handle incoming MQTT messages
//  if (strcmp(topic, "topic/esp32/header") == 0) {
//    // Parse the header message to get total length and current position
//    String header = String((char*)payload, length);
//    int commaIdx = header.indexOf(',');
//    int totalLength = header.substring(0, commaIdx).toInt();
//    int currentPosition = header.substring(commaIdx + 1).toInt();
//
//    // If it's the first chunk, reset the received data buffer
//    if (currentPosition == 0) {
//      receivedSize = 0;
//    }
//
//    // Check if the received data exceeds the maximum expected size
//    if (receivedSize + length - header.length() > MAX_DATA_SIZE) {
//      Serial.println("Data size exceeds the maximum expected size");
//      return;
//    }
//
//    // Copy the received data to the buffer
//    memcpy(&receivedData[receivedSize], payload + header.length(), length - header.length());
//    receivedSize += length - header.length();
//
//    // If it's the last chunk, the complete data has been received
//    if (currentPosition == totalLength - 1) {
//      Serial.println("Complete data received");
//      // Use the receivedData buffer containing the complete data
//      // Perform any necessary processing or handling of the data here
//      Serial.printf("%s", receivedData);
//      
//      unsigned long long plaintext_len = 0;
//      unsigned char* decrypt = (unsigned char*) malloc (receivedSize);
//      int inv = crypto_aead_decrypt(decrypt, &plaintext_len, NULL, receivedData, receivedSize, ad, adlen, nonce, key);
//  
//       if (inv != 0) {
//         Serial.printf("Decrypt failed \n");
//         Serial.println(inv);
//         //free(ciphertext);
//         free(decrypt);
//         return;
//       }
//    
//       Serial.printf("%s", decrypt);
//    }
//  }
//}


  // Dekripsi ciphertext dan mencetak decrypted_plaintext sebagai string
  // unsigned char* decrypt = (unsigned char*) malloc (ciphertext_len);
  // int inv = crypto_aead_decrypt(decrypt, &plaintext_len, NULL, ciphertext, ciphertext_len, ad, adlen, nonce, key);
  
  // if (inv != 0) {
  //   Serial.printf("Decrypt failed \n");
  //   Serial.println(inv);
  //   free(ciphertext);
  //   free(decrypt);
  //   return;
  // }

  // Serial.printf("%s", decrypt);
//}

//void reconnect() {
//  // Loop until we're reconnected
//  while (!client.connected()) {
//    Serial.print("Attempting MQTT connection...");
//    // Create a random client ID
//    String clientId = "ESP8266Client-";
//    clientId += String(random(0xffff), HEX);
//    // Attempt to connect
//    if (client.connect(clientId.c_str())) {
//      Serial.println("connected");
////      // Once connected, publish an announcement...
////      client.publish("peemce", "hi steve");
////      // ... and resubscribe
////      client.subscribe("peemce");
//    } else {
//      Serial.print("failed, rc=");
//      Serial.print(client.state());
//      Serial.println(" try again in 5 seconds");
//      // Wait 5 seconds before retrying
//      delay(5000);
//    }
//  }
//}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  
//  client.setServer(mqtt_server, 1883);
//  client.setCallback(callback);

//  client.subscribe("topic/PMC_KEL5/header");
//  client.subscribe("topic/PMC_KEL5/data");

 // Start the TCP server
  //WiFiServer server(serverPort);
  server.begin();
  
  Serial.println("Server started");
  Serial.print("IP Addr :");
  Serial.println(WiFi.localIP());
  
  

  // Dekripsi ciphertext dan mencetak decrypted_plaintext sebagai string
  // unsigned char* decrypt = (unsigned char*) malloc (ciphertext_len);
  // int inv = crypto_aead_decrypt(decrypt, &plaintext_len, NULL, ciphertext, ciphertext_len, ad, adlen, nonce, key);
  
  // if (inv != 0) {
  //   Serial.printf("Decrypt failed \n");
  //   Serial.println(inv);
  //   free(ciphertext);
  //   free(decrypt);
  //   return;
  // }

  // Serial.printf("%s", decrypt);
  

  //client.stop();

  

}

void loop() {
if (!client.connected()) {
    client = server.available();
    if (client) {
      Serial.println("Client connected");
      
      // Receive the array size
      unsigned long long ciphertext_len;
      Serial.println("debug1");
      if (client.available() >= sizeof(ciphertext_len)) {
        Serial.println("debug2");
        client.readBytes((char*)&ciphertext_len, sizeof(ciphertext_len));
        Serial.println("debug3");
        // Create a buffer to hold the received data
        unsigned char* ciphertext = new unsigned char[ciphertext_len];
        Serial.println("debug4");
        // Receive the array data
        Serial.println("debug5");
          client.readBytes((char*)ciphertext, ciphertext_len);
          Serial.println("debug6");
          // Process the received array data
//          for (int i = 0; i < ciphertext_len; i++) {
//            Serial.print(ciphertext[i]);
//            Serial.print(" ");
//          }
          Serial.printf("%s", ciphertext);
          Serial.printf("%d\n", ciphertext_len);
          Serial.println();

          // Dekripsi ciphertext dan mencetak decrypted_plaintext sebagai string
  unsigned long long plaintext_len = ciphertext_len - TAGBYTES;
  unsigned char* decrypt = (unsigned char*) malloc (plaintext_len);
  
  int inv = crypto_aead_decrypt(decrypt, &plaintext_len, NULL, ciphertext, ciphertext_len, ad, adlen, nonce, key);
  
  if (inv != 0) {
     Serial.printf("Decrypt failed \n");
     Serial.println(inv);
     free(ciphertext);
     free(decrypt);
     return;
   }
   decrypt[plaintext_len]= (unsigned char)'\0';

   Serial.printf("%s", decrypt);
          
          // Clean up the dynamically allocated memory
          delete[] ciphertext;
        
//        if (client.available() >= ciphertext_len) {
//          Serial.println("debug5");
//          client.readBytes((char*)ciphertext, ciphertext_len);
//          Serial.println("debug6");
//          // Process the received array data
////          for (int i = 0; i < ciphertext_len; i++) {
////            Serial.print(ciphertext[i]);
////            Serial.print(" ");
////          }
//          Serial.printf("%s", ciphertext);
//          Serial.println();
//          
//          // Clean up the dynamically allocated memory
//          delete[] ciphertext;
//        }
      }
    }
  }
  
  delay(1);
}
