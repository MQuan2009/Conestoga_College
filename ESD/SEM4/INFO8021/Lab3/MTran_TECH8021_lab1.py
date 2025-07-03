import pynmea2
import json
import time
import argparse
import keyboard

quit_flag = False  # Global flag to signal quitting

def is_valid_lat_lon(msg):

    lat = msg.latitude
    lon = msg.longitude

    # Check if lat/lon are numbers and within valid ranges
    if lat is None or lon is None:
        return False

    if lat == 0.0 and lon == 0.0:
        return False

    if not (-90 <= lat <= 90):
        return False

    if not (-180 <= lon <= 180):
        return False

    return True

def get_gps(file_path, debug = False, period_in_ms = 200, quit_callback = lambda:False):

    line_number = 0
    if debug:
        print("Press 'q' at any time to quit the program.")

    try:
        if debug:
            print("Attempting to open file...")

        with open(file_path, "r") as file:
            if debug:
                print("File opened successfully.")

            for line in file:
                if quit_callback():
                    if debug:
                        print("Quit callback detected. Exiting main loop.")
                    break

                line_number += 1
                line = line.strip()

                try:
                    msg = pynmea2.parse(line)

                    if hasattr(msg, 'latitude') and hasattr(msg, 'longitude'):
                        if is_valid_lat_lon(msg):
                            record = {
                                "count": str(line_number),
                                "application": "Minh Quan Tran",
                                "timestamp": str(int(time.time())),
                                "latitude": str(msg.latitude),
                                "longitude": str(msg.longitude)
                            }
                            yield record #VERY useful, pause the function then return the current record

                            if debug:
                                print(f"Parsed line {line_number}: lat={msg.latitude}, lon={msg.longitude}")
                        else:
                            if debug:
                                print(f"Line {line_number} does not contain a valid latitude/longitude info.")
                    else:
                        if debug:
                            print(f"Line {line_number} does not contain latitude/longitude info.")

                except pynmea2.ParseError as pe:
                    if debug:
                        print(f"Failed to parse line {line_number}: {pe}")
                    continue

                time.sleep(period_in_ms/1000)

    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except Exception as e:
        print(f"Unexpected error: {e}")

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description="Standalone GPS parser")
    parser.add_argument("-f", "--file", required=True, help="Path to NMEA file")
    parser.add_argument("-d", "--debug", action="store_true", help="Enable debug output")
    args = parser.parse_args()

    for rec in get_gps(args.file, args.debug, period_in_ms=20):
        print(rec)
