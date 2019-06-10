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
#include "BandejasQ.cpp"


RegistroSalida retirarRegistroDeQ(char tipo, string Nombre)
{
    char *DirQ = abrirQ(Nombre);
    HeaderQ *PosHeaderQ = (HeaderQ *)DirQ;

    int q = PosHeaderQ->q;
    int i = PosHeaderQ->i;
    int b = PosHeaderQ->b;
    int d = PosHeaderQ->d;
    int s = PosHeaderQ->s;

    sem_t *arrayMut, *arrayVacio, *arrayLleno, *arrayReact;
    int PosTipo;
    int PosBandejaQ;
    if (tipo == 'B')
    {
        PosTipo = i;
        PosBandejaQ = 0;
    }
    if (tipo == 'D')
    {
        PosTipo = i + 1;
        PosBandejaQ = 1;
    }
    if (tipo == 'S')
    {
        PosTipo = i + 2;
        PosBandejaQ = 2;
    }

    string Mutex = "Mut" + Nombre + to_string(PosTipo);
    string Vacio = "Vacio" + Nombre + to_string(PosTipo);
    string Lleno = "Lleno" + Nombre + to_string(PosTipo);
    string Reactivo = "Reactivo" + Nombre + to_string(PosBandejaQ);

    arrayMut = sem_open(Mutex.c_str(), 0);
    arrayVacio = sem_open(Vacio.c_str(), 0);
    arrayLleno = sem_open(Lleno.c_str(), 0);
    arrayReact = sem_open(Reactivo.c_str(), 0);

    int Recorrido = 0;

    char *Pos = DirQ + sizeof(HeaderQ) + (PosBandejaQ * q * sizeof(RegistroSalida));

    RegistroSalida Registro;

    sem_wait(arrayLleno);
    sem_wait(arrayMut);
    while (Recorrido < q)
    {

        char *PosN = (Pos + (Recorrido * sizeof(RegistroSalida)));
        RegistroSalida *PosRegistro = (RegistroSalida *)PosN;

        if (PosRegistro->cantidad > 0)
        {

            int Costo = 0;
            char TipoDelRegistro = PosRegistro->tipo;
            for (int f = 0; f < PosRegistro->cantidad; ++f)
            {
                if (TipoDelRegistro == 'B')
                    Costo += rand() % ((7 + 1) - 1);
                if (TipoDelRegistro == 'D')
                    Costo += 5 + rand() % ((20 + 1) - 5);
                if (TipoDelRegistro == 'S')
                    Costo += 8 + rand() % ((25 + 1) - 8);
            }
            if (TipoDelRegistro == 'B')
            {
                while (PosHeaderQ->b < Costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                PosHeaderQ->b -= Costo;
            }
            if (TipoDelRegistro == 'D')
            {
                while (PosHeaderQ->d < Costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                PosHeaderQ->d -= Costo;
            }
            if (TipoDelRegistro == 'S')
            {
                while (PosHeaderQ->s < Costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                PosHeaderQ->s -= Costo;
            }

            Registro.cantidad = PosRegistro->cantidad;
            Registro.id = PosRegistro->id;
            Registro.tipo = PosRegistro->tipo;
            Registro.bandeja = PosRegistro->bandeja;

            PosRegistro->id = 0;
            PosRegistro->tipo = '0';
            PosRegistro->cantidad = 0;
            PosRegistro->bandeja = 0;
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