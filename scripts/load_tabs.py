# read the first six lines of a file to a string
import os
import sys  
import pandas as pd
import serial
import time

def load_tabs(file_path):
    """
    Load the six lines of tabs file into a string.
    
    :param file_path: Path to the file to be read.
    :return: Array of strings containing the tab lines of the file.
    """
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"The file {file_path} does not exist.")
    
    # Initialize strings for each guitar string
    line_e = ""
    line_B = ""
    line_G = ""
    line_D = ""
    line_A = ""
    line_E = ""
    
    with open(file_path, 'r') as file:
        line_num = 0
        for line in file:
            # Don't strip here - let ESP handle raw data
            if not line.strip():  # Only check if line is empty (but don't modify it)
                continue
                
            idx = line_num % 6
            # Remove the first 2 characters (e.g., "e|") because we dont want them to be in mid string,
            # only at the start
            # Good Practce: ESP will need to handle the newlines and whitespace, i.e., we don't strip the line
            content = line[2:] if len(line) > 2 else ""
            
            if idx == 0:
                line_e += content
            elif idx == 1:
                line_B += content
            elif idx == 2:
                line_G += content
            elif idx == 3:
                line_D += content
            elif idx == 4:
                line_A += content
            elif idx == 5:
                line_E += content
            
            line_num += 1
    
    # Add the string labels back
    line_e = "e|" + line_e
    line_B = "B|" + line_B
    line_G = "G|" + line_G
    line_D = "D|" + line_D
    line_A = "A|" + line_A
    line_E = "E|" + line_E
    
    # Check for incomplete blocks
    if line_num % 6 != 0:
        print(f"Warning: tab file has incomplete block (lines not a multiple of 6).")
    
    return [line_e, line_B, line_G, line_D, line_A, line_E]

def wait_for_esp_ready(ser):
    """
    Wait for ESP to send "ESP_READY" message, ignoring boot messages.
    
    :param ser: Serial connection object
    :return: True if ESP_READY received, False if timeout
    """
    print("Waiting for ESP to be ready...")
    start_time = time.time()
    
    while time.time() - start_time < 10:  # 10 second timeout
        if ser.in_waiting > 0:  # Check if there is data to read
            try:
                # Read line and handle potential decoding errors from boot gibberish
                line = ser.readline()
                try:
                    response = line.decode('utf-8').strip()
                except UnicodeDecodeError:
                    # Skip lines that can't be decoded (boot gibberish)
                    print("Skipping boot message (decoding error)")
                    continue
                
                if response:  
                    print(f"Boot message: {response}")  # Print the boot message for debugging
                    
                    # Check if ESP is ready
                    if "ESP_READY" in response:
                        print("ESP is ready!")
                        return True
            except Exception as e:
                print(f"Error reading from serial: {e}")
                continue
        
        time.sleep(0.1)  # Small delay to avoid busy waiting
    
    return False


if __name__ == "__main__":
    try:
        # Load tabs
        tabs = load_tabs(r"..\test\tabs.txt")
        e, B, G, D, A, E = tabs
        
        # print(e + "\n" + B + "\n" + G + "\n" + D + "\n" + A + "\n" + E)  # Print the tabs for debugging
        # Connect to ESP
        ser = serial.Serial('COM5', 115200, timeout=4)  
        ser.dtr = False  # Disable DTR to prevent reset
        ser.rts = False  # Disable RTS to prevent reset        
        time.sleep(4)  # Wait for the serial connection to initialize
       
        print("ESP is ready, sending data...")
        for tab in tabs:  # loops through each tab line
            # Remove any trailing newlines and add exactly one, otherwise ESP will not read it correctly
            clean_tab = tab.rstrip('\n')
            ser.write((clean_tab + '\n').encode())
            print(f"Sent: {clean_tab}")
            time.sleep(0.1)  # Small delay to ensure ESP processes each line

        print("sending END")
        ser.write(b"END\n")
        response = ser.readline().decode().strip()

        # while response != "SUCCESS!":
        #     response = ser.readline().decode()
        while "Received tabs" not in response:
            response = ser.readline().decode().strip()
        print("response from esp: ", response)
        ser.close()

    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)