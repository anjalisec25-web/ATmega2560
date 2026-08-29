//Connect a LED to port J bit 0. Off LSB LED only (bit 0)

//code:

void setup() {
  volatile *dir;
  dir = 0x104;
  *dir = 0x01;
}

void loop() {
  volatile *out;
  out = 0x103;
  *out = (0 << 0);
}