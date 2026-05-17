#include <WiFi.h>
#include <esp_now.h>

const int BUZZER_PIN = 25;
int pwmChannel;

// Estructura del mensaje
typedef struct {
  uint8_t comando;   // 1 = ON, 0 = OFF
} mensaje_t;

mensaje_t mensaje;

// Callback de recepción (API nueva)
void onReceive(const esp_now_recv_info *info,
               const uint8_t *data,
               int len) {

  // Copia los datos recibidos
  memcpy(&mensaje, data, sizeof(mensaje));

  // ---- SALIDA POR SERIAL ----
  Serial.print("Mensaje recibido | Comando: ");
  Serial.print(mensaje.comando);
  Serial.print(" | Desde MAC: ");

  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", info->src_addr[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();

  // ---- ACCIÓN ----
  if (mensaje.comando == 1) {
    ledcWriteTone(pwmChannel, 2000);  // Beep
  } else {
    ledcWriteTone(pwmChannel, 0);     // Silencio
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 Receptora iniciando...");

  // PWM para buzzer
  pwmChannel = ledcAttach(BUZZER_PIN, 2000, 8);

  // ESP-NOW
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error inicializando ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(onReceive);

  Serial.println("Receptora lista, esperando mensajes...");
}

void loop() {
  // No se necesita código aquí
}
