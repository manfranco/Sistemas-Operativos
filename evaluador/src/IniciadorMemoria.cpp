#pragma once
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Elementos.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include "AbrirSemaforos.cpp"

using namespace std;

int crearEspacio(string nombre, int i, int ie, int oe, int q, int b, int d, int s)
{

  // Abrir espacio de memoria para usar, usando el nombre n
  int fd = shm_open(nombre.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660); 

  if (fd < 0)
  {
    cerr << "Error creando la memoria compartida: 1"
         << errno << strerror(errno) << endl;
    exit(1);
  }
  //convierte el tamaño del espacio de memoria compartida en bytes
  if (ftruncate(fd, sizeof( Header) != 0)) 
  {
    cerr << "Error creando la memoria compartida: 2"
         << errno << strerror(errno) << endl;
    exit(1);
  }

  char *dir;

  if ((dir = (char *)mmap(NULL, sizeof(struct Header), PROT_READ | PROT_WRITE, MAP_SHARED,
                          fd, 0)) == MAP_FAILED)
  {
    cerr << "Error mapeando la memoria compartida: 3"
         << errno << strerror(errno) << endl;
    exit(1);
  }
  struct Header *PosHeader = (struct Header *)dir;

  PosHeader->i = i;
  PosHeader->ie = ie;
  PosHeader->oe = oe;
  PosHeader->q = q;
  PosHeader->b = b;
  PosHeader->d = d;
  PosHeader->s = s;
  strcpy(PosHeader->n, nombre.c_str());

  close(fd);
  
  return EXIT_SUCCESS;
}