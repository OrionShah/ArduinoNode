#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10);
const uint32_t pipe = 7777;

byte massiv[2];

void setup()   
{
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  radio.begin();
  delay(10);
  radio.setChannel(7);
  //radio.setRetries(15,15);
  radio.setDataRate(RF24_250KBPS); // RF24_1MBPS или RF24_2MBPS
  radio.setPALevel(RF24_PA_MAX);
  //radio.setAutoAck(1);
  radio.openReadingPipe(1,pipe); // открыть канал на приём
  radio.startListening(); // приём
}


void loop()  
{
  if(radio.available())
    {
      radio.read(massiv, 1);
      if(massiv[0] == 200)
        {
          Serial.println(massiv[0]); 
          digitalWrite(6, HIGH);
        }

      if(massiv[0] == 100)
        {
          Serial.println(massiv[0]); 
          digitalWrite(6, LOW);
        }        
     }
}
