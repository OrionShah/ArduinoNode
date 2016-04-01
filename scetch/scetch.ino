int led = 9;
int led1 = 10;
int btn = 2;
int brightness = 0;
int fadeAmount = 10;

int led_fade = 255;
int led1_fade = 0;

char inc[64];
String last_command;

int btn_state;
bool send = true;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT);
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  if (Serial.available() > 0) {
    Serial.readBytes(inc, 63);

    bool chk = parseCommand(inc);

    if (send && !chk) {
      Serial.print("You say: ");
      Serial.write(inc);
      Serial.println();  
    }
    memset(inc, 0, sizeof(inc) / sizeof(inc[0]));
  }

  
  if (send) {
    btn_state = digitalRead(btn);
  }

  if (btn_state) {
    led_fade = led_fade + fadeAmount;
    led1_fade = led1_fade - fadeAmount;
  } else {
    led_fade = led_fade - fadeAmount;
    led1_fade = led1_fade + fadeAmount;
  }

  if (led_fade > 255) {
    led_fade = 255;
  }
  if (led1_fade > 255) {
    led1_fade = 255;
  }
  if (led_fade < 0) {
    led_fade = 0;
  }
  if (led1_fade < 0) {
    led1_fade = 0;
  }

  Serial.print("led_fade: ");
  Serial.println(led_fade);
  Serial.print("led1_fade: ");
  Serial.println(led1_fade);

  analogWrite(led, led_fade);
  analogWrite(led1, led1_fade);
  
  
  Serial.print("Button: ");
  Serial.println(btn_state);

  delay(30);
}

bool parseCommand(char cmd[64]) {
  if (cmd[0] == 's' && cmd[1] == 'e' && cmd[2] == 't') {
    btn_state = !btn_state;
    send = false;
    return true;
  }

  if (cmd[0] == 'r' && cmd[1] == 's' && cmd[2] == 't') {
    send = true;
    return true;
  }

  return false;
  
}

