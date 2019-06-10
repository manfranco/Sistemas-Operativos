#pragma once
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include "../include/Elementos.h"
#include "AbrirMemoria.cpp"

using namespace std;

int crearQ(string Nombre)
{
    char *dir = abrirMemoria(Nombre);
    Header *PosHeader = (Header *)dir;
    int q = PosHeader->q;
    int i = PosHeader->i;
    int b = PosHeader->b;
    int d = PosHeader->d;
    int s = PosHeader->s;

    Nombre = Nombre + "Q";

    int fd = shm_open(Nombre.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);
    if (fd < 0)
    {
        cerr << "Error creando la memoria compartida"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    if (ftruncate(fd, sizeof(HeaderQ) != 0))
    {
        cerr << "Error creando la memoria compartida"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    char *dirQ;

    if ((dirQ = (char *)mmap(NULL, sizeof(HeaderQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    HeaderQ *PosHeaderQ = (HeaderQ *)dirQ;
    PosHeaderQ->q = q;
    PosHeaderQ->i = i;
    PosHeaderQ->b = b;
    PosHeaderQ->d = d;
    PosHeaderQ->s = s;

    close(fd);
    return EXIT_SUCCESS;
}

char *abrirQ(string Nombre)
{
    Nombre = "/" + Nombre + "Q";
    int fd = shm_open(Nombre.c_str(), O_RDWR, 0660);
    if (fd < 0)
    {
        cerr << "Error abriendo la memoria compartida"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    char *dirQ;

    if ((dirQ = (char *)(mmap(NULL, sizeof(HeaderQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    HeaderQ *PosHeaderQ = (HeaderQ *)dirQ;
    int q = PosHeaderQ->q;
    int i = PosHeaderQ->i;
    int b = PosHeaderQ->b;
    int d = PosHeaderQ->d;
    int s = PosHeaderQ->s;

    munmap((void *)PosHeaderQ, sizeof(HeaderQ));
    size_t TamMem = sizeof(HeaderQ) + (sizeof(RegistroSalida) * q * 3);

    if ((dirQ = (char *)(mmap(NULL, TamMem, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    return dirQ;
}

int recorrerQ(string Nombre)
{

    int Temporal1 = 0;
    int Temporal2 = 0;
    int Recorrido = 0;

    char *Direccion = abrirQ(Nombre);
    HeaderQ *PosHeaderQ = (HeaderQ *)Direccion;
    int q = PosHeaderQ->q;
    int b = PosHeaderQ->b;
    int d = PosHeaderQ->d;
    int s = PosHeaderQ->s;

    while (Recorrido < 3)
    {
        char *Pos = Direccion + sizeof(HeaderQ) + (Recorrido * sizeof(RegistroSalida) * q);

        while (Temporal2 < q)
        {
            char *posn = Pos + (Temporal2 * sizeof(RegistroSalida));
            RegistroSalida *PosRegistroSalida = (RegistroSalida *)posn;
            cout << PosRegistroSalida->id << PosRegistroSalida->tipo << PosRegistroSalida->cantidad << endl;
            Temporal2++;
        }

        Recorrido++;
        Temporal2 = 0;
    }
    return 0;
}

int ingresarBandejaQ(RegistroSalida registro, string Nombre)
{

    char *Direccion = abrirQ(Nombre);
    HeaderQ *PosHeaderQ = (HeaderQ *)Direccion;

    int q = PosHeaderQ->q;
    int i = PosHeaderQ->i;
    int b = PosHeaderQ->b;
    int d = PosHeaderQ->d;
    int s = PosHeaderQ->s;

    sem_t *arrayMut, *arrayVacio, *arrayLleno;
    int Pipo;
    if (registro.tipo == 'B')
    {
        Pipo = i;
    }
    if (registro.tipo == 'D')
    {
        Pipo = i + 1;
    }
    if (registro.tipo == 'S')
    {
        Pipo = i + 2;
    }
    string Mutex = "Mut" + Nombre + to_string(Pipo);
    string Vacio = "Vacio" + Nombre + to_string(Pipo);
    string Lleno = "Lleno" + Nombre + to_string(Pipo);
    arrayMut = sem_open(Mutex.c_str(), 0);
    arrayVacio = sem_open(Vacio.c_str(), 0);
    arrayLleno = sem_open(Lleno.c_str(), 0);

    int Recorrido = 0;

    int posBandeja = Pipo - i;
    char *Pos = Direccion + sizeof(HeaderQ) + (posBandeja * sizeof(RegistroSalida) * q);

    sem_wait(arrayVacio);
    sem_wait(arrayMut);

    while (Recorrido < q)
    {
        
        char *posn = (Pos + (Recorrido * sizeof(RegistroSalida)));
        RegistroSalida *PosRegistroSalida = (RegistroSalida *)posn;

        if (PosRegistroSalida->cantidad <= 0)
        {
            PosRegistroSalida->id = registro.id;
            PosRegistroSalida->tipo = registro.tipo;
            PosRegistroSalida->cantidad = registro.cantidad;
            PosRegistroSalida->bandeja = registro.bandeja;

            sem_post(arrayMut);
            sem_post(arrayLleno);
            return EXIT_SUCCESS;
        }

        else
        {
            Recorrido++;
        }
    }

    return 1;
}

int IngresarReactivo(string Nombre, int cantidad, char tipo)
{

    char *dirQ = abrirQ(Nombre);
    HeaderQ *PosHeaderQ = (HeaderQ *)dirQ;

    int b = PosHeaderQ->b;
    int d = PosHeaderQ->d;
    int s = PosHeaderQ->s;
    int i = PosHeaderQ->i;

    sem_t *arrayMut;
    int PosTipo;
    int PosBandejaQ;
    if (tipo == 'B')
        PosTipo = i;
    if (tipo == 'D')
        PosTipo = i+1;
    if (tipo == 'S')
        PosTipo = i+2;
    string Mutex = "Mut" + Nombre + to_string(PosTipo);
    arrayMut = sem_open(Mutex.c_str(), 0);

    sem_wait(arrayMut);
    if (tipo == 'B')
    {
        PosHeaderQ->b += cantidad;
    }
    if (tipo == 'D')
    {
        PosHeaderQ->d += cantidad;
    }
    if (tipo == 'S')
    {
        PosHeaderQ->s += cantidad;
    }
    sem_post(arrayMut);

    return 0;
}

int ImprimirReactivo(string Nombre)
{
    char *dirQ = abrirQ(Nombre);
    HeaderQ *PosHeaderQ = (HeaderQ *)dirQ;

    cout << "Disponible " << PosHeaderQ->b << " de reactivo B" << endl;
    cout << "Disponible " << PosHeaderQ->d << " de reactivo D" << endl;
    cout << "Disponible" << PosHeaderQ->s << " de reactivo S" << endl;
    return 0;
}