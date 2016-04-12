#include <VirtualWire.h>
#include <EEPROM.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CSN_PIN 53  // Mega Pin
#define CE_PIN 40   // Mega Pin
#define LED_PIN 7
#define ONE_DOOR_PIN 9
#define ADDR_BASE 7777
#define ADDR_RMT 6666
//#define R_SPEED 200
//#define BTN_PIN 7
#define CHK_INT 777

RF24 radio(CE_PIN, CSN_PIN);

//struct BASE_MSG {
//    unsigned int from_id;
//    unsigned int to_id;
//    unsigned int cmd;
//};

int id;

int main_security = 0;
int main_alarm = 0;

void setup() {

    Serial.begin(9600);

    pinMode(LED_PIN,OUTPUT);
    pinMode(ONE_DOOR_PIN, INPUT);

    radio.begin();
    radio.setChannel(7);
    radio.setRetries(15,15);
    //radio.enableAckPayload();
    radio.setDataRate(RF24_250KBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
    radio.setPALevel(RF24_PA_MAX);

    radio.openReadingPipe(1, ADDR_BASE);
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

    //Serial.print("ID: ");
    //Serial.println(id);
}

void loop() {
    checkIncomingData();
    checkCar();
    delay(20);
}

void checkCar() {
    // тут проверка датчиков с авто
    if (main_security) {
        int door = digitalRead(ONE_DOOR_PIN);
        if (door != 1 && main_alarm == 0) {
            main_alarm = 1;
            sendMessage(2, 1); // 2 - зона дверей, 1 - есть открытая
            // TODO: локальный звуковой сигнал
        }
    }
}

void sendMessage (int zone, int val) {

    int cmd[3];
    cmd[0] = CHK_INT;
    cmd[1] = zone;
    cmd[2] = val;

    radio.stopListening();
    radio.openWritingPipe(ADDR_RMT);
    radio.write(cmd, sizeof(cmd));

    radio.openReadingPipe(1, ADDR_BASE);
    radio.startListening();

    // Serial.print("Send: ");
    // Serial.print(cmd[0]);
    // Serial.print(' ');
    // Serial.print(cmd[1]);
    // Serial.print(' ');
    // Serial.println(cmd[2]);
}

void checkIncomingData() {
    int cmd[3];
    if (radio.available()) {
        radio.read(cmd, sizeof(cmd));
        delay(100);
        Serial.print("REC: ");
        Serial.print(cmd[0]);
        Serial.print(' ');
        Serial.print(cmd[1]);
        Serial.print(' ');
        Serial.println(cmd[2]);

        if (cmd[0] == CHK_INT) {
            parseIncCmd(cmd[1], cmd[2]);
        }
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

void parseIncCmd(int zone, int val) {
    Serial.print("CMD: zone - ");
    Serial.print(zone);
    Serial.print(" ; val - ");
    Serial.println(val);

    if (zone == 1) { // охрана
        // Serial.println(main_security);
        main_security = !main_security;
        if (main_alarm == 1 ) {
            main_alarm = 0;
            main_security = 1;
            sendMessage(2, 0);
        }
        if (main_security == 1) {
            digitalWrite(LED_PIN, HIGH);
        } else {
            digitalWrite(LED_PIN, LOW);
        }
        sendMessage(1, main_security);
    }
}