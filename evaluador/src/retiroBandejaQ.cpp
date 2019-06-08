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
#include "BandejasQ.cpp"
//Aca estoy io
//Aca estamos los melos
using namespace std;
// función que le entregan un registro a guardar en la memoria compartida de nombre

registrosalida retirarRegistroDeQ(char tipo, string nombre)
{
  //accede a la memoria compartida
  // posición inicial
  char *dirQ = abrirQ(nombre);
  headerQ *pHeader = (headerQ *)dirQ;

  int q = pHeader->q;
  int i = pHeader->i;
  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor de las bandejas
  sem_t *arrayMut, *arrayVacio, *arrayLleno, *arrayReact;
  int pos_tipo;
  if (tipo == 'B')
  {
    pos_tipo = i;
  }
  if (tipo == 'D')
  {
    pos_tipo = i + 1;
  }
  if (tipo == 'S')
  {
    pos_tipo = i + 2;
  }

  string mutex = "Mut" + nombre + to_string(pos_tipo);
  string vacio = "Vacio" + nombre + to_string(pos_tipo);
  string lleno = "Lleno" + nombre + to_string(pos_tipo);
  string reactivo = "Reactivo" + nombre + to_string(pos_tipo - i);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 0);
  arrayLleno = sem_open(lleno.c_str(), 0);
  arrayReact = sem_open(reactivo.c_str(), 0);

  // variable para recorrer la bandeja
  int recorrido = 0;

  // posición inicial de la bandeja B|D|S
  char *pos = ((pos_tipo - i) * sizeof(registrosalida)) + dirQ + sizeof(headerQ);

  //Crear el registro de salida que d
  registrosalida registro;

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  sem_wait(arrayLleno);
  sem_wait(arrayMut);
  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < q)
  {

    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(registrosalida)));
    registrosalida *pRegistro = (registrosalida *)posn;
    

    //si encuentro elemento a retirar
    if (pRegistro->cantidad > 0)
    {

      //asigno los valores a devolver
      registro.cantidad = pRegistro->cantidad;
      registro.id = pRegistro->id;
      registro.tipo = pRegistro->tipo;

      //Pongo basura donde estaba
      //pRegistro->bandeja = bandeja;
      pRegistro->id = -1;
      pRegistro->tipo = 'a';
      pRegistro->cantidad = -1;
      sem_post(arrayMut);
      sem_post(arrayVacio);

      return registro;
    }
    else
    {
      recorrido++;
    }
    
  }
  return registro;
}