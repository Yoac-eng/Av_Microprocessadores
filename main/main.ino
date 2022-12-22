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