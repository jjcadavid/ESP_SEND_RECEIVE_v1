#include <WiFi.h>
#include <esp_now.h>

uint8_t broadcastAddress[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

typedef struct {
  uint8_t comando;
} mensaje_t;

mensaje_t mensaje;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  esp_now_add_peer(&peerInfo);

  Serial.println("Control remoto listo");
}

void loop() {
  // ENVIAR ON
  mensaje.comando = 1;
  esp_now_send(broadcastAddress, (uint8_t*)&mensaje, sizeof(mensaje));
  Serial.println("ON enviado");
  delay(2000);

  // ENVIAR OFF
  mensaje.comando = 0;
  esp_now_send(broadcastAddress, (uint8_t*)&mensaje, sizeof(mensaje));
  Serial.println("OFF enviado");
  delay(2000);
}
