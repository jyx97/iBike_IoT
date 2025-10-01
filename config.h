#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <DHT.h>
#include <map>
#include <Preferences.h>

// --- Configurações do sensor DHT ---
#define DHTPIN 13              // Pino do sensor DHT
#define DHTTYPE DHT11         // Tipo de sensor DHT (DHT11, DHT22, etc.)
#define TEMP_MAX 35.0         // Limite superior de temperatura (°C)
#define TEMP_MIN 0.0          // Limite inferior de temperatura (°C)
#define UMIDADE_MAX 80.0      // Limite superior de umidade (%)

// --- Pinos ---
#define PIN_SENSOR 14         // Pino do sensor PIR
#define LED_AUTORIZADA 2      // Pino do LED para moto autorizada
#define LED_ALERTA 4          // Pino do LED para alertas

// --- Configurações Wi-Fi ---
#define WIFI_SSID "Wokwi-GUEST"  // SSID da rede Wi-Fi
#define WIFI_PASSWORD ""         // Senha da rede Wi-Fi
#define WIFI_MAX_ATTEMPTS 20     // Máximo de tentativas de conexão Wi-Fi
#define WIFI_CONNECT_DELAY 500   // Delay entre tentativas de conexão Wi-Fi (ms)

// --- Configurações MQTT ---
#define MQTT_SERVER "broker.hivemq.com" // Endereço do broker MQTT
#define MQTT_PORT 1883                  // Porta do broker MQTT
#define MQTT_TOPIC "sistema/motos"      // Tópico MQTT
#define MQTT_RECONNECT_DELAY 5000       // Delay entre tentativas de reconexão MQTT (ms)
#define MQTT_CLIENT_ID "ESP32MotoMonitor" // ID do cliente MQTT

// --- Configurações gerais ---
#define PIR_INTERVAL 3000       // Intervalo de debounce do sensor PIR (ms)
#define LED_PISCA_INTERVAL 200  // Intervalo para piscar LEDs (ms)
#define LED_PISCA_AUTORIZADA 2  // Número de piscadas para moto autorizada
#define LED_PISCA_ALERTA 5      // Número de piscadas para alertas

// --- Configurações de Preferences ---
#define PREFS_NAMESPACE "iBike" // Namespace para armazenamento em Preferences

// Declaração do dicionário de motos e seus status
extern std::map<String, String> motos;

// Declaração da variável global para Preferences
extern Preferences prefs;

// Declaração da variável global para o sensor DHT
extern DHT dht;

// Estrutura de posição GPS
struct Posicao {
    float latitude;
    float longitude;
};

// Classe para sensor de presença (PIR)
class SensorPresenca {
private:
    int pin; // Pino conectado ao sensor PIR
    unsigned long ultimo = 0; // Última detecção registrada
    const unsigned long intervalo = PIR_INTERVAL; // Intervalo mínimo entre detecções
public:
    SensorPresenca(int p);
    bool detectado();
};

// Declarações de funções
void inicializarDHT();
String lerDHT();
Posicao atualizarPosicao();
bool deveEnviarCoordenadas(String status);
void inicializarPreferences();
void logEvento(const String& placa, const String& status, const String& ledStatus);
void lerHistorico();

#endif