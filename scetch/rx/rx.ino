#include <VirtualWire.h>
#include <EEPROM.h>

#define RX_PIN 12
#define TX_PIN 13
#define LED_PIN 3
#define R_SPEED 200

struct BASE_MSG {
    unsigned int from_id;
    unsigned int to_id;
    unsigned int cmd;
};

int id;

void setup() {

    Serial.begin(9600);

    pinMode(LED_PIN,OUTPUT);

    vw_set_ptt_inverted(true);
    vw_set_tx_pin(TX_PIN);
    vw_set_rx_pin(RX_PIN);
    vw_setup(R_SPEED);
    vw_rx_start();

    id = EEPROM.read(0);
    if (id == 255 || id == 0) {
        randomSeed(analogRead(0));
        int randNumber = random(0, 255);
        Serial.print("NEW RND ID: ");
        Serial.println(randNumber);
        //EEPROM.write(0, randNumber);
    }

    //Serial.print("ID: ");
    //Serial.println(id);
}

void loop() {
    checkIncomingData();
    delay(20);
}

void sendMessage (int cmd) {
    BASE_MSG msg = {id, 0, cmd};
    //Serial.println(msg);
    vw_send((uint8_t *)&msg, sizeof(msg));
    vw_wait_tx();
    //digitalWrite(LED_PIN, cmd);
    Serial.println("Send");
}

void checkIncomingData() {
    //Serial.println("Checking...");
    BASE_MSG receivedData;
    uint8_t msgSize = sizeof(receivedData);

    if (vw_get_message((uint8_t *)&receivedData, &msgSize)) {
        parseIncCmd(receivedData.cmd);
        //digitalWrite(LED_PIN, receivedData.cmd);
        //sendMessage(!receivedData.cmd);
    }
}

void parseIncCmd(int cmd) {
    Serial.print("CMD: ");
    Serial.println(cmd);
}