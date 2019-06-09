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

int ingresarSalida(RegistroSalida registro, string nombre)
{

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  Header *pHeader = (Header *)dir;
  int i = pHeader->i;
  int oe = pHeader->oe;
  int ie = pHeader->ie;

  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor
  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  int pos_sem = i + 3;
  string mutex = "Mut" + nombre + to_string(pos_sem);
  string vacio = "Vacio" + nombre + to_string(pos_sem);
  string lleno = "Lleno" + nombre + to_string(pos_sem);
  arrayMut = sem_open(mutex.c_str(), 0);
  arrayVacio = sem_open(vacio.c_str(), 0);
  arrayLleno = sem_open(lleno.c_str(), 0);
  int recorrido = 0;

  // posición inicial de la bandeja i
  char *pos = (i * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  sem_wait(arrayVacio);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < oe)
  {
    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(RegistroSalida)));
    RegistroSalida *pRegistro = (RegistroSalida *)posn;

    //si logra insertar se sale
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
    // sino sigue avanzando
    else
    {
      recorrido++;
    }
  }

  return 1;
}

/*RegistroSalida retirarSalida(int bandeja, string nombre)
{

  //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor de las bandejas
  int recorrido = 0;
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

  // posición inicial de la bandeja i
  char *pos = (bandeja * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  //Crear el registro de salida que devolver
  RegistroSalida registro;

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
  sem_wait(arrayLleno);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (recorrido < oe)
  {
    //posición en la bandeja
    char *posn = (pos + (recorrido * sizeof(RegistroSalida)));
    RegistroSalida *pRegistro = (RegistroSalida *)posn;

    //si encuentro elemento a retirar
    if (pRegistro->cantidad > 0)
    {
      //asigno los valores a devolver
      registro.cantidad = pRegistro->cantidad;
      registro.id = pRegistro->id;
      registro.tipo = pRegistro->tipo;

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
*/

int recorrerOE(string nombre)
{
  int temp1 = 0;

  // posición inicial
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