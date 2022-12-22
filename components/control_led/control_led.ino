#include "M5Atom.h"
#include <stdbool.h>

void ledTask(void *pvParameters) {
  info_button_t info_button;
  while (true) {
    // Pegando informações da fila
    xQueueReceive(xQueueButton, &info_button, portMAX_DELAY);
    xQueueReceive(xQueueButton, &info_cod, portMAX_DELAY);
    // Processando status e cod do butão
    if (info_button.status == true) {
      if(info_button.cod == true){
        M5.dis.fillpix(0xff0000); // vermelho
        Serial.println("ALERTA.\n");
        bright(100);  

      } else{
        M5.dis.fillpix(0x00F00F); // verde
        Serial.println("Ligado.\n"); 
        bright(100); 
      }      
    } else {
      Serial.println("Desligado.\n");
      M5.dis.clear();
    } 
  }
}

void alertaButao(void *pvParameters){
  info_button_t info_button, info_cod;    
  while (true) {
    if (M5.Btn.pressedFor(5000)) {
      info_button.cod == true;
      Serial.println("Acionado.\n");      
      xQueueSend(xQueueButton, &info_button, portMAX_DELAY);
      xQueueSend(xQueueButton, &info_cod, portMAX_DELAY);        
    }
        
    vTaskDelay(250/portTICK_RATE_MS);
  }
}