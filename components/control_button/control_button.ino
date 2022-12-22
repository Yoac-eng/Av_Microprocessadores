#include "M5Atom.h"
#include <stdbool.h>

void buttonTask(void *pvParameters) {
  info_button_t info_button, info_cod;
  while (true) {
    if (M5.Btn.read()) {
      info_button.status = !info_button.status;
      info_button.cod = info_button.cod;
    }
    // Enviando informações para fila
    xQueueSend(xQueueButton, &info_button, portMAX_DELAY);
    xQueueSend(xQueueButton, &info_cod, portMAX_DELAY);
    vTaskDelay(250/portTICK_RATE_MS);
  }
  
}  