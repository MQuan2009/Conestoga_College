from picamera2 import Picamera2
from pyzbar.pyzbar import decode
import time

def scan_qr_code():
    picam2 = Picamera2()
    picam2.preview_configuration.main.size = (320, 240)  # Lower resolution
    picam2.preview_configuration.main.format = "RGB888"
    picam2.configure("preview")
    picam2.start()

    print("Starting continuous QR code scan at low resolution. Waiting for QR code...")

    try:
        while True:
            frame = picam2.capture_array()
            qr_codes = decode(frame)
            if qr_codes:
                data = qr_codes[0].data.decode('utf-8')
                print("QR Code Detected:", data)
                return data
            time.sleep(0.2)
    except KeyboardInterrupt:
        print("QR code scanning stopped by user.")
        return None
    finally:
        picam2.stop()

if __name__ == "__main__":
    result = scan_qr_code()
    if result:
        print("Scanned QR Code:", result)
s
