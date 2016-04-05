#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // можно использовать любые
const uint32_t pipe = 0xF0F1F2F3F1LL; // адрес
byte out[1];

void setup()
{
  Serial.begin(9600);
  radio.begin();
  delay(10);
  radio.setChannel(7);
  //radio.setRetries(15,15);
  radio.setDataRate(RF24_250KBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  radio.setPALevel(RF24_PA_MAX);
  radio.openWritingPipe(pipe); // открыть канал на отправку
}


void loop()   
{
  out[0] = 200;
  radio.write(out, 1);
  Serial.println(out[0]);
  delay(500);
  out[0] = 100;
  radio.write(out, 1);
  Serial.println(out[0]);
  delay(500);
}
