from awscrt import io, mqtt
from awsiot import mqtt_connection_builder
import json
import time

def connect_mqtt(serial_number):
    endpoint = "a3ghhywi7gsung-ats.iot.ca-central-1.amazonaws.com"
    keep_alive_secs = 60

    #topic = "dt/conestoga/esd/lab/" + serial_number

    ca_filepath = "certificates/AmazonRootCA1.pem"
    cert_filepath = "certificates/certificate.pem.crt"
    key_filepath = "certificates/private.pem.key"

    # Spin up resources
    event_loop_group = io.EventLoopGroup(1)
    host_resolver = io.DefaultHostResolver(event_loop_group)
    client_bootstrap = io.ClientBootstrap(event_loop_group, host_resolver)
    mqtt_connection = mqtt_connection_builder.mtls_from_path(
        endpoint=endpoint,
        cert_filepath=cert_filepath,
        pri_key_filepath=key_filepath,
        client_bootstrap=client_bootstrap,
        ca_filepath=ca_filepath,
        client_id=serial_number,
        clean_session=False,
        keep_alive_secs=keep_alive_secs
    )
    try:
        mqtt_connection.connect().result()
        print("MQTT connected")
        return mqtt_connection
    except Exception as e:
        print(f"MQTT connection failed: {e}")
        return None

def send_single_record(mqtt_connection, topic, record, serial_number):
    payload = json.dumps(record)
    mqtt_connection.publish(topic=topic, payload=payload, qos=mqtt.QoS.AT_LEAST_ONCE)
    print(f"Device:{serial_number} s ending payload: {payload}")

# 🔹 Standalone test mode
if __name__ == "__main__":
    serial_number = "Tran"
    topic = f"dt/conestoga/esd/lab/{serial_number}"
    mqtt_conn = connect_mqtt(serial_number)

    if mqtt_conn:
        #dummy record
        test_record = {
            "count": "1",
            "application": "Minh Quan Tran",
            "timestamp": int(time.time()),
            "latitude": 48.1173,
            "longitude": 11.5167
        }
        send_single_record(mqtt_conn, topic, test_record, serial_number)
        mqtt_conn.disconnect().result()
        print("Disconnected")
