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

def quit_this():
    global quit_flag
    print("\nQuit hotkey 'q' pressed. Exiting gracefully.")
    quit_flag = True

def main():
    global quit_flag

    #Added argument
    parser = argparse.ArgumentParser(description="NMEA GPS Reader")
    parser.add_argument("-f","--file", required=True, help="Path to NMEA file") #Choosing what file to read
    parser.add_argument("-d","--debug", action="store_true", help="Enable debug messages") #Enabling debug message
    args = parser.parse_args()

    file_path = args.file
    debug = args.debug
    output_list = []
    line_number = 0

    # Added hotkey
    keyboard.add_hotkey('q', quit_this)
    if debug:
        print("Press 'q' at any time to quit the program.")

    try:
        if debug:
            print("Attempting to open file...")

        with open(file_path, "r") as file:
            if debug:
                print("File opened successfully.")

            for line in file:
                if quit_flag:
                    if debug:
                        print("Quit flag detected. Exiting main loop.")
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
                                "timestamp": str(int(time.time())),la
                                "latitude": str(msg.latitude),
                                "longitude": str(msg.longitude)
                            }
                            output_list.append(record)

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

                time.sleep(0.2)

    except FileNotFoundError:
        print(f"File not found: {file_path}")
    except Exception as e:
        print(f"Unexpected error: {e}")
    else:
        print("File processing complete.")
        print("Outputting final JSON...\n")
        print(json.dumps(output_list, indent=4))

if __name__ == "__main__":
    main()
