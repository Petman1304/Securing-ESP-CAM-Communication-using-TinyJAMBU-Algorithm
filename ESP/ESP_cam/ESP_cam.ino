#include <Arduino.h>
#include <string.h>
#include "tinyjambu_crypto_aead.h"
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

#define KEY_SIZE 16
#define NONCE_SIZE 16
#define TAG_SIZE 16
#define BLOCK_SIZE 64


#define KEYBYTES 32
#define NONCEBYTES 12
#define TAGBYTES 8

#define FILE_PHOTO "/photo.jpg"

// OV2640 camera module pins (CAMERA_MODEL_AI_THINKER)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

// Replace with your network credentials
const char* ssid = "Edulag";
const char* password = "98765432";



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    ESP.restart();
  } else {
    delay(500);
    Serial.println("SPIFFS mounted successfully");
  }

  // Turn-off the 'brownout detector'
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  // OV2640 camera module
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }
  // Camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    ESP.restart();
  } else {
    Serial.printf("Camera init success!");
  }

  
  capturePhotoSaveSpiffs();

  unsigned char key[KEY_SIZE] = { 0 };      // use a random key instead of all zeros
  unsigned char nonce[NONCEBYTES] = { 0 };  // use a random nonce instead of all zeros
  unsigned char ad[] = { 0 };               // no associated data
  unsigned long adlen = 0;
  unsigned char* plaintext = read_image();
  Serial.println("DEbug 6");
  unsigned long long plaintext_len = strlen((char*)plaintext);
  Serial.println("DEbug 7");
  unsigned char* ciphertext = new unsigned char[plaintext_len];
  Serial.println("DEbug 8");
  unsigned long long ciphertext_len = 0;
  unsigned char tag[TAGBYTES] = { 0 };
  
  //Serial.printf("\n%s \n", plaintext);
  Serial.printf("Image converted to string successfully\n");

  int ret = crypto_aead_encrypt(ciphertext, &ciphertext_len,
                                plaintext, plaintext_len,
                                ad, adlen,
                                NULL,  // no additional authenticated data
                                nonce,
                                key);

  if (ret != 0) {
    Serial.printf("Encryption failed\n");
  }
  //delete[] plaintext;
  free(plaintext);

  Serial.printf("Encrypt success\n");
  //Serial.printf("%s\n", ciphertext);

  Serial.println("Debug decrypt-1");
  unsigned char* decrypt = new unsigned char[ciphertext_len];
  Serial.println("Debug decrypt0");
  int inv = crypto_aead_decrypt(decrypt, &plaintext_len, NULL, ciphertext, ciphertext_len, ad, adlen, nonce, key);
  Serial.println("Debug decrypt1");

  if (inv != 0) {
    Serial.printf("Decrypt failed \n");
    Serial.println(inv);
    exit(1);
  }
  //delete[] ciphertext;
  free(ciphertext);
  Serial.printf("Decrypt success\n");
  Serial.println(reinterpret_cast<char*>(decrypt));
  //Serial.printf("\n%d", sizeof(decrypt));
}

void loop() {

  // put your main code here, to run repeatedly:
  delay(1);  // this speeds up the simulation
}




