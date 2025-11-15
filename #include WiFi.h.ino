#include <WiFi.h>
#include <WebServer.h>

// --- CONFIGURAÇÕES WI-FI ---
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// --- PINOS ---
const int MQ2_PIN = 34;    // pino analógico do MQ-2 (AOUT)
const int LED_PIN = 22;     // pino digital do LED

// --- OBJETO SERVIDOR WEB ---
WebServer server(80);

// --- VARIÁVEIS GLOBAIS ---
int gasValue = 0;
String statusAr = "Desconhecido";

// --- FUNÇÃO PARA GERAR PÁGINA HTML ---
String getPage() {
  String html = "<html><body style='font-family:sans-serif;text-align:center;'>";
  html += "<h2>Monitor de Qualidade do Ar - MQ2</h2>";
  html += "<p><b>Valor do Sensor:</b> " + String(gasValue) + "</p>";
  html += "<p><b>Status do Ar:</b> " + statusAr + "</p>";
  html += "<p>Atualize a página para ver o valor mais recente.</p>";
  html += "</body></html>";
  return html;
}

// --- CONFIGURAÇÃO INICIAL ---
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  Serial.print("Conectando-se ao Wi-Fi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println("\nConectado!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());

  // Inicia servidor web
  server.on("/", []() {
    server.send(200, "text/html", getPage());
  });
  server.begin();
  Serial.println("Servidor Web Iniciado!");
}


void loop() {
  gasValue = analogRead(MQ2_PIN);

  // Ajuste dos limites conforme necessidade e calibração do sensor
  if (gasValue < 2300) {
    statusAr = "Ar Limpo 😌";
    digitalWrite(LED_PIN, LOW);
  } else if (gasValue < 3600) {
    statusAr = "Atenção ⚠️ - Poluição Moderada";
    digitalWrite(LED_PIN, LOW);
  } else {
    statusAr = "Perigo 🚨 - Alta Poluição!";
    digitalWrite(LED_PIN, HIGH);
  }

  // Exibe no Serial Monitor
  Serial.print("Valor MQ2: ");
  Serial.print(gasValue);
  Serial.print(" | Status: ");
  Serial.println(statusAr);

  // Atualiza servidor
  server.handleClient();

  delay(1000);
}