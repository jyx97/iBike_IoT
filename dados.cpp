#include <map>
#include <Preferences.h>
#include <Arduino.h>
#include "config.h"

// Definição do dicionário de motos e seus status
std::map<String, String> motos = {
    {"FHG1A23", "EM_USO"},
    {"BCZ5D21", "MANUTENCAO"},
    {"JLM3F45", "PATIO"},
    {"QWE9B12", "EM_USO"},
    {"ASD7C98", "PENDENTE"},
    {"ZXC2D34", "EM_USO"},
    {"RTY4F56", "PATIO"},
    {"HJK6A78", "EM_USO"},
    {"VBN8E90", "MANUTENCAO"},
    {"MNB5C43", "EM_USO"},
    {"POI3D21", "PENDENTE"},
    {"LKU7A65", "EM_USO"},
    {"GHJ1F88", "PATIO"},
    {"WER2C17", "EM_USO"},
    {"TYU4D52", "MANUTENCAO"},
    {"SDF6E31", "EM_USO"},
    {"EDC8A94", "PENDENTE"},
    {"RFV3C12", "EM_USO"},
    {"TGB9D65", "PATIO"},
    {"YHN5F77", "EM_USO"}
};

// Definição da variável global para Preferences
Preferences prefs;

// Função para inicializar Preferences (EEPROM/NVS)
void inicializarPreferences() {
    prefs.begin(PREFS_NAMESPACE, false); // Namespace para armazenamento
    Serial.println("Preferences (EEPROM/NVS) inicializado com sucesso");
}

// Função para registrar evento no EEPROM/NVS
void logEvento(const String &placa, const String &status, const String &ledStatus) {
    unsigned long timestamp = millis();

    // Incrementa contadores de acordo com o status
    if (status == "EM_USO") {
        prefs.putUInt("autorizadas", prefs.getUInt("autorizadas", 0) + 1);
    } else {
        prefs.putUInt("alertas", prefs.getUInt("alertas", 0) + 1);
    }

    // Salva último evento
    prefs.putString("ultima_placa", placa);
    prefs.putString("ultimo_status", status);
    prefs.putString("ultimo_led", ledStatus);
    prefs.putULong("ultimo_timestamp", timestamp);

    Serial.println("Evento registrado: " + String(timestamp) + "," + placa + "," + status + "," + ledStatus);
}

// Função para ler e exibir o histórico do EEPROM/NVS
void lerHistorico() {
    Serial.println("Histórico (EEPROM/NVS):");
    Serial.println("Autorizadas: " + String(prefs.getUInt("autorizadas", 0)));
    Serial.println("Alertas: " + String(prefs.getUInt("alertas", 0)));
    Serial.println("Último evento: " +
                   String(prefs.getULong("ultimo_timestamp", 0)) + "," +
                   prefs.getString("ultima_placa", "") + "," +
                   prefs.getString("ultimo_status", "") + "," +
                   prefs.getString("ultimo_led", ""));
}