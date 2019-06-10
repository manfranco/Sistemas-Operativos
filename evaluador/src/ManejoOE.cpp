#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include "../include/Elementos.h"
#include "AbrirMemoria.cpp"

using namespace std;

int ingresarSalida(RegistroSalida registro, string nombre)
{

  char *dir = abrirMemoria(nombre);
  Header *PosHeader = (Header *)dir;
  int i = PosHeader->i;
  int oe = PosHeader->oe;
  int ie = PosHeader->ie;

  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  int pos_sem = i + 3;
  string Mutex = "Mut" + nombre + to_string(pos_sem);
  string Vacio = "Vacio" + nombre + to_string(pos_sem);
  string Lleno = "Lleno" + nombre + to_string(pos_sem);
  arrayMut = sem_open(Mutex.c_str(), 0);
  arrayVacio = sem_open(Vacio.c_str(), 0);
  arrayLleno = sem_open(Lleno.c_str(), 0);
  int Recorrido = 0;

  char *pos = (i * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  while (Recorrido < oe)
  {
    char *PosN = (pos + (Recorrido * sizeof(RegistroSalida)));
    RegistroSalida *PosRegistro = (RegistroSalida *)PosN;

    if (PosRegistro->cantidad <= 0)
    {
      PosRegistro->id = registro.id;
      PosRegistro->tipo = registro.tipo;
      PosRegistro->cantidad = registro.cantidad;
      PosRegistro->bandeja = registro.bandeja;
      sem_post(arrayMut);
      sem_post(arrayLleno);
      return EXIT_SUCCESS;
    }
    else
    {
      Recorrido++;
    }
  }

  return 1;
}

int recorrerOE(string nombre)
{
  int Temporal1 = 0;

  char *dir = abrirMemoria(nombre);
  Header *PosHeader = (Header *)dir;

  int i = PosHeader->i;
  int ie = PosHeader->ie;
  int oe = PosHeader->oe;
  char *pos = (i * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  while (Temporal1 < oe)
  {
    char *PosN = (pos + (Temporal1 * sizeof(RegistroSalida)));
    RegistroSalida *PosRegistro = (RegistroSalida *)PosN;
    cout << PosRegistro->id << PosRegistro->tipo << PosRegistro->cantidad << endl;
    Temporal1++;
  }

  return 0;
}