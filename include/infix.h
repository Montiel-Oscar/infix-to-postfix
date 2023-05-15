#ifndef __INFIX_H__
#define __INFIX_H__
#include <stdbool.h>
// NODO
typedef struct _nodo {
  char dato;
  struct _nodo *sig;
} nodo;
// COLA
struct _cola {
  nodo *head;
  nodo *tail;
  int len;
};
typedef struct _cola cola;
// PILA
struct _pila {
  nodo *top; // para no confundir con head
  int len;
};
typedef struct _pila pila;

// FUNCINOES PILA (STACK)
void crearPila(pila*p); 
void borrar_pila(pila *p);
void push(pila *p, char dato);
char pop(pila *p);
bool es_vacia(pila *p);
// FUNCIONES COLA (QUEUE)
void *crear_cola();
void encolar(cola *c, char dato);
char desencolar(cola *c);
bool es_vacia_cola(cola *p);
//INFIX TO POSTFIX
int precedencia(char operador);
void infix_to_postfix(char *infix, char *postfix);
float evaluar_expresion(const char *expresion);
void procesarArchivo(const char *nombreArchivo);
#endif
