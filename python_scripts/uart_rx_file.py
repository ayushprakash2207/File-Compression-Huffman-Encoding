import serial           # import the module
import time

# For Windows users, check the correct COM port from Device Manager
ComPort = serial.Serial('COM10')  # Replace with the correct port if needed

ComPort.baudrate = 115200  # Set Baud rate
ComPort.bytesize = 8       # Number of data bits
ComPort.parity = 'N'       # No parity
ComPort.stopbits = 1       # Number of Stop bits

# Create a dummy text file with 34 characters
# with open('dummy_data.txt', 'w') as f:
#     f.write('This is a dummy file with 34 chars.')

# Read the data from the file byte by byte
with open('dummy_data_100KB.txt', 'rb') as f:
    while True:
        byte = f.read(1)
        if not byte:
            break
        ComPort.write(byte)  # Send data to FPGA
        print(f"Sent {byte.hex()} over UART")
        time.sleep(0.0005)  # Delay between transmissions

ComPort.close()  # Close the Com port