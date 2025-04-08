#include "Vmorse.h"

Vmorse morse(8, 9); // TX no pino 8, RX no pino 9

void setup() {
    Serial.begin(9600);
    morse.begin(150);  // unidade de tempo em ms
    morse.send("3+5=8! ola, mundo!");
}

void loop() {
    if (morse.available()) {
        String msg = morse.read();
        Serial.print("Recebido: ");
        Serial.println(msg);
    }
}
