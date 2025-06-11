from picamera2 import Picamera2
from pyzbar.pyzbar import decode
import time

MAX_ATTEMPTS = 10

def scan_qr_code():
    picam2 = Picamera2()
    picam2.preview_configuration.main.size = (640, 480)
    picam2.preview_configuration.main.format = "RGB888"
    picam2.configure("preview")
    picam2.start()

    print("Starting QR code scan (up to 10 attempts)...")

    for attempt in range(1, MAX_ATTEMPTS + 1):
        print(f"Attempt {attempt}...")
        frame = picam2.capture_array()
        qr_codes = decode(frame)
        if qr_codes:
            data = qr_codes[0].data.decode('utf-8')
            print("QR Code Detected:", data)
            return data
        time.sleep(0.5)

    print("Failed to detect QR code.")
    return None
 
if __name__ == "__main__":
    scan_qr_code()
