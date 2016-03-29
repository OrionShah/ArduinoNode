// Tx на пине D12
#include <VirtualWire.h>

char *controller;
char str;
void setup() {
//  Serial.begin(9600);
  pinMode(13,OUTPUT);
  vw_set_ptt_inverted(true);
  vw_set_tx_pin(12);
  vw_setup(200);// скорость передачи данных в Kbps
}

void loop(){
  str='1';
  controller=&str;
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); // ждем, пока отправится все сообщение
  digitalWrite(13,1);
//  Serial.println("send 1");
  delay(1000);
  str='0';
  controller=&str;
  vw_send((uint8_t *)controller, strlen(controller));
  vw_wait_tx(); // ждем, пока отправится все сообщение
  digitalWrite(13,0);
//  Serial.println("send 0");
  delay(1000);
}
