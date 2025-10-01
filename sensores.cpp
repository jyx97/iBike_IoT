#include "config.h"

// Definição da variável global para o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Função para inicializar o sensor DHT
void inicializarDHT() {
    dht.begin();
    Serial.println("Sensor DHT iniciado");
}

// Função que retorna valores de temperatura, umidade e alertas
String lerDHT() {
    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    
    // Depuração
    Serial.print("DHT - Temperatura: ");
    Serial.print(temperatura, 1);
    Serial.print("°C, Umidade: ");
    Serial.print(umidade, 1);
    Serial.println("%");
    
    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha ao ler dados do DHT");
        return "ERRO: Falha leitura DHT";
    }
    
    // Sempre inclui valores numéricos
    String mensagem = "Temp: " + String(temperatura, 1) + "C, Umidade: " + String(umidade, 1) + "%";
    String alerta = "";
    bool temAlerta = false;
    
    if (temperatura > TEMP_MAX) {
        alerta += "ALERTA: Moto muito quente! Temperaturas elevadas podem danificar componentes do motor.";
        temAlerta = true;
    }
    if (temperatura < TEMP_MIN) {
        if (temAlerta) alerta += " ";
        alerta += "ALERTA: Moto muito fria! Temperaturas baixas podem dificultar a lubrificação.";
        temAlerta = true;
    }
    if (umidade > UMIDADE_MAX) {
        if (temAlerta) alerta += " ";
        alerta += "ALERTA: Umidade alta! Pode causar ferrugem e danos elétricos.";
    }
    
    if (alerta != "") {
        mensagem += " | " + alerta;
    }
    
    return mensagem;
}

// Implementação da classe SensorPresenca
SensorPresenca::SensorPresenca(int p) : pin(p) {
    pinMode(pin, INPUT);
}

bool SensorPresenca::detectado() {
    int estado = digitalRead(pin);
    unsigned long agora = millis();
    if (estado == HIGH && (agora - ultimo > intervalo)) {
        ultimo = agora;
        return true;
    }
    return false;
}