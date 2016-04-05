#include <VirtualWire.h>
#include <EEPROM.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CSN_PIN 10
#define CE_PIN 9
#define LED_PIN 6
#define ADDR_BASE 7777
#define ADDR_RMT 6666
//#define R_SPEED 200
#define BTN_PIN 7
#define CHK_INT 777

RF24 radio(CE_PIN, CSN_PIN);


int main_security = 0;
int main_alarm = 0;

int id;

int str = 1;

int btn_state = 0;
int btn_state_last = 0;

void setup() {
    Serial.begin(9600);

    pinMode(LED_PIN,OUTPUT);
    pinMode(BTN_PIN, INPUT);

    radio.begin();
    radio.setChannel(7);
    radio.setRetries(15,15);
    //radio.enableAckPayload();
    radio.setDataRate(RF24_250KBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
    radio.setPALevel(RF24_PA_MAX);

    radio.openReadingPipe(1, ADDR_RMT);
    radio.startListening();

    //vw_set_ptt_inverted(true);
    //vw_set_tx_pin(TX_PIN);
    //vw_set_rx_pin(RX_PIN);
    //vw_setup(R_SPEED);
    //vw_rx_start();

    id = EEPROM.read(0);
    if (id == 255 || id == 0) {
        randomSeed(analogRead(0));
        int randNumber = random(0, 255);
        Serial.print("NEW RND ID: ");
        Serial.println(randNumber);
        EEPROM.write(0, randNumber);
    }
    //sendMessage(str);
    //Serial.print("ID: ");
    //Serial.println(id);

}

void loop () {
    checkIncomingData();
    checkButtons();
    delay(100);
}

void sendMessage (int zone, int val) {
//    BASE_MSG msg = {id, 0, cmd};
    //Serial.println(msg);
//    Serial.print("Size of msg: ");
//    Serial.println(sizeof(msg));
//    vw_send((uint8_t *)&msg, sizeof(msg));
//    vw_wait_tx();
    //digitalWrite(LED_PIN, cmd);
    //radio.writeAckPayload(1, &ok_msg, sizeof(ok_msg));
    
    int cmd[3];
    cmd[0] = CHK_INT;
    cmd[1] = zone; //main security
    cmd[2] = val;

    radio.stopListening();
    radio.openWritingPipe(ADDR_BASE);
    radio.write(cmd, sizeof(cmd));

    radio.openReadingPipe(1, ADDR_RMT);
    radio.startListening();

    Serial.print("Send: ");
    Serial.print(cmd[0]);
    Serial.print(' ');
    Serial.print(cmd[1]);
    Serial.print(' ');
    Serial.println(cmd[2]);


}


void checkIncomingData() {
    if (radio.available()) {

    }

    //Serial.println("Checking...");
    //BASE_MSG receivedData;
    //uint8_t msgSize = sizeof(receivedData);
    //if (vw_get_message((uint8_t *)&receivedData, &msgSize)) {
    //    parseIncCmd(receivedData.cmd);

        //digitalWrite(LED_PIN, receivedData.cmd);
        //sendMessage(!receivedData.cmd);
    //}
}

void parseIncCmd(int cmd) {
    Serial.print("CMD: ");
    Serial.println(cmd);
}

void checkButtons() {
    btn_state = digitalRead(BTN_PIN);

    if (btn_state != btn_state_last) {
        btn_state_last = btn_state;

        Serial.print("BTN: ");
        Serial.println(btn_state);
        if (btn_state > 0) {
            
            sendMessage(1, !main_security);
            main_security = !main_security;
            digitalWrite(LED_PIN, main_security);
        }
    }

}