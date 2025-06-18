from QR_Read import scan_qr_code
from AWSIoT import publish_to_aws

def main():
    qr_data = scan_qr_code()
    if qr_data:
        publish_to_aws(qr_data)
    else:
        print("No QR code found; nothing published.")

if __name__ == "__main__":
    main()

