#include <VirtualWire.h>
#include <EEPROM.h>

#define RX_PIN 11
#define TX_PIN 12
#define LED_PIN 13
#define R_SPEED 200
#define BTN_PIN 7

struct BASE_MSG {
    unsigned short int from_id;
    unsigned short int to_id;
    unsigned short int cmd;
};

int main_security = 0;

int id;

int str = 1;

void setup() {
    Serial.begin(9600);

    pinMode(LED_PIN,OUTPUT);
    pinMode(BTN_PIN, INPUT);

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
        EEPROM.write(0, randNumber);
    }
    //sendMessage(str);
    //Serial.print("ID: ");
    //Serial.println(id);

}

void loop () {
    checkIncomingData();
    checkButtons();
    delay(20);
}

void sendMessage (int cmd) {
    BASE_MSG msg = {id, 0, cmd};
    //Serial.println(msg);
    Serial.print("Size of msg: ");
    Serial.println(sizeof(msg));
    vw_send((uint8_t *)&msg, sizeof(msg));
    vw_wait_tx();
    //digitalWrite(LED_PIN, cmd);
    Serial.print("Send: ");
    Serial.println(cmd);
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

void checkButtons() {
    int btn_state = digitalRead(BTN_PIN);
    Serial.print("BTN: ");
    Serial.println(btn_state);

    if (btn_state > 0) {
        int cmd = 1 * 10 + main_security;
        sendMessage(cmd);
    }
}