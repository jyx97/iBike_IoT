#include <WiFi.h>
#include <PubSubClient.h>
#include <map>

// --- Configurações Wi-Fi ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- Configurações MQTT ---
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "sistema/motos";

// --- Pinos ---
#define PIN_SENSOR 14
#define LED_AUTORIZADA 2
#define LED_ALERTA 4

WiFiClient espClient;
PubSubClient client(espClient);

// --- Dicionário motos e seus status ---
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

// Sensor Presença
class SensorPresenca {
  private:
    int pin;
    unsigned long ultimo = 0;
    const unsigned long intervalo = 3000;

  public:
    SensorPresenca(int p) : pin(p) {
      pinMode(pin, INPUT);
    }

    bool detectado() {
      int estado = digitalRead(pin);
      unsigned long agora = millis();
      if (estado == HIGH && (agora - ultimo > intervalo)) {
        ultimo = agora;
        return true;
      }
      return false;
    }
};

SensorPresenca sensor(PIN_SENSOR);

// Piscar LED
void piscaLed(int pino, int vezes, int intervalo) {
  for (int i = 0; i < vezes; i++) {
    digitalWrite(pino, HIGH);
    delay(intervalo);
    digitalWrite(pino, LOW);
    delay(intervalo);
  }
}

// Simula leitura de uma moto passando
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

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a rede: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tentativas++;
    if (tentativas > 20) {
      Serial.println("Falha ao conectar Wi-Fi");
      return;
    }
  }
  Serial.println("");
  Serial.println("Wi-Fi conectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop até conectar
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    if (client.connect("ESP32MotoMonitor")) {
      Serial.println("Conectado ao MQTT");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_AUTORIZADA, OUTPUT);
  pinMode(LED_ALERTA, OUTPUT);
  pinMode(PIN_SENSOR, INPUT);

  Serial.println("Sistema iniciado - Monitoramento de motos");
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);

  randomSeed(analogRead(0)); // Inicializa gerador randomico
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (sensor.detectado()) {
    String placa = lerPlacaMoto();
    Serial.println("Movimento detectado - Moto: " + placa);

    if (motos.count(placa) == 0) {
      Serial.println("[ERRO] Moto não cadastrada no sistema: " + placa);
      piscaLed(LED_ALERTA, 3, 500);
      client.publish(mqtt_topic, ("ERRO: Moto nao cadastrada: " + placa).c_str());
      return;
    }

    String status = motos[placa];
    String mensagem;

    if (status == "EM_USO") {
      Serial.println("[SISTEMA] Moto autorizada: " + placa);
      piscaLed(LED_AUTORIZADA, 2, 300);
      mensagem = "Autorizada: " + placa;
    } else {
      Serial.println("[ALERTA] Inconsistencia detectada! Moto com status '" + status + "': " + placa);
      piscaLed(LED_ALERTA, 5, 200);
      mensagem = "Alerta: " + placa + " status: " + status;
    }

    client.publish(mqtt_topic, mensagem.c_str());
  }
}
