int led = 9;
int brightness = 0;
int fadeAmount = 10;
char inc[64];
bool send = false;
// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0) {
    Serial.readBytes(inc, 63);
    Serial.print("You say: ");
    Serial.write(inc);
    Serial.println();
    memset(inc, 0, sizeof(inc) / sizeof(inc[0]));
  }

  analogWrite(led, brightness);
  brightness = brightness + fadeAmount;
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount;
  }
  delay(30);
}
