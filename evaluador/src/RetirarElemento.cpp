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

  sem_t *arrayMut, *arrayVacio, *arrayLleno;
  string Mutex = "Mut" + nombre + to_string(bandeja);
  string Vacio = "Vacio" + nombre + to_string(bandeja);
  string Lleno = "Lleno" + nombre + to_string(bandeja);
  arrayMut = sem_open(Mutex.c_str(), 0);
  arrayVacio = sem_open(Vacio.c_str(), 1);
  arrayLleno = sem_open(Lleno.c_str(), 0);


  char *dir = abrirMemoria(nombre);
  Header *PosHeader = (Header *)dir;

  int i = PosHeader->i;
  int ie = PosHeader->ie;
  int oe = PosHeader->oe;

  int Recorrido = 0;

  char *Pos = (bandeja * ie * sizeof(RegistroEntrada)) + dir + sizeof(Header);
  RegistroSalida Registro;

  sem_wait(arrayLleno);
  sem_wait(arrayMut);

  while (Recorrido < ie)
  {
    char *PosN = (Pos + (Recorrido * sizeof(RegistroEntrada)));
    RegistroEntrada *PosRegistro = (RegistroEntrada *)PosN;

    if (PosRegistro->cantidad > 0)
    {
      
      Registro.cantidad = PosRegistro->cantidad;
      Registro.id = PosRegistro->id;
      Registro.tipo = PosRegistro->tipo;
      Registro.bandeja = PosRegistro->bandeja;
      
      PosRegistro->bandeja = bandeja;
      PosRegistro->id = 0;
      PosRegistro->tipo = '0';
      PosRegistro->cantidad = 0;
      sem_post(arrayMut);
      sem_post(arrayVacio);

      return Registro;
    }
    else
    {
      Recorrido++;
    }
  }

  return Registro;
}