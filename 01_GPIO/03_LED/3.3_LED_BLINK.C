//Connect a LED to Port J bit 0.Blink the led.

void setup() {
  volatile *dir;
  dir=0x104;
  *dir=0xFF;
}

void loop() {
  volatile *out;
  out=0x103;
  *out=0x01;
  delay(1000);
  *out=0x00;
  delay(1000);
}
