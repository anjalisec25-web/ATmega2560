//Connect a LED to port D bit 7. Glow MSB LED only (bit 7).

void setup() {
  volatile *dir;
  dir=0x2A;
  *dir=0xFF;
}

void loop() {
  volatile *out;
  out=0x29;
  *out=1<<7; 
}