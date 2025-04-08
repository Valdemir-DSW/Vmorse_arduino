#ifndef VMORSE_H
#define VMORSE_H

#include <Arduino.h>

class Vmorse {
public:
    Vmorse(uint8_t txPin, uint8_t rxPin);

    void begin(unsigned int unitTime = 100);
    void send(const char* message);
    bool available();
    String read();

private:
    uint8_t _txPin, _rxPin;
    unsigned int _unitTime;
    String _receiveBuffer;         // fora do código Morse original
    unsigned long _lastReadTime;   // fora do código Morse original

    String encodeChar(char c);
    char decodeSymbol(String code);

    void transmitSymbol(char symbol);
    void transmitChar(char c);
    void checkReception();         // fora do código Morse original
};

#endif
