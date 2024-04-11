#include "Arducam_Mega.h"
// Ensure you have included all necessary libraries and that Arducam_Mega.h is available in your environment
#define RXD 3  //RXD
#define TXD 1  //TXD

const int CS = 17; // Chip select pin for the ArduCAM
Arducam_Mega myCAM(CS);
  

void setup() {
  Serial.begin(115200); 
  myCAM.begin(); 
  Serial2.begin(9600, SERIAL_8N1, RXD, TXD); 
}

void loop() {
  // if (Serial2.available()) {
  // String command = Serial2.readStringUntil('\n'); 
  // if (command == "CAPTURE") {
  // Start capturing an image
  myCAM.takePicture(CAM_IMAGE_MODE_QVGA, CAM_IMAGE_PIX_FMT_JPG); // Adjust parameters as needed

  uint32_t totalLength = myCAM.getTotalLength();
  uint32_t readLength = 0;
  const uint8_t bufferSize = 255; 
  uint8_t buffer[bufferSize];

  while (readLength < totalLength) {
    uint8_t lengthToRead = min(totalLength - readLength, (uint32_t)bufferSize);
    uint8_t actualRead = myCAM.readBuff(buffer, lengthToRead);

    // Transmit the buffer via Serial
    Serial.write(buffer, actualRead);

    readLength += actualRead;
  }

  // Signal the end of the image data transmission
  Serial.println(); // New line to separate images
  Serial.flush(); 
  delay(10000);
  // }
  // }
}
