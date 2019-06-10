#pragma once
#include <semaphore.h>

struct Header{
  int i;
  int ie;
  int oe;
  int q;
  int b;
  int d;
  int s;
  char n[30];
  int contador;
};

struct HeaderQ{
  int q;
  int i;
  int b;
  int d;
  int s;
};

struct RegistroEntrada {
  int bandeja;
  int id;
  char tipo;
  int cantidad;
  int time;
};

struct RegistroSalida{
  int id;
  int bandeja;
  char tipo;
  int cantidad;
  int time;
  char resultado;
};
