# 🛵 iBike – Monitoramento Inteligente de Motos em Pátio com ESP32 e MQTT

O **iBike** é um sistema IoT para monitoramento de motos em um pátio, utilizando um **ESP32** para detectar a presença de motos, validar seus status, coletar dados de temperatura e umidade, e enviar coordenadas GPS. As informações são publicadas via **MQTT** e exibidas em um **dashboard Node-RED** com histórico, status atual, mapa, e gauges.

---

## 📲 Visão Geral

* Motos enviam automaticamente suas **placas via IoT** (simuladas neste projeto).
* O **ESP32 com sensor de presença** detecta a chegada da moto no ponto de checagem.
* O **sensor DHT11** mede temperatura e umidade, gerando alertas para condições fora dos limites.
* **Coordenadas GPS** são enviadas para motos com status inconsistente (e.g., `PENDENTE`).
* O sistema cruza a placa com um **cadastro interno de placas e seus status**.
* Dados são publicados no **broker MQTT** e exibidos em um **dashboard Node-RED**.

---

## 🧠 Lógica do Projeto

* O **ESP32** não identifica a placa diretamente — ele **recebe os dados da moto via rede (simulados)**.
* Com a placa recebida, o ESP32:
  * Valida se o status da moto permite a movimentação.
  * Lê **temperatura** e **umidade** do sensor DHT11.
  * Obtém **coordenadas GPS** para status específicos.
  * Publica a mensagem via MQTT com placa, status, GPS, temperatura, umidade, e alertas.
  * Aciona LEDs:
    * ✅ **Verde**: Moto autorizada.
    * ⚠️ **Vermelho**: Status inconsistente ou moto não cadastrada.
* O **Node-RED** exibe:
  * Histórico de eventos.
  * Status atual com placa, GPS, temperatura, e umidade.
  * Mapa com localização da moto.
  * Gauges de temperatura e umidade.
  * Notificações toast.

---

## 🏷️ Exemplo de Status Cadastrados

* `EM_USO` – Moto pode sair do pátio.
* `MANUTENCAO` – Moto em manutenção, bloqueada.
* `PATIO` – Moto deve permanecer no pátio.
* `PENDENTE` – Cadastro incompleto ou análise em andamento.

---

## 🔌 Componentes Usados

* **ESP32**.
* **Sensor de Presença** (digital).
* **Sensor DHT11** (temperatura e umidade, pino 13).
* **LED Verde** (pino 2): Autorizada.
* **LED Vermelho** (pino 4): Alerta/Inconsistência.
* **Conexão Wi-Fi**.
* **Broker MQTT público**: `broker.hivemq.com`.
* **Node-RED Dashboard** para visualização.

---

## 📡 Tópico MQTT Utilizado

```
sistema/motos
```

---

## 📤 Fluxo da Informação

```plaintext
Moto (via IoT) → Envia placa → ESP32 detecta presença → Consulta status → Lê DHT11 → Obtém GPS → Publica via MQTT → Node-RED exibe no painel
```

---

## 🧪 Simulação de Placas

* **Placas**: Simuladas com seleção aleatória no código. Em um ambiente real, seriam enviadas por ESP8266 ou RFID.
* **GPS**: Simulado com coordenadas fixas (e.g., `-23.462900, -46.505798`). Em hardware real, seria via módulo GPS.
* **DHT11**: Simulado no Wokwi ou lido via hardware real.

---

## 🖥️ Visualização com Node-RED

* **Tópico monitorado**: `sistema/motos`.
* **Dashboard**:
  * **Histórico**: Tabela com até 50 eventos (placa, status, GPS, temperatura, umidade, hora).
  * **Status Atual**: Card com a última mensagem, GPS, e dados ambientais.
  * **Mapa GPS**: Exibe localização (centrado em `-23.5, -46.6`, zoom 15).
  * **Gauges**: Temperatura (0–50°C) e umidade (0–100%).
  * **Notificações**: Mensagens toast para alertas/autorizações.

---

## 🚦 Comportamento dos LEDs

* **Verde (LED_AUTORIZADA)**: Moto com status `EM_USO`.
* **Vermelho (LED_ALERTA)**:
  * Moto em `MANUTENCAO`, `PATIO`, `PENDENTE`, ou não cadastrada.

---

## ▶️ Como Usar

1. Faça upload do código para o ESP32 (Arduino IDE ou PlatformIO).
   * Instale bibliotecas: `WiFi.h`, `PubSubClient.h`, `DHT.h` (Adafruit v1.4.4+).
   * Use os arquivos: `main.ino`, `sensores.cpp`, `motos.cpp`, `leds.cpp`, `gps.cpp`, `config.h`.
2. Verifique o monitor serial (115200 baud) para saídas como:
   ```
   Movimento detectado - Moto: POI3D21
   DHT - Temperatura: 3.0°C, Umidade: 6.4%
   Mensagem MQTT: Alerta: POI3D21 status: PENDENTE | GPS: (-23.462900,-46.505798) | Temp: 3.0C, Umidade: 6.4%
   ```
3. Configure o Node-RED com broker `broker.hivemq.com` e tópico `sistema/motos`.
   * Instale paletas: `node-red-dashboard`, `node-red-contrib-web-worldmap`.
   * Importe o fluxo JSON fornecido.
4. Visualize os eventos em tempo real em `http://<node-red-host>:1880/ui`.

---

## 📋 Exemplo de Saída Serial

```
Sistema iniciado - iBike
Wi-Fi conectado!
Endereço IP: 192.168.0.105
Movimento detectado - Moto: POI3D21
DHT - Temperatura: 3.0°C, Umidade: 6.4%
Coordenadas GPS: (-23.462900,-46.505798)
[ALERTA] Status inconsistente 'PENDENTE': POI3D21
Mensagem MQTT: Alerta: POI3D21 status: PENDENTE | GPS: (-23.462900,-46.505798) | Temp: 3.0C, Umidade: 6.4%
Conectando ao MQTT...Conectado ao MQTT
```

---

## 👥 Integrantes

* Gabriel Dias Menezes – RM: 555019
* Júlia Soares Farias dos Santos – RM: 554609
* Sofia Domingues Gonçalves – RM: 554920 