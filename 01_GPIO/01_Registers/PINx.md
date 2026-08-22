# PINx — Port Input Pins Address Register

The **PINx (Port Input Pins Address Register)** is used to **read the current logic level** of GPIO pins in an AVR microcontroller.

Each bit of the PINx register represents the logic level present on the corresponding GPIO pin.

## Reading a Pin

- `0` → Pin is LOW
- `1` → Pin is HIGH