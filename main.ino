#include "M5Atom.h"
#include <stdbool.h>
#include <controll_led.ino>
#include <controll_button.ino>


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



