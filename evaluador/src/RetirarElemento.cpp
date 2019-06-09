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
//Aca estamos los melos
using namespace std;
// función que le entregan un registro a guardar en la memoria compartida de nombre

RegistroSalida retirarRegistro(int bandeja, string nombre)
{

  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor de las bandejas
  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string mutex = "Mut" + nombre + to_string(bandeja);
  string vacio = "Vacio" + nombre + to_string(bandeja);
  string lleno = "Lleno" + nombre + to_string(bandeja);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 1);
  arrayLleno = sem_open(lleno.c_str(), 0);

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  bool insertado = false;

  Header *pHeader = (Header *)dir;

  int i = pHeader->i;
  int ie = pHeader->ie;
  int oe = pHeader->oe;

  // variable para recorrer la bandeja
  int recorrido = 0;
  //Semaforos
  int posSem = i;
  string s = to_string(posSem);

  // posición inicial de la bandeja i
  char *pos = (bandeja * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  //Crear el registro de salida que devolver
  RegistroSalida registro;

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  sem_wait(arrayLleno);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < ie)
  {
    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(RegistroEntrada)));
    RegistroEntrada *pRegistro = (RegistroEntrada *)posn;

    //si encuentro elemento a retirar
    if (pRegistro->cantidad > 0)
    {
      //asigno los valores a devolver
      registro.cantidad = pRegistro->cantidad;
      registro.id = pRegistro->id;
      registro.tipo = pRegistro->tipo;
      registro.bandeja = pRegistro->bandeja;
      
      //Pongo basura donde estaba
      pRegistro->bandeja = bandeja;
      pRegistro->id = 0;
      pRegistro->tipo = '0';
      pRegistro->cantidad = 0;
      //soy consumidor
      sem_post(arrayMut);
      sem_post(arrayVacio);

      return registro;
    }
    // sino sigue avanzando
    else
    {
      recorrido++;
    }
  }

  return registro;
}
