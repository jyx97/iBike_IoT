Aqui está o texto revisado e com a nova seção **"👥 Integrantes"** adicionada no final. Também removi a duplicação da seção "📋 Exemplo de Saída Serial".

---

# 🛵 iBike – Monitoramento Inteligente de Motos em Pátio com ESP32 e MQTT

O **iBike** é um sistema de monitoramento que recebe automaticamente dados de identificação de motos via IoT (simulados neste projeto) e verifica o **status da moto no pátio**. O sistema utiliza um **ESP32**, um **sensor de presença**, comunicação **MQTT**, e dois LEDs para indicar se a moto está com permissão para circular ou se há alguma inconsistência.

---

## 📲 Visão Geral

* Motos enviam automaticamente suas **placas via IoT**
* O **ESP32 com sensor de presença** detecta a chegada da moto no ponto de checagem
* O sistema cruza a placa recebida com um **cadastro interno de placas e seus status**
* A mensagem é publicada no **broker MQTT**, sendo visualizada via **dashboard Node-RED**

---

## 🧠 Lógica do Projeto

* O ESP32 não identifica a placa da moto diretamente — ele **recebe os dados da moto via rede (simulados)**
* Com a placa recebida, o ESP32:

  * Valida se o status da moto permite a movimentação
  * Publica a informação em um **tópico MQTT**
  * Aciona o LED correspondente:

    * ✅ Autorizada
    * ⚠️ Alerta por status inconsistente
    * ❌ Não cadastrada

---

## 🏷️ Exemplo de Status Cadastrados

* `EM_USO` – Moto pode sair do pátio
* `MANUTENCAO` – Moto em manutenção, bloqueada
* `PATIO` – Moto deve permanecer no pátio
* `PENDENTE` – Cadastro incompleto ou análise em andamento

---

## 🔌 Componentes Usados

* **ESP32**
* Sensor de presença (digital)
* LED verde (pino 2): Autorizada
* LED vermelho (pino 4): Alerta/Inconsistência
* Conexão Wi-Fi
* Broker MQTT público: `broker.hivemq.com`
* **Node-RED Dashboard** para visualização

---

## 📡 Tópico MQTT Utilizado

```
sistema/motos
```

---

## 📤 Fluxo da Informação

```plaintext
Moto (via IoT) → Envia placa → ESP32 detecta presença → Consulta status → Publica via MQTT → Node-RED exibe no painel
```

---

## 🧪 Simulação de Placas

No código, a leitura da placa é simulada com seleção aleatória. Em um ambiente real, essa placa seria enviada por cada moto (por exemplo, via ESP8266 acoplado à moto ou módulo RFID).

---

## 🖥️ Visualização com Node-RED

* Tópico monitorado: `sistema/motos`
* Dashboard exibe:

  * Histórico de eventos
  * Mensagens toast de alerta/autorização
  * Lista de status por placa

---

## 🚦 Comportamento dos LEDs

* **Verde (LED\_AUTORIZADA)**: Moto autorizada a sair
* **Vermelho (LED\_ALERTA)**:

  * Moto em manutenção, pátio ou pendente
  * Moto não cadastrada

---

## ▶️ Como Usar

1. Faça upload do código para o ESP32 (Arduino IDE ou PlatformIO)
2. Verifique o monitor serial
3. Configure o Node-RED com broker `broker.hivemq.com` e tópico `sistema/motos`
4. Visualize os eventos em tempo real

---

## 📋 Exemplo de Saída Serial

```
Sistema iniciado - iBike
Wi-Fi conectado!
Endereço IP: 192.168.0.105
Movimento detectado - Moto: JLM3F45
[ALERTA] Inconsistencia detectada! Moto com status 'PATIO': JLM3F45
```

---

## 👥 Integrantes

* Gabriel Dias Menezes - RM: 555019
* Júlia Soares Farias dos Santos – RM: 554609
