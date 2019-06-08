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
#include "elementos.h"
#include "AbrirMemoria.cpp"

using namespace std;
// función que le entregan un registro a guardar en la memoria compartida de nombre

int ingresarRegistro(struct registroentrada registro, string nombre)
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

  struct header *pHeader = (struct header *)dir;

  int i  = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;

  // variable para recorrer la bandeja
  int recorrido = 0;
  //Semaforos
  int posSem = i;
  string s = to_string(posSem);

  // posición inicial de la bandeja i
  char *pos = (registro.bandeja * ie * sizeof(registroentrada)) + dir + sizeof(struct header);

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < ie)
  {
    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(registroentrada)));
    struct registroentrada *pRegistro = (struct registroentrada *)posn;

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
  int temp1 = 0;
  int temp2 = 0;

  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;
  struct header *pHeader = (struct header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;

  while (temp1 < i)
  {
    char *pos = (temp1 * ie * sizeof(registroentrada)) + dir + sizeof(struct header);
    while (temp2 < ie)
    {
      char *posn = (pos + (temp2 * sizeof(registroentrada)));
      struct registroentrada *pRegistro = (struct registroentrada *)posn;
      cout << pRegistro->id << pRegistro->tipo << pRegistro->cantidad << endl;
      temp2++;
    }
    temp1++;
    temp2 = 0;
  }
  return 0;
}
