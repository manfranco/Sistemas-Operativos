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
#include "AbrirMemoria.cpp"
#include "elementos.h"

using namespace std;

int crearQ(string nombre)
{
    //accede a la memoria compartida
    // posición inicial
    char *dir = abrirMemoria(nombre);
    struct header *pHeader = (struct header *)dir;
    int q = pHeader->q;

    // Abrir espacio de memoria para usar, usando el nombre n
    nombre = nombre + "Q";

    int fd = shm_open(nombre.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);
    if (fd < 0)
    {
        cerr << "Error creando la memoria compartida: Q1"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    if (ftruncate(fd, sizeof(headerQ) != 0))
    {
        cerr << "Error creando la memoria compartida: Q2"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    char *dirQ;

    if ((dirQ = (char *)mmap(NULL, sizeof(headerQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q3"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    headerQ *pHeaderQ = (headerQ *)dirQ;
    pHeaderQ->q = q;

    close(fd);
    return EXIT_SUCCESS;
}

// Permite abrir el espacio de memoria compartida Q y devuelve la posición inicial
char *abrirQ(string nombre)
{
    nombre = "/" + nombre + "Q";
    int fd = shm_open(nombre.c_str(), O_RDWR, 0660);
    if (fd < 0)
    {
        cerr << "Error abriendo la memoria compartida: Q4"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    char *dir;

    if ((dir = (char *)(mmap(NULL, sizeof(headerQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q5"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    headerQ *pHeaderQ = (headerQ *)dir;
    int q = pHeaderQ->q;

    munmap((void *)pHeaderQ, sizeof(headerQ));
    size_t memorysize = sizeof(headerQ) + (sizeof(registrosalida) * q * 3);

    if ((dir = (char *)(mmap(NULL, memorysize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q6"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    return dir;
}

int recorrerQ(string nombre)
{

    int temp1 = 0;
    int temp2 = 0;
    int recorrido = 0;

    // posición inicial
    char *dire = abrirQ(nombre);
    headerQ *pHeaderQ = (headerQ *)dire;
    int q = pHeaderQ->q;

    while (recorrido < 3)
    {
        char *pos = dire + sizeof(struct headerQ) + (recorrido * sizeof(registrosalida) * q);

        while (temp2 < q)
        {
            char *posn = pos + (temp2 * sizeof(registrosalida));
            registrosalida *pRegistroSalida = (registrosalida *)posn;
            cout << pRegistroSalida->id << pRegistroSalida->tipo << pRegistroSalida->cantidad << endl;
            temp2++;
        }

        recorrido++;
        temp2 = 0;
    }
    return 0;
}

int ingresarBandejaQ(struct registrosalida registro, string nombre)
{
    //accede a la memoria compartida
    // posición inicial
    char *dir = abrirMemoria(nombre);
    struct header *pHeader = (struct header *)dir;
    int i = pHeader->i;

    //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor
    sem_t *arrayMut, *arrayVacio, *arrayLleno;
    int tipopipo;
    if (registro.tipo == 'B')
    {
        tipopipo = i;
    }
    if (registro.tipo == 'D')
    {
        tipopipo = i + 1;
    }
    if (registro.tipo == 'S')
    {
        tipopipo = i + 2;
    }
    string mutex = "Mut" + nombre + to_string(tipopipo);
    string vacio = "Vacio" + nombre + to_string(tipopipo);
    string lleno = "Lleno" + nombre + to_string(tipopipo);
    arrayMut = sem_open(mutex.c_str(), 0);
    arrayVacio = sem_open(vacio.c_str(), 1);
    arrayLleno = sem_open(lleno.c_str(), 0);

    //accede a la memoria compartida
    // posición inicial
    char *dire = abrirQ(nombre);

    headerQ *pHeaderQ = (struct headerQ *)dire;

    int q = pHeaderQ->q;

    // variable para recorrer la bandeja
    int recorrido = 0;
    //Semaforos
    int posSem = q;
    string s = to_string(posSem);

    // posición inicial de la bandeja según el tipo
    int posBandeja = tipopipo - i;
    char *pos = dire + sizeof(headerQ) + (posBandeja * sizeof(registrosalida) * q);

    //hasta que no logre insertar intentar
    // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
    sem_wait(arrayVacio);
    sem_wait(arrayMut);
    // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
    while (recorrido < q)
    {
        //posición en la bandeja
        char *posn = (pos + (recorrido * sizeof(registrosalida)));
        struct registrosalida *pRegistroSalida = (struct registrosalida *)posn;
        //si logra insertar se sale
        if (pRegistroSalida->cantidad <= 0)
        {
            pRegistroSalida->id = registro.id;
            pRegistroSalida->tipo = registro.tipo;
            pRegistroSalida->cantidad = registro.cantidad;
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
