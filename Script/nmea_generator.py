#!/usr/bin/env python3
# This code written by AI. Please before run check it.
import serial
import time
import signal
import sys
from datetime import datetime

# =========================================================
# Configuration
# =========================================================

SERIAL_PORT = "/dev/ttyUSB0"
BAUD_RATE = 9600
SEND_INTERVAL_SEC = 0.2

# =========================================================
# ANSI Colors
# =========================================================

COLOR_RESET  = "\033[0m"
COLOR_GREEN  = "\033[32m"
COLOR_YELLOW = "\033[33m"
COLOR_RED    = "\033[31m"
COLOR_CYAN   = "\033[36m"

# =========================================================
# NMEA Test Stream
# =========================================================

NMEA_MESSAGES = [
    "$GPRMC,183729.00,A,3907.356,N,12102.482,W,000.5,054.7,190526,015.5,E*67\r\n",
    "$GPGGA,183730.00,3907.356,N,12102.482,W,1,08,0.9,545.4,M,46.9,M,,*47\r\n",
    "$GPGSA,A,3,04,05,09,12,24,25,29,31,,,,,1.8,1.0,1.5*33\r\n",
    "$GPGSV,3,1,11,04,77,045,42,05,65,123,45,09,52,067,43,12,48,312,41*70\r\n",
    "$GPVTG,054.7,T,034.4,M,005.5,N,010.2,K,A*23\r\n",
]

# =========================================================
# Global State
# =========================================================

running = True
tx_counter = 0
byte_counter = 0

# =========================================================
# Utility Functions
# =========================================================

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

# =========================================================
# Signal Handler
# =========================================================

def signal_handler(sig, frame):
    global running
    running = False
    log_warn("Stopping GPS simulator...")

signal.signal(signal.SIGINT, signal_handler)

# =========================================================
# Main
# =========================================================

def main():

    global tx_counter
    global byte_counter
    global running

    log_info("========================================")
    log_info("STM32 GPS / NMEA UART Simulator")
    log_info("========================================")

    log_info(f"Serial Port : {SERIAL_PORT}")
    log_info(f"Baud Rate   : {BAUD_RATE}")
    log_info(f"Interval    : {SEND_INTERVAL_SEC} sec")

    try:
        ser = serial.Serial(
            port=SERIAL_PORT,
            baudrate=BAUD_RATE,
            timeout=1
        )

        log_info("Serial port opened successfully")

    except Exception as e:
        log_error(f"Failed to open serial port: {e}")
        sys.exit(1)

    log_info("Starting continuous NMEA transmission...")
    log_info("Press CTRL+C to stop")

    while running:

        for idx, sentence in enumerate(NMEA_MESSAGES):

            if not running:
                break

            try:
                encoded = sentence.encode("ascii")

                ser.write(encoded)

                tx_counter += 1
                byte_counter += len(encoded)

                sentence_name = sentence[1:6]

                log_tx(
                    f"#{tx_counter:06d} | "
                    f"{sentence_name} | "
                    f"{len(encoded)} bytes | "
                    f"Total: {byte_counter} bytes"
                )

                print(f"      {sentence.strip()}")

                time.sleep(SEND_INTERVAL_SEC)

            except Exception as e:
                log_error(f"Transmission failed: {e}")
                running = False
                break

    try:
        ser.close()
        log_info("Serial port closed")

    except Exception:
        pass

    log_info("========================================")
    log_info("Transmission Summary")
    log_info("========================================")
    log_info(f"Sentences Sent : {tx_counter}")
    log_info(f"Bytes Sent     : {byte_counter}")
    log_info("GPS simulator terminated")


if __name__ == "__main__":
    main()