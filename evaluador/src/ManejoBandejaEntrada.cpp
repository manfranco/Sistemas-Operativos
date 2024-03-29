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

int ingresarRegistro(RegistroEntrada registro, string nombre)
{

  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string mutex = "Mut" + nombre + to_string(registro.bandeja);
  string vacio = "Vacio" + nombre + to_string(registro.bandeja);
  string lleno = "Lleno" + nombre + to_string(registro.bandeja);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 1);
  arrayLleno = sem_open(lleno.c_str(), 0);

  char *dir = abrirMemoria(nombre);
  bool insertado = false;

  Header *pHeader = (Header *)dir;

  int i  = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;

  int recorrido = 0;
  int posSem = i;
  string s = to_string(posSem);

  char *pos = (registro.bandeja * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  while (recorrido < ie)
  {

    char *posn = (pos + (recorrido * sizeof(RegistroEntrada)));
    RegistroEntrada *pRegistro = (RegistroEntrada *)posn;


    if (pRegistro->cantidad <= 0)
    {
      pRegistro->bandeja = registro.bandeja;
      pRegistro->id = registro.id;
      pRegistro->tipo = registro.tipo;
      pRegistro->cantidad = registro.cantidad;

      sem_post(arrayMut);
      sem_post(arrayLleno);
      return EXIT_SUCCESS;
      return 0;
    }

    else
    {
      recorrido++;
    }
  }

  return 1;
}

int recorrer(string nombre)
{
  int temp1 = 0;
  int temp2 = 0;

  char *dir = abrirMemoria(nombre);
  bool insertado = false;
  Header *pHeader = (Header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;

  while (temp1 < i)
  {
    char *pos = (temp1 * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);
    while (temp2 < ie)
    {
      char *posn = (pos + (temp2 * sizeof(RegistroEntrada)));
      RegistroEntrada *pRegistro = (RegistroEntrada *)posn;
      cout << pRegistro->id << pRegistro->tipo << pRegistro->cantidad << endl;
      temp2++;
    }
    temp1++;
    temp2 = 0;
  }
  return 0;
}


int retornarContador(string nombre){

  char *dir = abrirMemoria(nombre);
  Header *pHeader = (Header *)dir;
  int contador = pHeader->contador;
  return 0;

}
