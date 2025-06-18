from picamera2 import Picamera2
import cv2
from pyzbar.pyzbar import decode
import time

MAX_ATTEMPTS = 10

def capture_and_decode(picam2):
    frame = picam2.capture_array()
    qr_codes = decode(frame)
    if qr_codes:
        return qr_codes[0].data.decode('utf-8')
    return None

def main():
    picam2 = Picamera2()
    picam2.preview_configuration.main.size = (640, 480)
    picam2.preview_configuration.main.format = "RGB888"
    picam2.configure("preview")
    picam2.start()

    print("Starting QR code scan (up to 10 attempts)...")

    for attempt in range(1, MAX_ATTEMPTS + 1):
        print(f"Attempt {attempt}...")
        result = capture_and_decode(picam2)

        if result:
            print("? QR Code Detected:", result)
            return

        time.sleep(0.5)  # short delay before next try

    print("? Failed to detect QR code after 10 attempts.")

if __name__ == "__main__":
    main()

