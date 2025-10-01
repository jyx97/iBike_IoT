#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

// Instâncias para Wi-Fi e MQTT
WiFiClient espClient;
PubSubClient client(espClient);

// Instancia o sensor de presença
SensorPresenca sensor(PIN_SENSOR);

// Pisca LED
void piscaLed(int pino, int vezes, int intervalo) {
    for (int i = 0; i < vezes; i++) {
        digitalWrite(pino, HIGH);
        delay(intervalo);
        digitalWrite(pino, LOW);
        delay(intervalo);
    }
}

// Simula leitura de placa de moto
String lerPlacaMoto() {
    String listaPlacas[] = {
        "FHG1A23", "BCZ5D21", "JLM3F45", "QWE9B12", "ASD7C98",
        "ZXC2D34", "RTY4F56", "HJK6A78", "VBN8E90", "MNB5C43",
        "POI3D21", "LKU7A65", "GHJ1F88", "WER2C17", "TYU4D52",
        "SDF6E31", "EDC8A94", "RFV3C12", "TGB9D65", "YHN5F77"
    };
    int indice = random(0, 20);
    return listaPlacas[indice];
}

// Conecta ao Wi-Fi
void setup_wifi() {
    delay(10);
    Serial.println("\nConectando a rede: " + String(WIFI_SSID));
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    int tentativas = 0;
    while (WiFi.status() != WL_CONNECTED && tentativas < WIFI_MAX_ATTEMPTS) {
        delay(WIFI_CONNECT_DELAY);
        Serial.print(".");
        tentativas++;
    }
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWi-Fi conectado!");
        Serial.println("Endereço IP: " + WiFi.localIP().toString());
    } else {
        Serial.println("\nFalha ao conectar Wi-Fi");
    }
}

// Conecta ao MQTT
void reconnect() {
    while (!client.connected()) {
        Serial.print("Conectando ao MQTT...");
        if (client.connect(MQTT_CLIENT_ID)) {
            Serial.println("Conectado ao MQTT");
        } else {
            Serial.print("Falha, rc=");
            Serial.print(client.state());
            Serial.println(" Tentando novamente em " + String(MQTT_RECONNECT_DELAY / 1000) + "s");
            delay(MQTT_RECONNECT_DELAY);
        }
    }
}

// Setup
void setup() {
    Serial.begin(115200);
    pinMode(LED_AUTORIZADA, OUTPUT);
    pinMode(LED_ALERTA, OUTPUT);
    pinMode(PIN_SENSOR, INPUT);

    inicializarPreferences();
    inicializarDHT();

    Serial.println("Sistema iniciado - Monitoramento de motos");

    setup_wifi();
    client.setServer(MQTT_SERVER, MQTT_PORT);

    randomSeed(analogRead(0)); // Inicializa gerador de números aleatórios
}

// Loop Principal
void loop() {
    if (!client.connected()) reconnect();
    client.loop();

    if (sensor.detectado()) {
        String placa = lerPlacaMoto();
        Serial.println("\nMovimento detectado - Moto: " + placa);

        String ledStatus;
        String mensagem;

        if (motos.count(placa) == 0) {
            Serial.println("[ERRO] Moto não cadastrada: " + placa);
            piscaLed(LED_ALERTA, LED_PISCA_ALERTA, LED_PISCA_INTERVAL);
            mensagem = "ERRO: Moto nao cadastrada: " + placa;
            ledStatus = "NAO";
            logEvento(placa, "NAO_CADASTRADA", ledStatus);
        } else {
            String status = motos[placa];
            if (status == "EM_USO") {
                Serial.println("[SISTEMA] Moto autorizada: " + placa);
                piscaLed(LED_AUTORIZADA, LED_PISCA_AUTORIZADA, LED_PISCA_INTERVAL);
                mensagem = "Autorizada: " + placa;
                ledStatus = "AUTORIZADA";
            } else {
                Serial.println("[ALERTA] Status inconsistente '" + status + "': " + placa);
                piscaLed(LED_ALERTA, LED_PISCA_ALERTA, LED_PISCA_INTERVAL);
                mensagem = "Alerta: " + placa + " status: " + status;
                ledStatus = "NAO";

                if (deveEnviarCoordenadas(status)) {
                    Posicao pos = atualizarPosicao();
                    mensagem += " | GPS: (" + String(pos.latitude, 6) + "," + String(pos.longitude, 6) + ")";
                    Serial.println("Coordenadas GPS: (" + String(pos.latitude, 6) + "," + String(pos.longitude, 6) + ")");
                }
            }
            logEvento(placa, status, ledStatus);
        }

        // Adiciona alertas de temperatura e umidade
        String alertaAmbiente = lerDHT();
        Serial.println("DHT Resultado: " + (alertaAmbiente == "" ? "Sem alertas" : alertaAmbiente));
        if (alertaAmbiente != "") {
            mensagem += " | " + alertaAmbiente;
        }

        Serial.println("Mensagem MQTT: " + mensagem);
        client.publish(MQTT_TOPIC, mensagem.c_str());
    }

    // Lê histórico via Serial
    if (Serial.available()) {
        String comando = Serial.readStringUntil('\n');
        if (comando == "ler") lerHistorico();
    }
}