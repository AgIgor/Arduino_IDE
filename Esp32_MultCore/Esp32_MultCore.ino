#include <Arduino.h>

TaskHandle_t Task1;
TaskHandle_t Task2;

// Função para o primeiro núcleo
void task1Function(void *pvParameters) {
  while(1) {
    Serial.println("Task 1 running on core " + String(xPortGetCoreID()));
    delay(1000);
  }
}

// Função para o segundo núcleo
void task2Function(void *pvParameters) {
  while(1) {
    Serial.println("Task 2 running on core " + String(xPortGetCoreID()));
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);

  // Criação das tarefas para cada núcleo
  xTaskCreatePinnedToCore(
    task1Function,    // Função da tarefa 1
    "Task1",          // Nome da tarefa 1
    10000,            // Tamanho da pilha da tarefa 1
    NULL,             // Parâmetros da tarefa 1
    1,                // Prioridade da tarefa 1
    &Task1,           // Handle da tarefa 1
    0                 // Núcleo para executar a tarefa 1 (0 para o núcleo 1)
  );

  xTaskCreatePinnedToCore(
    task2Function,    // Função da tarefa 2
    "Task2",          // Nome da tarefa 2
    10000,            // Tamanho da pilha da tarefa 2
    NULL,             // Parâmetros da tarefa 2
    1,                // Prioridade da tarefa 2
    &Task2,           // Handle da tarefa 2
    1                 // Núcleo para executar a tarefa 2 (1 para o núcleo 2)
  );
}

void loop() {
  // Não é necessário fazer nada no loop principal
}
