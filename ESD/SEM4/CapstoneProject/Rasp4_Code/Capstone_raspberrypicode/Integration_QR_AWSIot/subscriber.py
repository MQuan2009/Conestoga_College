# subscriber.py
import json
import time
import subprocess
from awscrt import io, mqtt
from awsiot import mqtt_connection_builder

# AWS IoT Config
endpoint = "aqrm5lymlussz-ats.iot.ca-central-1.amazonaws.com"
client_id = "ESP32_WiFi"  # Reused from publish script
subscribe_topic = "esp32/pub"
cert = "device.pem.crt"
private_key = "private.pem.key"
root_ca = "root-CA.crt"

# MQTT Setup
event_loop_group = io.EventLoopGroup(1)
host_resolver = io.DefaultHostResolver(event_loop_group)
client_bootstrap = io.ClientBootstrap(event_loop_group, host_resolver)

mqtt_connection = mqtt_connection_builder.mtls_from_path(
    endpoint=endpoint,
    cert_filepath=cert,
    pri_key_filepath=private_key,
    client_bootstrap=client_bootstrap,
    ca_filepath=root_ca,
    client_id=client_id,
    clean_session=False,
    keep_alive_secs=30
)

# Callback when message is received
def on_message_received(topic, payload, **kwargs):
    print(f"\nMessage received on topic '{topic}': {payload}")
    try:
        message = json.loads(payload)
        if message.get("status") == "item_ready_to_scan":
            print("Trigger received. Starting scanner_service.py...")
            subprocess.run(["python3", "ScannerService.py"])
    except Exception as e:
        print(f"Error handling message: {e}")

def main():
    print("Connecting to AWS IoT Core...")
    mqtt_connection.connect().result()
    print("Connected.")

    print(f"Subscribing to topic '{subscribe_topic}'...")
    subscribe_future, _ = mqtt_connection.subscribe(
    topic=subscribe_topic,
    qos=mqtt.QoS.AT_LEAST_ONCE,
    callback=on_message_received
    )
    subscribe_future.result()

    print("Waiting for messages...")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Disconnecting...")
        mqtt_connection.disconnect().result()
        print("Disconnected.")

if __name__ == "__main__":
    main()

