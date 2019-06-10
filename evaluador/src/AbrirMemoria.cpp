#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <unistd.h>
#include "Elementos.h"

using namespace std;

char *abrirMemoria(string Nombre)
{
    Nombre = "/" + Nombre;
    int fd = shm_open(Nombre.c_str(), O_RDWR, 0660);
    if (fd < 0)
    {
        cerr << "Error al abrir memoria compartida: 4 "
             << errno << strerror(errno) << endl;
        exit(1);
    }

    char *dir;

    if ((dir = (char *)(mmap(NULL, sizeof(struct Header), PROT_READ | PROT_WRITE, MAP_SHARED,
                             fd, 0))) == MAP_FAILED)
    {
        cerr << "Error intentando mapear la memoria compartida: 5"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    struct Header *PosHeader = (struct Header *)dir;

    int i = PosHeader->i;
    int ie = PosHeader->ie;
    int oe = PosHeader->oe;
    int q = PosHeader->q;

    munmap((void *)PosHeader, sizeof(struct Header));
    size_t memorysize =  sizeof(struct Header) + 
                        (sizeof(struct RegistroEntrada) * i * ie) + 
                        (sizeof(struct RegistroSalida) * oe); 

    if ((dir = (char *)(mmap(NULL, memorysize, PROT_READ | PROT_WRITE, MAP_SHARED,
                             fd, 0))) == MAP_FAILED)
    {
        cerr << "Error intentando mapear memoria compartida: 6"
             << errno << strerror(errno) << endl;
        exit(1);
    }
}