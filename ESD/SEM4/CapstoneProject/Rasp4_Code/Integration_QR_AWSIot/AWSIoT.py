import json
import time
from awscrt import io, mqtt, auth, http
from awsiot import mqtt_connection_builder

# AWS IoT Config
endpoint = "aqrm5lymlussz-ats.iot.ca-central-1.amazonaws.com"
client_id = "ESP32_WiFi"
topic = "esp32/sub"
cert = "device.pem.crt"
private_key = "private.pem.key"
root_ca = "root-CA.crt"

def publish_to_aws(qr_data):
    # Setup
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

    print(f"Connecting to AWS IoT at '{endpoint}'...")
    mqtt_connection.connect().result()
    print("Connected to AWS IoT")

    # Publish
    message = {
        "qr_data": qr_data,
        "timestamp": time.strftime("%Y-%m-%d %H:%M:%S")
    }
    payload = json.dumps(message, indent=2)

    mqtt_connection.publish(
        topic=topic,
        payload=payload,
        qos=mqtt.QoS.AT_LEAST_ONCE,
        retain=True
    )
    print(f"Published to topic '{topic}':\n{payload}")

    mqtt_connection.disconnect().result()
    print("Disconnected from AWS IoT")

if __name__ == "__main__":
    test_data = "Test QR Content"
    publish_to_aws(test_data)

