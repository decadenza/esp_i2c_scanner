# Basic I2C scanner project for Espressif ESP-IDF

## Usage
1. Configure the definitions at the beginning of the `esp_i2c_scanner.c`.
2. Build.
3. Flash.
4. **At boot only** the code scans for devices, prints the results and exits. Reboot to perform a new scan.


## Notes
- Supports only standard (7 bit) I2C address range. It may be easily extended to the 10-bit addressing, if needed.
- Tested on ESP32.
