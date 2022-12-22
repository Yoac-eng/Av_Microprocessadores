// TRABALHO AVALIATIVO - DISCIPLINA MICROCONTROLADORES E MICROPROCESSADORES
// UNIVERSIDADE ESTADUAL DO MARANHÃO - ENGENHARIA DA COMPUTAÇÃO
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
#include <controll_Button.ino>
#include <controll_Led.ino>


// Tarefas criadas
TaskHandle_t tarefa1;
TaskHandle_t tarefa2;
TaskHandle_t tarefa3;

// Criação fila para ação butão/led
QueueHandle_t xQueueButton;

typedef struct {
  bool status;
  bool cod;  
} info_button_t;

info_button_t info_button, info_cod;

uint8_t FSM = 0;

void setup() {
  
  // Inicializando o AtomLite
  M5.begin(true, false, true); 
  delay(50);

  // Inicializando o butão
  xQueueButton = xQueueCreate(5, sizeof(info_button));

  // Rodando Tarefas
  // xTaskCreatePinnedToCore(nome da funcao, "tarefa", tamanho da pilha, parametros, prioridade, alteração manual, nucleo);
  xTaskCreatePinnedToCore(ledTask, "tarefa1", 1000, NULL, 0, &tarefa1, 0);
  xTaskCreatePinnedToCore(buttonTask, "tarefa2", 1000, NULL, 2, &tarefa2, 1); //maior prioridade
  xTaskCreatePinnedToCore(alertaButao, "tarefa3", 1000, NULL, 4, &tarefa2, 0); 

}

void loop() {

}

void bright(int intensity) {
  M5.dis.setBrightness(intensity); 
}

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