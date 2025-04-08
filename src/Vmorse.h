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
    void checkReception();

private:
    uint8_t _txPin, _rxPin;
    unsigned int _unitTime;
    unsigned long _lastReadTime;
    String _receiveBuffer;

    bool _receiving;     // <-- Adicionado
    bool _inMessage;     // <-- Adicionado

    void transmitChar(char c);
    void transmitSymbol(char symbol);
    String encodeChar(char c);
    char decodeSymbol(String code);
    
};

#endif
