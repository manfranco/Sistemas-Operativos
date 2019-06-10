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
#include "Elementos.h"
#include "AbrirMemoria.cpp"

using namespace std;

int ingresarSalida(RegistroSalida registro, string nombre)
{

  char *dir = abrirMemoria(nombre);
  Header *pHeader = (Header *)dir;
  int i = pHeader->i;
  int oe = pHeader->oe;
  int ie = pHeader->ie;

  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  int pos_sem = i + 3;
  string mutex = "Mut" + nombre + to_string(pos_sem);
  string vacio = "Vacio" + nombre + to_string(pos_sem);
  string lleno = "Lleno" + nombre + to_string(pos_sem);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 0);
  arrayLleno = sem_open(lleno.c_str(), 0);
  int recorrido = 0;

  char *pos = (i * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  while (recorrido < oe)
  {
    char *posn = (pos + (recorrido * sizeof(RegistroSalida)));
    RegistroSalida *pRegistro = (RegistroSalida *)posn;

    if (pRegistro->cantidad <= 0)
    {
      pRegistro->id = registro.id;
      pRegistro->tipo = registro.tipo;
      pRegistro->cantidad = registro.cantidad;
      pRegistro->bandeja = registro.bandeja;
      sem_post(arrayMut);
      sem_post(arrayLleno);
      return EXIT_SUCCESS;
    }
    else
    {
      recorrido++;
    }
  }

  return 1;
}

int recorrerOE(string nombre)
{
  int temp1 = 0;

  char *dir = abrirMemoria(nombre);
  Header *pHeader = (Header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;
  char *pos = (i * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  while (temp1 < oe)
  {
    char *posn = (pos + (temp1 * sizeof(RegistroSalida)));
    RegistroSalida *pRegistro = (RegistroSalida *)posn;
    cout << pRegistro->id << pRegistro->tipo << pRegistro->cantidad << endl;
    temp1++;
  }

  return 0;
}