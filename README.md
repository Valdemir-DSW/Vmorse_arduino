# Vmorse - Biblioteca de Comunicação Morse para Arduino

**Vmorse** é uma biblioteca criada para permitir comunicação entre dois Arduinos utilizando sinais de código Morse via pinos digitais comuns. Foi feita para fins didáticos, com simplicidade e flexibilidade em mente, e **não depende de UART ou comunicação serial tradicional**.

---

## 🔌 Como conectar

Você precisa de dois Arduinos com fios conectando os pinos de transmissão (`TX`) e recepção (`RX`) entre eles:

```
[Arduino A] TX_PIN ----> RX_PIN [Arduino B]
[Arduino B] TX_PIN ----> RX_PIN [Arduino A]
```

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
```

---

## ⚙️ Recursos

- Comunicação duplex (transmissão e recepção simultânea).
- Suporte a:
  - Letras (A-Z)
  - Números (0-9)
  - Pontuação: `. , ? ! ' " : ; = + - / ( ) & @ $ % *`  
  - (Caracteres extras fora do padrão Morse estão comentados no código)
- Tempo base configurável.
- Detecção automática de traço (dash) e ponto (dot) com base na duração do pulso.

---

## 📄 Licença

Este projeto foi desenvolvido por **Valdemir** e está **liberado para uso livre**, inclusive **para fins didáticos, acadêmicos ou comerciais**.

> ⚠️ **Aviso**: Esta biblioteca é fornecida **sem nenhuma garantia de funcionamento perfeito**. É um projeto experimental e didático, podendo conter limitações e imprecisões.

---

## 💡 Sugestões

Quer usar sinais de luz ou som em vez de fios diretos? Sem problemas! Basta adaptar a parte física, e o código continuará funcionando com qualquer tipo de sinal binário (luz ligada/desligada, som curto/longo etc).

---

📬 Dúvidas, sugestões ou melhorias? Contribuições são bem-vindas!
