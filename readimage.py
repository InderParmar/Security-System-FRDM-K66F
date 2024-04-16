import serial
import time
import datetime


# Open the serial port
ser = serial.Serial('COM4', 115200, timeout=1)  # Adjust 'COM_PORT' as necessary


def save_image(data, filename=None):
    if filename is None:
        # Use current time to create a unique filename
        timestamp = datetime.datetime.now().strftime("%Y-%m-%d_%H-%M-%S")
        filename = f"image_{timestamp}.jpg"
    with open(filename, 'wb') as f:
        f.write(data)


def read_image():
    image_data = bytearray()
    start_time = time.time()

    while True:
        if ser.in_waiting:
            byte = ser.read()
            image_data += byte

            # Reset the timer if data is being received
            start_time = time.time()
        elif time.time() - start_time > 2:
            # No data received for 2 seconds, assume image is complete
            break

    return image_data

if __name__ == '__main__':
    while True:
        print("Waiting for image data...")
        data = read_image()
        if data:
            print("Image received, saving...")
            save_image(data)
            print("Image saved.")
        time.sleep(5)  #
