// TRABALHO AVALIATIVO - DISCIPLINA MICROCONTROLADORES E MICROPROCESSADORES
// OBJETIVOS: UTILIZAR FreeRTOS P/ GERENCIAMENTO DE TASK
//            UMA TASK GERENCIA A OUTRA
//
//COMPONENTES: MARCOS COSTA OLIVEIRA;
//             THIAGO GONZÁLEZ;
//             CLISTENYS EDUARDO;
//             CAYO RICARDO;
//             YAGO GOLTARA;

#include "M5Atom.h"
#include <stdbool.h>

// Tarefas criadas
TaskHandle_t tarefa1;
TaskHandle_t tarefa2;

// Criação fila para ação butão/led
QueueHandle_t xQueueButton;

typedef struct {
  bool status;
  int cod;  
} info_button_t;

info_button_t info_button;

void setup() {
  
  // Inicializando o AtomLite
  M5.begin(true, false, true); 
  delay(50);

  // Inicializando o butão
  xQueueButton = xQueueCreate(8, sizeof(info_button));

  // Rodando Tarefas
  // xTaskCreatePinnedToCore(nome da funcao, "tarefa", tamanho da pilha, parametros, prioridade, alteração manual, nucleo);
  xTaskCreatePinnedToCore(ledTask, "tarefa1", 1000, NULL, 0, &tarefa1, 0);
  xTaskCreatePinnedToCore(buttonTask, "tarefa2", 1000, NULL, 1, &tarefa2, 1);

}

void loop() {

}

void bright(int intensity) {
  M5.dis.setBrightness(intensity); 
}

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

// @author: Marcos Costa - 22/12/2022