# read the first six lines of a file to a string
import os
import sys  
import pandas as pd
import serial

def load_tabs(file_path):
    """
    Load the first six lines of a file into a string.
    
    :param file_path: Path to the file to be read.
    :return: A string containing the first six lines of the file.
    """
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"The file {file_path} does not exist.")
    
    with open(file_path, 'r') as file:
        lines = [next(file) for _ in range(6)]
    
    return ''.join(lines)

if __name__ == "__main__":
    try:
        result = load_tabs("../test/tabs.txt")
        e = result.splitlines()[0]
        B = result.splitlines()[1]
        G = result.splitlines()[2]
        D = result.splitlines()[3]
        A = result.splitlines()[4]
        E = result.splitlines()[5]

        # send the data to the serial port COM7
        ser = serial.Serial('COM7', 115200, timeout=1)
        ser.write(e.encode())
        ser.write(B.encode())
        ser.write(G.encode())
        ser.write(D.encode())
        ser.write(A.encode())
        ser.write(E.encode())

        # read from serial
        while True:
            response = ser.readline().decode() 
            print(response)
            
    except Exception as e:
        print(f"An error occurred: {e}")
        sys.exit(1)