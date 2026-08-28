//Connect a LED to port J bit 0. Glow LSB LED only (bit 0)

//code:

void setup() {
  volatile *dir;
  dir=0x104;
  *dir=0x01;

}

void loop() {
  volatile *out;
  out=0x103;
  *out=(1<<0);

}
