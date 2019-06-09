#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <cstring>
#include <semaphore.h>
#include "Elementos.h"
#include "AbrirSemaforos.cpp"
using namespace std;

int main(void)
{
  string nombre = "evaluator";
  char *dir = abrirMemoria(nombre);
  struct Header *PosHeader = (struct Header *)dir;

  int i = PosHeader->i;

  int h = 0;
  string NameMemory = nombre;
  int TotalSemaforos = 4 + i;
  string Mutex = "Mut" + nombre;
  string Lleno = "Lleno" + nombre;
  string Vacio = "Vacio" + nombre;
  string Reactivo = "Reactivo" + nombre;
  string NombreQ = nombre + "Q";
  sem_unlink(((Reactivo + "0")).c_str());
  sem_unlink(((Reactivo + "1")).c_str());
  sem_unlink(((Reactivo + "2")).c_str());
  shm_unlink(NameMemory.c_str());
  shm_unlink(NombreQ.c_str());

  while (h < TotalSemaforos)
  {

    ostringstream namemut;
    namemut << Mutex << h;
    string realNameMut(namemut.str());
    sem_unlink(realNameMut.c_str());

    ostringstream namellen;
    namellen << Lleno << h;
    string realNameLlen(namellen.str());
    sem_unlink(realNameLlen.c_str());

    ostringstream namevac;
    namevac << Vacio << h;
    string realNameVac(namevac.str());
    sem_unlink(realNameVac.c_str());
    h++;
  }
  return EXIT_SUCCESS;
}