#pragma once
#include <semaphore.h>


struct header{
  int i;
  int ie;
  int oe;
  int q;
  int b;
  int d;
  int s;
  char n[30];
};


struct registroentrada {
  int bandeja;
  int id;
  char tipo;
  int cantidad;
};

// externo -> nombrado
// interno -> anonimo
struct registrosalida{
  int id;
  char tipo;
  int cantidad;
};
