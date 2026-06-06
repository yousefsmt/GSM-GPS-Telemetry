import serial
import time
import sys

# --- CONFIGURATION FOR LINUX ---
SERIAL_PORT = '/dev/ttyUSB0'  # Linux USB serial port
BAUD_RATE = 115200            # Your specified baud rate
INTERVAL = 0.1                # Time delay between sends in seconds
MESSAGE = "Hello World\n"      # The string you want to send
# -------------------------------

def periodic_serial_sender():
    print(f"Initializing serial port {SERIAL_PORT} at {BAUD_RATE} baud...")
    
    try:
        # Open the serial port
        with serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1) as ser:
            print(f"Successfully connected to {SERIAL_PORT}.")
            print(f"Sending '{MESSAGE.strip()}' every {INTERVAL} seconds. Press Ctrl+C to stop.\n")
            
            # Flush buffers to start clean
            ser.reset_output_buffer()
            
            while True:
                # Encode the string to bytes and send
                ser.write(MESSAGE.encode('utf-8'))
                
                print(f"[{time.strftime('%H:%M:%S')}] Sent: {MESSAGE.strip()}")
                
                # Wait for the specified interval
                time.sleep(INTERVAL)
                
    except serial.SerialException as e:
        print(f"\n[Error] Could not open serial port {SERIAL_PORT}: {e}")
        print("\n💡 Linux Quick Fix:")
        print(f"If it's a permission error, run this command to grant access:\nsudo chmod 666 {SERIAL_PORT}")
        sys.exit(1)
        
    except KeyboardInterrupt:
        print("\nScript stopped by user. Closing port.")
        sys.exit(0)

if __name__ == "__main__":
    periodic_serial_sender()
