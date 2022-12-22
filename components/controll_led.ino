#include "M5Atom.h"
#include <stdbool.h>


void ledTask(void *pvParameters) {
  while (true) {
    // Obter informações da fila
    xQueueReceive(xQueueButton, &info_button, portMAX_DELAY);
    // Processando o status do botão
    if (info_button.status == true) {
      switch (info_button.cod) {
        case 1:
          M5.dis.fillpix(0xff0000); // vermelho
          Serial.println("ALERTA.\n");
          bright(100);
          break;
        case 2:
          M5.dis.fillpix(0x00F00F); // verde
          Serial.println("Ligado.\n");
          bright(100); 
          break;
      }     
    } else {
      Serial.println("Desligado.\n");
      M5.dis.clear();
    } 
  }
}