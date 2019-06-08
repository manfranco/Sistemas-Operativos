#pragma once
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <semaphore.h>
#include "elementos.h"
#include "AbrirSemaforos.cpp"
using namespace std;

int main(void)
{
  string nombre = "buffer";
  char *dir = abrirMemoria(nombre);
  struct header *pHeader = (struct header *)dir;

  int i = pHeader->i;

  int h = 0;
  int totalsems = 4 + i;
  string mut = "Mut" + nombre;
  string llen = "Lleno" + nombre;
  string vac = "Vacio" + nombre;
  string reactivo = "Reactivo" + nombre;
  string nombreq = nombre + "Q";
  sem_unlink("vacios");
  sem_unlink("llenos");
  sem_unlink("mutex");
  sem_unlink(((reactivo + "0")).c_str());
  sem_unlink(((reactivo + "1")).c_str());
  sem_unlink(((reactivo + "2")).c_str());
  shm_unlink("/buffer");
  shm_unlink(nombreq.c_str());

  while (h < totalsems)
  {

    ostringstream namemut;
    namemut << mut << h;
    string realNameMut(namemut.str());
    sem_unlink(realNameMut.c_str());

    ostringstream namellen;
    namellen << llen << h;
    string realNameLlen(namellen.str());
    sem_unlink(realNameLlen.c_str());

    ostringstream namevac;
    namevac << vac << h;
    string realNameVac(namevac.str());
    sem_unlink(realNameVac.c_str());
    h++;
  }
  return EXIT_SUCCESS;
}