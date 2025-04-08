#include "Vmorse.h"

Vmorse::Vmorse(uint8_t txPin, uint8_t rxPin)
    : _txPin(txPin), _rxPin(rxPin), _unitTime(100), _lastReadTime(0), _receiving(false), _inMessage(false) {}

void Vmorse::begin(unsigned int unitTime) {
    _unitTime = unitTime;
    pinMode(_txPin, OUTPUT);
    pinMode(_rxPin, INPUT_PULLUP);
    digitalWrite(_txPin, HIGH); // repouso
}

void Vmorse::send(const char* message) {
    // Sinal de início de transmissão
    digitalWrite(_txPin, LOW);
    delay(_unitTime * 10);
    digitalWrite(_txPin, HIGH);
    delay(_unitTime * 3);

    for (int i = 0; message[i] != '\0'; i++) {
        char c = tolower(message[i]);

        if (c == ' ') {
            delay(_unitTime * 7);  // Espaço entre palavras
            continue;
        }

        transmitChar(c);
        delay(_unitTime * 3);  // Espaço entre letras
    }

    // Sinal de fim de transmissão
    digitalWrite(_txPin, LOW);
    delay(_unitTime * 5);
    digitalWrite(_txPin, HIGH);
}

bool Vmorse::available() {
    checkReception();
    return _receiveBuffer.length() > 0;
}

String Vmorse::read() {
    String result = _receiveBuffer;
    _receiveBuffer = "";
    return result;
}

void Vmorse::transmitSymbol(char symbol) {
    digitalWrite(_txPin, LOW);
    delay(symbol == '.' ? _unitTime : _unitTime * 3);
    digitalWrite(_txPin, HIGH);
    delay(_unitTime);
}

void Vmorse::transmitChar(char c) {
    String code = encodeChar(c);
    for (int i = 0; i < code.length(); i++) {
        transmitSymbol(code[i]);
    }
}

String Vmorse::encodeChar(char c) {
    switch (c) {
        case 'a': return ".-"; case 'b': return "-..."; case 'c': return "-.-.";
        case 'd': return "-.."; case 'e': return "."; case 'f': return "..-.";
        case 'g': return "--."; case 'h': return "...."; case 'i': return "..";
        case 'j': return ".---"; case 'k': return "-.-"; case 'l': return ".-..";
        case 'm': return "--"; case 'n': return "-."; case 'o': return "---";
        case 'p': return ".--."; case 'q': return "--.-"; case 'r': return ".-.";
        case 's': return "..."; case 't': return "-"; case 'u': return "..-";
        case 'v': return "...-"; case 'w': return ".--"; case 'x': return "-..-";
        case 'y': return "-.--"; case 'z': return "--..";
        case '1': return ".----"; case '2': return "..---"; case '3': return "...--";
        case '4': return "....-"; case '5': return "....."; case '6': return "-....";
        case '7': return "--..."; case '8': return "---.."; case '9': return "----.";
        case '0': return "-----";
        case '.': return ".-.-.-"; case ',': return "--..--"; case '?': return "..--..";
        case '!': return "-.-.--"; case '\'': return ".----."; case '\"': return ".-..-.";
        case ':': return "---..."; case ';': return "-.-.-."; case '=': return "-...-";
        case '+': return ".-.-."; case '-': return "-....-"; case '/': return "-..-.";
        case '(': return "-.--."; case ')': return "-.--.-"; case '&': return ".-...";
        case '*': return "-.-"; case '%': return "--.-."; case '$': return "...-..-";
        case '@': return ".--.-.";
        default: return "";
    }
}

char Vmorse::decodeSymbol(String code) {
    for (char c = 'a'; c <= 'z'; c++) if (encodeChar(c) == code) return c;
    for (char c = '0'; c <= '9'; c++) if (encodeChar(c) == code) return c;

    const char symbols[] = {
        '.', ',', '?', '!', '\'', '\"', ':', ';',
        '=', '+', '-', '/', '(', ')', '&', '*', '%', '$', '@'
    };

    for (char symbol : symbols) {
        if (encodeChar(symbol) == code) return symbol;
    }

    return '?';
}

void Vmorse::checkReception() {
    static String symbolBuffer = "";
    static unsigned long pulseStart = 0;
    int val = digitalRead(_rxPin);
    unsigned long now = millis();

    if (val == LOW && !_receiving) {
        pulseStart = now;
        _receiving = true;
    }

    if (val == HIGH && _receiving) {
        unsigned long duration = now - pulseStart;
        _receiving = false;

        if (!_inMessage && duration >= _unitTime * 9) {
            _inMessage = true;  // início da transmissão detectado
            _receiveBuffer = "";
        } else if (_inMessage && duration >= _unitTime * 4 && duration <= _unitTime * 6) {
            _inMessage = false; // fim da transmissão
        } else if (_inMessage) {
            if (duration >= _unitTime * 3) {
                symbolBuffer += '-';
            } else if (duration >= _unitTime) {
                symbolBuffer += '.';
            }

            _lastReadTime = now;
        }
    }

    if (_inMessage && now - _lastReadTime > _unitTime * 5 && symbolBuffer.length() > 0) {
        char decoded = decodeSymbol(symbolBuffer);
        _receiveBuffer += decoded;
        symbolBuffer = "";
    }

    if (_inMessage && now - _lastReadTime > _unitTime * 10) {
        _receiveBuffer += ' ';  // adiciona espaço entre palavras se houver silêncio longo
        _lastReadTime = now;
    }
}
