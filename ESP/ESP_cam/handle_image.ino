unsigned char* read_image(void){
  File file = SPIFFS.open(FILE_PHOTO, "r");
  if (!file) {
    Serial.println("Failed to open file");
    exit(1);
  }

  size_t fileSize = file.size();
  std::unique_ptr<uint8_t[]> imageData(new uint8_t[fileSize]);
  if (file.readBytes(reinterpret_cast<char*>(imageData.get()), fileSize) != fileSize) {
    Serial.println("Error reading file");
    exit(2);
  }
  file.close();

  String base64Image = base64::encode(imageData.get(), fileSize);
  unsigned char* buffer = new unsigned char[base64Image.length() + 1];
  Serial.println("DEbug 1");

  // Serial.println("Base64 Image:");
  // Serial.println(base64Image);
  
  Serial.println("DEbug 2");
  // Copy the string to the unsigned char buffer
  mystrlcpy((char*)buffer, base64Image.c_str(), fileSize);

  return(buffer);
}

void mystrlcpy(char* dst, const char* src, size_t size) {
  size_t srcLen = strlen(src);
  Serial.println(srcLen);
  //size_t copyLen = srcLen >= size ? size - 1 : srcLen;
  //Serial.println(copyLen);
  memcpy(dst, src, srcLen);
  Serial.println("DEbug 5");
  dst[srcLen] = '\0';
}
