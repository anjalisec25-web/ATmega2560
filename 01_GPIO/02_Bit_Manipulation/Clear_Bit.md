# Clear Bit

## Definition

Clearing a bit means changing a specific bit from `1` to `0`.

---

## Operator Used

The **Bitwise AND (`&`)** and **Bitwise NOT (`~`)** operators are used to clear a bit.

### Syntax

value = value & ~(1 << position);

or,

value &= ~(1 << position);

## Example

clear Bit 5 of PORT B register.

PORTB &= ~(1 << 5);

Multiple bits can be cleared using AND.

PORTB &= ~((1 << 5) | (1 << 3) | (1 << 1));