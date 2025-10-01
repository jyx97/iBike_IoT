#include <Arduino.h>
#include "config.h"

// Função que simula atualização da posição GPS
Posicao atualizarPosicao() {
    Posicao pos;
    // Simula pequenas variações na posição (aproximadamente ~10m)
    pos.latitude = -23.5 + (random(0, 1000) / 10000.0);
    pos.longitude = -46.6 + (random(0, 1000) / 10000.0);
    return pos;
}

// Função que determina se deve enviar coordenadas
bool deveEnviarCoordenadas(String status) {
    if (status != "EM_USO") {
        Serial.println("Status fora de uso, coordenadas serão enviadas");
        return true;
    } else {
        Serial.println("Status EM_USO, coordenadas não serão enviadas");
        return false;
    }
}