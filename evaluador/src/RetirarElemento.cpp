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



RegistroSalida retirarRegistro(int bandeja, string nombre)
{

  //Llama los 3 semaforo requeridos, Mutex, Vacio Lleno para el productor consumidor de las bandejas
  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string Mutex = "Mut" + nombre + to_string(bandeja);
  string Vacio = "Vacio" + nombre + to_string(bandeja);
  string Lleno = "Lleno" + nombre + to_string(bandeja);
  arrayMut = sem_open(Mutex.c_str(), 0);
  arrayVacio = sem_open(Vacio.c_str(), 1);
  arrayLleno = sem_open(Lleno.c_str(), 0);

  //accede a la memoria compartida
  // posición inicial
  char *dir = abrirMemoria(nombre);
  Header *PosHeader = (Header *)dir;

  int i = PosHeader->i;
  int ie = PosHeader->ie;
  int oe = PosHeader->oe;

  // variable para recorrer la bandeja
  int Recorrido = 0;

  // posición inicial de la bandeja i
  char *Pos = (bandeja * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);

  //Crear el Registro de salida que devolver
  RegistroSalida Registro;

  //hasta que no logre insertar intentar
  // Espera la semaforo para insertar, Vacio para saber si hay cupo y el Mutex
  //Soy Productor
  sem_wait(arrayLleno);
  sem_wait(arrayMut);

  // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
  while (Recorrido < ie)
  {
    //posición en la bandeja
    char *PosN = (Pos + (Recorrido * sizeof(RegistroEntrada)));
    RegistroEntrada *PosRegistro = (RegistroEntrada *)PosN;

    //si encuentro elemento a retirar
    if (PosRegistro->cantidad > 0)
    {
      
      //asigno los valores a devolver
      Registro.cantidad = PosRegistro->cantidad;
      Registro.id = PosRegistro->id;
      Registro.tipo = PosRegistro->tipo;
      Registro.bandeja = PosRegistro->bandeja;
      
      //Pongo basura donde estaba
      PosRegistro->bandeja = bandeja;
      PosRegistro->id = 0;
      PosRegistro->tipo = '0';
      PosRegistro->cantidad = 0;
      //soy consumidor
      sem_post(arrayMut);
      sem_post(arrayVacio);

      return Registro;
    }
    // sino sigue avanzando
    else
    {
      Recorrido++;
    }
  }

  return Registro;
}