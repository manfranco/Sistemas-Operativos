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

int ingresarRegistro(RegistroEntrada registro, string nombre)
{

  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string Mutex = "Mut" + nombre + to_string(registro.bandeja);
  string Vacio = "Vacio" + nombre + to_string(registro.bandeja);
  string Lleno = "Lleno" + nombre + to_string(registro.bandeja);
  arrayMut = sem_open(Mutex.c_str(), 0);
  arrayVacio = sem_open(Vacio.c_str(), 1);
  arrayLleno = sem_open(Lleno.c_str(), 0);

  char *dir = abrirMemoria(nombre);
  bool Insertado = false;

  Header *PosHeader = (Header *)dir;

  int i  = PosHeader->i;
  int ie = PosHeader->ie;
  int oe = PosHeader->oe;


  int Recorrido = 0;
  int PosSem = i;
  string s = to_string(PosSem);

  char *Pos = (registro.bandeja * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  while (Recorrido < ie)
  {
    char *PosN = (Pos + (Recorrido * sizeof(RegistroEntrada)));
    RegistroEntrada *PosRegistro = (RegistroEntrada *)PosN;

    if (PosRegistro->cantidad <= 0)
    {
      PosRegistro->bandeja = registro.bandeja;
      PosRegistro->id = registro.id;
      PosRegistro->tipo = registro.tipo;
      PosRegistro->cantidad = registro.cantidad;
      sem_post(arrayMut);
      sem_post(arrayLleno);
      return EXIT_SUCCESS;
      return 0;
    }
    else
    {
      Recorrido++;
    }
  }

  return 1;
}

int recorrer(string nombre)
{
  int Temp1 = 0;
  int Temp2 = 0;

  char *dir = abrirMemoria(nombre);
  bool Insertado = false;
  Header *PosHeader = (Header *)dir;

  int i = PosHeader->i;
  int ie = PosHeader->ie;
  int oe = PosHeader->oe;

  while (Temp1 < i)
  {
    char *Pos = (Temp1 * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);
    while (Temp2 < ie)
    {
      char *PosN = (Pos + (Temp2 * sizeof(RegistroEntrada)));
      RegistroEntrada *PosRegistro = (RegistroEntrada *)PosN;
      cout << PosRegistro->id << PosRegistro->tipo << PosRegistro->cantidad << endl;
      Temp2++;
    }
    Temp1++;
    Temp2 = 0;
  }
  return 0;
}
