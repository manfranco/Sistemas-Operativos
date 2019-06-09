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
#include "Elementos.h"

using namespace std;

int crearQ(string Nombre)
{
    //accede a la memoria compartida
    // posición inicial
    char *dir = abrirMemoria(Nombre);
     Header *PosHeader = ( Header *)dir;
    int q = PosHeader->q;
    int i = PosHeader->i;

    // Abrir espacio de memoria para usar, usando el Nombre n
    Nombre = Nombre + "Q";

    int fd = shm_open(Nombre.c_str(), O_RDWR | O_CREAT | O_EXCL, 0660);
    if (fd < 0)
    {
        cerr << "Error creando la memoria compartida: Q1"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    //Acorta la region de memoria, de acuerdo al tamaño requerido
    if (ftruncate(fd, sizeof(HeaderQ) != 0)) 
    {
        cerr << "Error creando la memoria compartida: Q2"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    char *dirQ;

    if ((dirQ = (char *)mmap(NULL, sizeof(HeaderQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q3"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    HeaderQ *PosHeaderQ = (HeaderQ *)dirQ;
    PosHeaderQ->q = q;
    PosHeaderQ->i = i;

    close(fd);
    return EXIT_SUCCESS;
}

// Permite abrir el espacio de memoria compartida Q y devuelve la posición inicial
char *abrirQ(string Nombre)
{
    Nombre = "/" + Nombre + "Q";
    int fd = shm_open(Nombre.c_str(), O_RDWR, 0660);
    if (fd < 0)
    {
        cerr << "Error abriendo la memoria compartida: Q4"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    char *dir;

    if ((dir = (char *)(mmap(NULL, sizeof(HeaderQ), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q5"
             << errno << strerror(errno) << endl;
        exit(1);
    }

    HeaderQ *PosHeaderQ = (HeaderQ *)dir;
    int q = PosHeaderQ->q;
    int i = PosHeaderQ->i;

    munmap((void *)PosHeaderQ, sizeof(HeaderQ));
    size_t memorysize = sizeof(HeaderQ) + (sizeof(RegistroSalida) * q * 3);

    if ((dir = (char *)(mmap(NULL, memorysize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0))) == MAP_FAILED)
    {
        cerr << "Error mapeando la memoria compartida: Q6"
             << errno << strerror(errno) << endl;
        exit(1);
    }
    return dir;
}

int recorrerQ(string Nombre)
{

    int Temp1 = 0;
    int Temp2 = 0;
    int Recorrido = 0;

    // posición inicial
    char *dire = abrirQ(Nombre);
    HeaderQ *PosHeaderQ = (HeaderQ *)dire;
    int q = PosHeaderQ->q;

    while (Recorrido < 3)
    {
        char *Pos = dire + sizeof( HeaderQ) + (Recorrido * sizeof(RegistroSalida) * q);

        while (Temp2 < q)
        {
            char *PosN = Pos + (Temp2 * sizeof(RegistroSalida));
            RegistroSalida *PRegistroSalida = (RegistroSalida *)PosN;
            cout << PRegistroSalida->id << PRegistroSalida->tipo << PRegistroSalida->cantidad << endl;
            Temp2++;
        }

        Recorrido++;
        Temp2 = 0;
    }
    return 0;
}

int ingresarBandejaQ( RegistroSalida Registro, string Nombre)
{
    //accede a la memoria compartida
    // posición inicial
    char *dire = abrirQ(Nombre);

    HeaderQ *PosHeaderQ = ( HeaderQ *)dire;

    int q = PosHeaderQ->q;
    int i = PosHeaderQ->i;

    //Llama los 3 semaforo requeridos, Mutex, Vacio Lleno para el productor consumidor
    sem_t *arrayMut, *arrayVacio, *arrayLleno;
    int TipoPipo;
    if (Registro.tipo == 'B')
    {
        TipoPipo = i;
    }
    if (Registro.tipo == 'D')
    {
        TipoPipo = i + 1;
    }
    if (Registro.tipo == 'S')
    {
        TipoPipo = i + 2;
    }
    string Mutex = "Mut" + Nombre + to_string(TipoPipo);
    string Vacio = "Vacio" + Nombre + to_string(TipoPipo);
    string Lleno = "Lleno" + Nombre + to_string(TipoPipo);
    arrayMut = sem_open(Mutex.c_str(), 0);
    arrayVacio = sem_open(Vacio.c_str(), 0);
    arrayLleno = sem_open(Lleno.c_str(), 0);



    // variable para recorrer la bandeja
    int Recorrido = 0;
    //Semaforos
    int PosSem = q;
    string s = to_string(PosSem);

    // posición inicial de la bandeja según el tipo
    int PosBandeja = TipoPipo - i;
    char *Pos = dire + sizeof(HeaderQ) + (PosBandeja * sizeof(RegistroSalida) * q);

    //hasta que no logre insertar intentar
    // Espera la semaforo para insertar, Vacio para saber si hay cupo y el Mutex
    sem_wait(arrayVacio);
    sem_wait(arrayMut);
    // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
    while (Recorrido < q)
    {
        //posición en la bandeja
        char *PosN = (Pos + (Recorrido * sizeof(RegistroSalida)));
        RegistroSalida *PRegistroSalida = (RegistroSalida *)PosN;
        //si logra insertar se sale
        if (PRegistroSalida->cantidad <= 0)
        {
            PRegistroSalida->id = Registro.id;
            PRegistroSalida->tipo = Registro.tipo;
            PRegistroSalida->cantidad = Registro.cantidad;
            PRegistroSalida->bandeja = Registro.bandeja;
            sem_post(arrayMut);
            sem_post(arrayLleno);
            return EXIT_SUCCESS;
        }
        // sino sigue avanzando
        else
        {
            Recorrido++;
        }
    }

    return 1;
}
