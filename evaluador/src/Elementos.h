#pragma once
#include <semaphore.h>

//Header ingresado por consola
struct Header{
  int i;
  int ie;
  int oe;
  int q;
  int b;
  int d;
  int s;
  char n[30];
};

//
struct HeaderQ{
  int q;
  int i;
  int b;
  int d;
  int s;
};

//Contenido de una posicion ie en la bandeja de entrada
struct RegistroEntrada {
  int bandeja;
  int id;
  char tipo;
  int cantidad;
};

// externo -> nombrado
// interno -> anonimo
//Contenido de una posicion en la bandeja de salida.
struct RegistroSalida{
  int id;
  int bandeja;
  char tipo;
  int cantidad;
};
