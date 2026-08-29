# PORTx — Port Data Register

The **PORTx (Port Data Register)** is used to control the logic level of GPIO pins in an AVR microcontroller.

Its function depends on the pin direction configured using `DDRx`.

- `DDRx = 1` → `PORTx` controls the **output level**
- `DDRx = 0` → `PORTx` controls the **internal pull-up resistor**
