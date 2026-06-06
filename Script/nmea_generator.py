#!/usr/bin/env python3
import serial
import time
import signal
import sys
import argparse
from datetime import datetime, timezone

# =========================================================
# ANSI Colors
# =========================================================
COLOR_RESET  = "\033[0m"
COLOR_GREEN  = "\033[32m"
COLOR_YELLOW = "\033[33m"
COLOR_RED    = "\033[31m"
COLOR_CYAN   = "\033[36m"

# =========================================================
# Global State
# =========================================================
running = True
tx_counter = 0
byte_counter = 0

def timestamp():
    return datetime.now().strftime("%H:%M:%S.%f")[:-3]

def log_info(msg):
    print(f"{COLOR_GREEN}[INFO]{COLOR_RESET} {timestamp()} - {msg}")

def log_warn(msg):
    print(f"{COLOR_YELLOW}[WARN]{COLOR_RESET} {timestamp()} - {msg}")

def log_error(msg):
    print(f"{COLOR_RED}[ERROR]{COLOR_RESET} {timestamp()} - {msg}")

def log_tx(msg):
    print(f"{COLOR_CYAN}[TX]{COLOR_RESET} {timestamp()} - {msg}")

def build_nmea_sentence(payload):
    """Calculates the XOR checksum and wraps the payload in $ and \\r\\n"""
    checksum = 0
    for char in payload:
        checksum ^= ord(char)
    return f"${payload}*{checksum:02X}\r\n".encode('ascii')

def generate_dynamic_burst():
    """Generates a burst of NMEA sentences using current UTC time"""
    # GPS standard uses UTC time, not local PC time
    now_utc = datetime.now(timezone.utc)
    
    # Format: HHMMSS.00
    nmea_time = now_utc.strftime("%H%M%S.00")
    # Format: DDMMYY
    nmea_date = now_utc.strftime("%d%m%y")

    # Inject real time into the payloads
    payloads = [
        f"GPRMC,{nmea_time},A,3907.356,N,12102.482,W,000.5,054.7,{nmea_date},015.5,E",
        f"GPGGA,{nmea_time},3907.356,N,12102.482,W,1,08,0.9,545.4,M,46.9,M,,",
        "GPGSA,A,3,04,05,09,12,24,25,29,31,,,,,1.8,1.0,1.5",
        "GPGSV,3,1,11,04,77,045,42,05,65,123,45,09,52,067,43,12,48,312,41",
        "GPVTG,054.7,T,034.4,M,005.5,N,010.2,K,A"
    ]
    
    return [build_nmea_sentence(p) for p in payloads]

# =========================================================
# Signal Handler
# =========================================================
def signal_handler(sig, frame):
    global running
    running = False
    print() 
    log_warn("Stopping GPS simulator...")

signal.signal(signal.SIGINT, signal_handler)

# =========================================================
# Main
# =========================================================
def main():
    global tx_counter
    global byte_counter
    global running

    parser = argparse.ArgumentParser(description="True NEO-6M GPS Emulator")
    parser.add_argument("-p", "--port", type=str, default="/dev/ttyUSB0", help="Serial port (default: /dev/ttyUSB0)")
    parser.add_argument("-b", "--baud", type=int, default=9600, help="Baud rate (default: 9600)")
    parser.add_argument("-i", "--interval", type=float, default=1.0, help="Update rate in seconds (default: 1.0)")
    args = parser.parse_args()

    log_info("========================================")
    log_info("STM32 True NEO-6M UART Simulator")
    log_info("========================================")
    log_info(f"Serial Port : {args.port}")
    log_info(f"Baud Rate   : {args.baud}")
    log_info(f"Update Rate : {args.interval} sec (1Hz Default)")

    try:
        ser = serial.Serial(port=args.port, baudrate=args.baud, timeout=1)
        log_info("Serial port opened successfully")
    except serial.SerialException as e:
        log_error(f"Failed to open serial port: {e}")
        sys.exit(1)

    log_info("Transmitting NEO-6M Boot Sequence...")
    # Send standard u-blox boot sequence
    boot_msgs = [
        build_nmea_sentence("GPTXT,01,01,02,u-blox ag - www.u-blox.com"),
        build_nmea_sentence("GPTXT,01,01,02,HW  UBX-G60xx  00040007"),
        build_nmea_sentence("GPTXT,01,01,02,ANTSTATUS=OK")
    ]
    for msg in boot_msgs:
        ser.write(msg)
        time.sleep(0.1)

    log_info("Starting continuous NMEA live-time transmission...")
    print("-" * 40)

    while running:
        start_time = time.time()

        try:
            # Generate fresh sentences with the current exact UTC time
            compiled_sentences = generate_dynamic_burst()

            for encoded in compiled_sentences:
                if not running:
                    break
                
                ser.write(encoded)
                tx_counter += 1
                byte_counter += len(encoded)

                # Decode for terminal display
                sentence_str = encoded.decode('ascii').strip()
                sentence_name = sentence_str[1:6]

                log_tx(f"#{tx_counter:06d} | {sentence_name} | {len(encoded):02d} bytes | {sentence_str}")

            ser.flush()
            print("-" * 40)

            # Wait exactly the remainder of the interval (usually 1 second)
            elapsed = time.time() - start_time
            sleep_time = args.interval - elapsed
            if sleep_time > 0 and running:
                time.sleep(sleep_time)

        except Exception as e:
            log_error(f"Transmission failed: {e}")
            running = False
            break

    try:
        if ser.is_open:
            ser.close()
            log_info("Serial port closed")
    except Exception:
        pass

if __name__ == "__main__":
    main()