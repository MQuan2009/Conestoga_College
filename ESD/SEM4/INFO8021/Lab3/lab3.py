import argparse
import keyboard
from MTran_TECH8021_lab1 import get_gps
from MTran_TECH8021_lab3 import connect_mqtt, send_single_record

quit_flag = False

def quit_this():
    global quit_flag
    print("Hotkey 'q' pressed. Exiting...")
    quit_flag = True

def should_quit():
    return quit_flag

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run Lab3")
    parser.add_argument("-f", "--file", help="Path to NMEA file (required for lab1/full)")
    parser.add_argument("-d", "--debug", action="store_true")
    parser.add_argument("-p", "--pub_period", type = int, default = 200, help="Publish period")
    args = parser.parse_args()

    serial_number = "Tran"
    topic = f"dt/conestoga/esd/lab/{serial_number}"

    if not args.file:
        print("Error: --file is required")
    else:
        keyboard.add_hotkey("q", quit_this)
        print("Press 'q' to quit")
        mqtt_conn = connect_mqtt(serial_number)
        if mqtt_conn:
            for record in get_gps(args.file, debug=args.debug, period_in_ms=args.pub_period, quit_callback=should_quit):
                send_single_record(mqtt_conn, topic, record, serial_number)
            mqtt_conn.disconnect().result()
            print("Disconnected from AWS IoT")
