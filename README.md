# STM32 RTOS-Based GPS/GSM Telemetry System

A lightweight bare-metal / RTOS-ready telemetry platform based on the STM32F103 series.

This project is designed for remote monitoring and telemetry applications using:

- GSM/GPRS communication via SIM800L
- GPS location and UTC time extraction via NEO-6M
- Environmental and system monitoring
- USART + DMA communication pipelines
- Modular peripheral APIs for embedded development

The current implementation focuses on:

- Low-level driver validation
- Communication stability
- Hardware integration testing
- Embedded firmware architecture

---


# Requirement

- STM32F103C8T6 ( I used by Blue Pill )
- cmake >= 3.22
- ARM toolchain
- OpenOCD
- ST-LINK

---


# How to build

```shell
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/gcc-arm-none-eabi.cmake ..
make -j$(nproc)
```

The build process generates a .bin file. Flash this firmware to your microcontroller using ST-LINK or any hardware tool that supports STM32.

NOTE: This project is not ready for production use. Anyone using this repository must complete the following steps:

1- Review the code carefully to verify compatibility with your specific hardware

2- Check the system clock configuration (HSI or HSE 8MHz crystal can be selected in CMake)

3- Choose the appropriate build type – build in Debug mode for initial testing, then switch to Release only after full validation

4- Double-check all hardware connections during testing, including GPIO, UART, SIM800L, and NEO-6M


---


# Compile-Time Feature Flags

```cmake
option(MONITOR_INTERNAL_TEMP "Enable internal temperature monitoring" ON)

option(TEST_DMA "Enable DMA testing utilities" OFF)
```

---


# License

This project is intended for educational, research, and embedded systems development purposes.
