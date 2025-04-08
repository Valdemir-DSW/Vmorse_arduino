#include "Vmorse.h"

Vmorse::Vmorse(uint8_t txPin, uint8_t rxPin)
    : _txPin(txPin), _rxPin(rxPin), _unitTime(100), _lastReadTime(0) {}

void Vmorse::begin(unsigned int unitTime) {
    _unitTime = unitTime;
    pinMode(_txPin, OUTPUT);       // fora do código Morse original
    pinMode(_rxPin, INPUT);        // fora do código Morse original
    digitalWrite(_txPin, LOW);     // fora do código Morse original
}

void Vmorse::send(const char* message) {
    for (int i = 0; message[i] != '\0'; i++) {
        transmitChar(tolower(message[i]));
        delay(_unitTime * 3);
    }
}

bool Vmorse::available() {
    checkReception();               // fora do código Morse original
    return _receiveBuffer.length() > 0;
}

String Vmorse::read() {
    String result = _receiveBuffer;
    _receiveBuffer = "";            // fora do código Morse original
    return result;
}

// ---- Transmissão ----

void Vmorse::transmitSymbol(char symbol) {
    digitalWrite(_txPin, HIGH); // fora do código Morse original
    delay(symbol == '.' ? _unitTime : _unitTime * 3);
    digitalWrite(_txPin, LOW);  // fora do código Morse original
    delay(_unitTime);
}

void Vmorse::transmitChar(char c) {
    if (c == ' ') {
        delay(_unitTime * 7);
        return;
    }

    String code = encodeChar(c);
    for (int i = 0; i < code.length(); i++) {
        transmitSymbol(code[i]);
    }
}

// ---- Codificação ----

String Vmorse::encodeChar(char c) {
    switch (c) {
        // Letras (Morse padrão)
        case 'a': return ".-";
        case 'b': return "-...";
        case 'c': return "-.-.";
        case 'd': return "-..";
        case 'e': return ".";
        case 'f': return "..-.";
        case 'g': return "--.";
        case 'h': return "....";
        case 'i': return "..";
        case 'j': return ".---";
        case 'k': return "-.-";
        case 'l': return ".-..";
        case 'm': return "--";
        case 'n': return "-.";
        case 'o': return "---";
        case 'p': return ".--.";
        case 'q': return "--.-";
        case 'r': return ".-.";
        case 's': return "...";
        case 't': return "-";
        case 'u': return "..-";
        case 'v': return "...-";
        case 'w': return ".--";
        case 'x': return "-..-";
        case 'y': return "-.--";
        case 'z': return "--..";

        // Números (Morse padrão)
        case '1': return ".----";
        case '2': return "..---";
        case '3': return "...--";
        case '4': return "....-";
        case '5': return ".....";
        case '6': return "-....";
        case '7': return "--...";
        case '8': return "---..";
        case '9': return "----.";
        case '0': return "-----";

        // Pontuações (Morse padrão)
        case '.': return ".-.-.-";
        case ',': return "--..--";
        case '?': return "..--..";
        case '!': return "-.-.--";
        case '\'': return ".----.";  // apóstrofo
        case '\"': return ".-..-.";  // aspas
        case ':': return "---...";   // dois pontos
        case ';': return "-.-.-.";   // ponto e vírgula
        case '=': return "-...-";    // igual
        case '+': return ".-.-.";    // mais
        case '-': return "-....-";   // menos
        case '/': return "-..-.";    // barra
        case '(': return "-.--.";    // parêntese esquerdo
        case ')': return "-.--.-";   // parêntese direito
        case '&': return ".-...";    // e comercial

        // Extras — fora do código Morse original
        case '*': return "-.-";      // reutilizado do "k" (fora do padrão)
        case '%': return "--.-.";    // código inventado (fora do padrão)
        case '$': return "...-..-";  // moderno, fora do padrão antigo
        case '@': return ".--.-.";   // moderno, fora do padrão antigo

        default: return "";
    }
}

// ---- Decodificação ----

char Vmorse::decodeSymbol(String code) {
    // Letras
    for (char c = 'a'; c <= 'z'; c++) {
        if (encodeChar(c) == code) return c;
    }

    // Números
    for (char c = '0'; c <= '9'; c++) {
        if (encodeChar(c) == code) return c;
    }

    // Símbolos e operadores
    const char symbols[] = {
        '.', ',', '?', '!', '\'', '\"', ':', ';',
        '=', '+', '-', '/', '(', ')', '&', '*', '%', '$', '@'
    };

    for (char symbol : symbols) {
        if (encodeChar(symbol) == code) return symbol;
    }

    return '?';
}

// ---- Recepção simplificada (duplex) ----

void Vmorse::checkReception() {
    static bool receiving = false;       // fora do código Morse original
    static unsigned long pulseStart = 0; // fora do código Morse original
    static String symbolBuffer = "";     // fora do código Morse original

    int val = digitalRead(_rxPin);       // fora do código Morse original
    unsigned long now = millis();

    if (val == HIGH && !receiving) {
        pulseStart = now;
        receiving = true;
    }

    if (val == LOW && receiving) {
        unsigned long duration = now - pulseStart;
        receiving = false;

        if (duration >= _unitTime * 3) {
            symbolBuffer += '-';
        } else if (duration >= _unitTime) {
            symbolBuffer += '.';
        }

        _lastReadTime = now;
    }

    if (now - _lastReadTime > _unitTime * 5 && symbolBuffer.length() > 0) {
        char decoded = decodeSymbol(symbolBuffer);
        _receiveBuffer += decoded;
        symbolBuffer = "";
    }
}
