import spidev
import time

spi = spidev.SpiDev()
spi.open(0, 0)           # SPI0, CE0
spi.max_speed_hz = 500000
spi.mode = 0b00

# Optional: Wait for STM32 to initialize
time.sleep(1)

while True:
    message = "Hello STM32"
    # Pad to 16 bytes
    tx_data = [ord(c) for c in message.ljust(16)]

    # Send 16 bytes and receive 16 bytes in return
    rx_data = spi.xfer2(tx_data)

    # Convert received bytes to string
    response = bytes(rx_data).decode('utf-8', errors='ignore')
    print("Received from STM32:", response)

    time.sleep(1)

