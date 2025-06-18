import time
import json
from awscrt import io, mqtt, auth, http
from awsiot import mqtt_connection_builder

# Config
endpoint = "aqrm5lymlussz-ats.iot.ca-central-1.amazonaws.com"
client_id = "ESP32_WiFi"
topic = "esp32/sub"
cert = "device.pem.crt"
private_key = "private.pem.key"
root_ca = "root-CA.crt"

# Set up MQTT connection
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

print(f"Connecting to {endpoint} with client ID '{client_id}'...")
connect_future = mqtt_connection.connect()
connect_future.result()
print("Connected!")

# Publish message
message = {"message": "Hi from RaspberryPi4B"}
payload = json.dumps(message,indent=2)

# message = "Hello from Raspberry Pi 4B!".encode("utf-8")
mqtt_connection.publish(topic=topic, payload=payload, qos=mqtt.QoS.AT_LEAST_ONCE,  retain=True)
print(f"Published: '{message}' to topic '{topic}'")

# time.sleep(2)

# Disconnect
disconnect_future = mqtt_connection.disconnect()
disconnect_future.result()
print("Disconnected.")
 
