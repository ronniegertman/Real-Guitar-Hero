import serial
import time
import sys

# --- Configuration ---
COM_PORT = 'COM5'    # IMPORTANT: Change this if your ESP32 is on a different port!
BAUD_RATE = 115200
TIMEOUT = 5 # Timeout for reading serial port

def serial_communication():
    """Handles the serial connection and message exchange."""
    
    print(f"Attempting to connect to {COM_PORT} at {BAUD_RATE} baud...")
    
    try:
        # Open the serial connection
        ser = serial.Serial(COM_PORT, BAUD_RATE, timeout=TIMEOUT)
        
        # Give the ESP32 time to reboot after the serial connection is opened
        print("Waiting for ESP32 to reboot (3 seconds)...")
        time.sleep(3) 
        
        print("--- Start of Communication ---")

        # 1. Wait for ESP32's "READY" signal
        print("Waiting for ESP32 'READY' signal...")
        ready = False
        start_time = time.time()
        
        while time.time() - start_time < 10: # 10 second timeout
            if ser.in_waiting > 0:
                line = ser.readline().decode('utf-8').strip()
                if "ESP_READY" in line:
                    print(f"ESP32: {line}")
                    ready = True
                    break
                # Print other boot messages
                if line:
                    print(f"ESP32: {line}")
            time.sleep(0.1)

        if not ready:
            print("ERROR: ESP32 did not send 'ESP_READY'. Exiting.")
            ser.close()
            sys.exit(1)

        # 2. Send the message to ESP32
        message_to_send = "Hello from Python!\n"
        print(f"\nSending to ESP32: {message_to_send.strip()}")
        ser.write(message_to_send.encode('utf-8'))
        
        # 3. Listen for response and print everything
        print("\nListening for ESP32 response (10 seconds)...")
        start_time = time.time()
        
        while time.time() - start_time < 10:
            if ser.in_waiting > 0:
                try:
                    # Read the incoming data line by line
                    response = ser.readline().decode('utf-8').strip()
                    if response:
                        print(f"ESP32: {response}")
                except UnicodeDecodeError:
                    print("ERROR: Received non-UTF-8 data.")
                except Exception as e:
                    print(f"An error occurred while reading: {e}")
            
            time.sleep(0.01) # Small delay to prevent high CPU usage

        print("\n--- Communication Timeout Reached. Closing port. ---")
        
    except serial.SerialException as e:
        print(f"FATAL ERROR: Could not open or communicate with serial port {COM_PORT}.")
        print(f"Details: {e}")
        print("1. Check if the port is correct.")
        print("2. Ensure no other program (like the Arduino Serial Monitor) is using the port.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    serial_communication()