#include "infix.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//  PILA (STACK)
void crearPila(pila *p) {
  p->top = NULL;
  p->len = 0;
}

void borrar_pila(pila *p) {
  while (p->top != NULL) {
    nodo *temp = p->top;
    p->top = p->top->sig;
    free(temp);
  }
  p->len = 0;
}

void push(pila *p, char dato) {
  nodo *nuevoNodo = (nodo *)malloc(sizeof(nodo));
  nuevoNodo->dato = dato;
  nuevoNodo->sig = p->top;
  p->top = nuevoNodo;
  p->len++;
}

char pop(pila *p) {
  if (p->top == NULL) {
    printf("Error: Pila vacía\n");
    return '\0';
  }

  char dato = p->top->dato;
  nodo *temp = p->top;
  p->top = p->top->sig;
  free(temp);
  p->len--;

  return dato;
}

bool es_vacia(pila *p) { return (p->top == NULL); }

//  COLA (QUEUE)
void *crear_cola() {
  cola *c = (cola *)malloc(sizeof(cola));
  c->head = NULL;
  c->tail = NULL;
  c->len = 0;
  // https://www.tutorialspoint.com/return-from-void-functions-in-cplusplus
  return (void *)c;
}

void encolar(cola *c, char dato) {
  nodo *nuevoNodo = (nodo *)malloc(sizeof(nodo));
  if (nuevoNodo == NULL) {
    printf("Error: No se pudo asignar memoria para el nuevo nodo\n");
    return;
  }

  nuevoNodo->dato = dato;
  nuevoNodo->sig = NULL;

  if (c->head == NULL) {
    c->head = nuevoNodo;
    c->tail = nuevoNodo;
  } else {
    if (c->tail == NULL) {
      printf("Cola inválida\n");
      free(nuevoNodo);
      return;
    }
    c->tail->sig = nuevoNodo;
    c->tail = nuevoNodo;
  }

  c->len++;
}

char desencolar(cola *c) {
  if (c->head == NULL) {
    printf("Error: Cola vacía\n");
    return '\0';
  }

  char dato = c->head->dato;
  nodo *temp = c->head;
  c->head = c->head->sig;
  free(temp);
  c->len--;

  if (c->head == NULL) {
    c->tail = NULL;
  }

  return dato;
}

bool es_vacia_cola(cola *c) { return (c->head == NULL); }

//------infijo a postfijo--------------------

int precedencia(char operador) {
  if (operador == '+' || operador == '-')
    return 1;
  else if (operador == '*' || operador == '/')
    return 2;
  else if (operador == '^')
    return 3;
  else
    return -1; // Operador inválido
}

void infix_to_postfix(char *infix, char *postfix) {
  pila p;
  crearPila(&p);

  int j = 0; // Índice para la cadena de caracteres postfix

  for (int i = 0; infix[i] != '\0'; i++) {
    char c = infix[i];

    if (isspace(c)) {
      continue;
    } else if (isalnum(c)) {
      while (isalnum(infix[i])) {
        postfix[j++] = infix[i++];
      }
      postfix[j++] = ' ';
      i--;
    } else if (c == '(') {
      push(&p, c);
    } else if (c == ')') {
      while (!es_vacia(&p) && p.top->dato != '(') {
        char operador = pop(&p);
        postfix[j++] = operador;
        postfix[j++] = ' ';
      }
      if (!es_vacia(&p) && p.top->dato == '(') {
        pop(&p);
      }
    } else {
      while (!es_vacia(&p) && precedencia(p.top->dato) >= precedencia(c)) {
        char operador = pop(&p);
        postfix[j++] = operador;
        postfix[j++] = ' ';
      }
      push(&p, c);
    }
  }

  while (!es_vacia(&p)) {
    char operador = pop(&p);
    postfix[j++] = operador;
    postfix[j++] = ' ';
  }

  postfix[j] = '\0'; // Agregar el carácter nulo al final de la cadena
}

float evaluar_expresion(const char *expresion) {
  int capacidad = strlen(expresion);
  float *pila = malloc(capacidad * sizeof(float));
  int tope = -1;

  for (int i = 0; expresion[i] != '\0'; i++) {
    char c = expresion[i];

    if (isspace(c)) {
      continue;
    } else if (isdigit(c)) {
      float num = 0.0;
      while (isdigit(expresion[i])) {
        num = num * 10 + (expresion[i] - '0');
        i++;
      }
      i--;
      tope++;
      pila[tope] = num;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
      if (tope < 1) {
        printf("Error: Expresión inválida\n");
        free(pila);
        return 0.0;
      }
      float num2 = pila[tope];
      tope--;
      float num1 = pila[tope];
      tope--;

      float resultado;
      switch (c) {
        case '+':
          resultado = num1 + num2;
          break;
        case '-':
          resultado = num1 - num2;
          break;
        case '*':
          resultado = num1 * num2;
          break;
        case '/':
          if (num2 != 0.0)
            resultado = num1 / num2;
          else {
            printf("Error: División entre cero\n");
            free(pila);
            return 0.0;
          }
          break;
      }

      tope++;
      pila[tope] = resultado;
    } else if (c == '^') {
      if (tope < 1) {
        printf("Error: Expresión inválida\n");
        free(pila);
        return 0.0;
      }
      float num2 = pila[tope];
      tope--;
      float num1 = pila[tope];
      tope--;

      float resultado = pow(num1, num2);
      tope++;
      pila[tope] = resultado;
    } else {
      printf("Error: Operador inválido\n");
      free(pila);
      return 0.0;
    }
  }

  if (tope != 0) {
    printf("Error: Expresión inválida\n");
    free(pila);
    return 0.0;
  }

  float resultado = pila[0];
  free(pila);
  return resultado;
}

void procesarArchivo(const char *nombreArchivo) {
  FILE *archivo;
  archivo = fopen(nombreArchivo, "r");
  if (archivo == NULL) {
    printf("No se pudo abrir el archivo.\n");
    return;
  }

  char expresion[100];
  char postfix[200]; // Cadena para almacenar la expresión postfija

  while (fgets(expresion, sizeof(expresion), archivo) != NULL) {
    expresion[strcspn(expresion, "\n")] = '\0';
    printf("Expresión original: %s\n", expresion);
    infix_to_postfix(expresion, postfix);
    printf("Expresión postfija: %s\n", postfix);
    float resultado = evaluar_expresion(postfix);
    printf("Resultado: %.2f\n\n", resultado);
  }

  fclose(archivo);
}
