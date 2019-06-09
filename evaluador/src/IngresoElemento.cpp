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
// función que le entregan un registro a guardar en la memoria compartida de nombre

int ingresarRegistro(RegistroEntrada registro, string nombre)
{

  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor
  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string mutex = "Mut" + nombre + to_string(registro.bandeja);
  string vacio = "Vacio" + nombre + to_string(registro.bandeja);
  string lleno = "Lleno" + nombre + to_string(registro.bandeja);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 1);
  arrayLleno = sem_open(lleno.c_str(), 0);

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;

  Header *pHeader = (Header *)dir;

  int i  = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;

  // variable para recorrer la bandeja
  int recorrido = 0;
  //Semaforos
  int posSem = i;
  string s = to_string(posSem);

  // posición inicial de la bandeja i
  char *Pos = (registro.bandeja * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < ie)
  {
    //posición en la bandeja
    char *PosN = (Pos + (recorrido * sizeof(RegistroEntrada)));
    RegistroEntrada *pRegistro = (RegistroEntrada *)PosN;

    //si logra insertar se sale
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
    // sino sigue avanzando
    else
    {
      recorrido++;
    }
  }

  return 1;
}

// Método que imprime el contenido de las bandejas de entrada
int recorrer(string nombre)
{
  int Temp1 = 0;
  int Temp2 = 0;

  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;
  Header *pHeader = (Header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;

  while (Temp1 < i)
  {
    char *Pos = (Temp1 * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);
    while (Temp2 < ie)
    {
      char *PosN = (Pos + (Temp2 * sizeof(RegistroEntrada)));
      RegistroEntrada *pRegistro = (RegistroEntrada *)PosN;
      cout << pRegistro->id << pRegistro->tipo << pRegistro->cantidad << endl;
      Temp2++;
    }
    Temp1++;
    Temp2 = 0;
  }
  return 0;
}
