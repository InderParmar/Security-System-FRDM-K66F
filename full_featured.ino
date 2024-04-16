#include <Arducam_Mega.h>


// Define the chip select pin for the Arducam
const int CS_PIN = 17;
Arducam_Mega myCAM(CS_PIN);

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  
  // Initialize the camera
  CamStatus status = myCAM.begin();
  if (status != CAM_ERR_SUCCESS) {
    Serial.println("Camera initialization failed!");
    while (1){  Serial.println("Camera initialization failed!");}; // Stop execution if camera initialization fails
  }
  Serial.println("Camera initialized successfully.");
}

void loop() {
  // Capture an image every 10 seconds
  delay(10000);
  
  // Take a picture
  CamStatus status = myCAM.takePicture(CAM_IMAGE_MODE_QVGA, CAM_IMAGE_PIX_FMT_JPG);
  if (status != CAM_ERR_SUCCESS) {
    Serial.println("Error capturing image");
    return;
  }

  // Read and transmit the image data
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
  
  // Indicate the end of image data transmission
  Serial.println(); // End the transmission with a newline for simplicity
}
