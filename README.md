# Vmorse_arduino
Vmorse_arduino biblioteca de código morse Arduino


Cada Arduino utiliza dois pinos:

- **TX_PIN**: saída digital (usado para enviar sinais).
- **RX_PIN**: entrada digital (usado para receber sinais).

Você pode utilizar LEDs, fotoresistores (LDRs) ou apenas conexão direta com fios. A biblioteca detecta sinais `HIGH` e `LOW` e interpreta como traços e pontos em código Morse.

---

## 🧠 Exemplo básico

```cpp
#include "Vmorse.h"

Vmorse morse(8, 9); // TX no pino 8, RX no pino 9

void setup() {
    Serial.begin(9600);
    morse.begin(150);  // tempo base de unidade em milissegundos
    morse.send("Ola, mundo!");
}

void loop() {
    if (morse.available()) {
        String msg = morse.read();
        Serial.print("Recebido: ");
        Serial.println(msg);
    }
}
