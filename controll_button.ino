#include "M5Atom.h"
#include <stdbool.h>

void buttonTask(void *pvParameters) {
  while (true) {
    if (M5.Btn.read()) {
      vTaskDelay(300/portTICK_RATE_MS);
      info_button.status = true;
      info_button.cod = 2;      
    }
    // Condição para botão de Alerta
    if (M5.Btn.pressedFor(3000)) {
      vTaskDelay(3000/portTICK_RATE_MS);
      info_button.status = true;      
      info_button.cod = 1;       
      Serial.println("Pressionou port 3 segundos!");
    }
    // Enviando informações para fila
    xQueueSend(xQueueButton, &info_button, portMAX_DELAY);
  }
}