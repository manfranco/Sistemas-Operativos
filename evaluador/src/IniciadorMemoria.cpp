#pragma once
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "elementos.h"
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

  if (ftruncate(fd, sizeof(struct header) != 0))
  {
    cerr << "Error creando la memoria compartida: 2"
         << errno << strerror(errno) << endl;
    exit(1);
  }

  char *dir;

  if ((dir = (char *)mmap(NULL, sizeof(struct header), PROT_READ | PROT_WRITE, MAP_SHARED,
                          fd, 0)) == MAP_FAILED)
  {
    cerr << "Error mapeando la memoria compartida: 3"
         << errno << strerror(errno) << endl;
    exit(1);
  }
  struct header *pHeader = (struct header *)dir;

  pHeader->i = i;
  pHeader->ie = ie;
  pHeader->oe = oe;
  pHeader->q = q;
  pHeader->b = b;
  pHeader->d = d;
  pHeader->s = s;
  strcpy(pHeader->n, nombre.c_str());

  close(fd);
  
  return EXIT_SUCCESS;
}