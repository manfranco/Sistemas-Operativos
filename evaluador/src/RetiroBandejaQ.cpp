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
#include "Elementos.h"
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
    int Pos_Tipo;
    int Pos_BandejaQ;
    if (tipo == 'B')
    {
        Pos_Tipo = i;
        Pos_BandejaQ = 0;
    }
    if (tipo == 'D')
    {
        Pos_Tipo = i + 1;
        Pos_BandejaQ = 1;
    }
    if (tipo == 'S')
    {
        Pos_Tipo = i + 2;
        Pos_BandejaQ = 2;
    }

    string mutex = "Mut" + Nombre + to_string(Pos_Tipo);
    string vacio = "Vacio" + Nombre + to_string(Pos_Tipo);
    string lleno = "Lleno" + Nombre + to_string(Pos_Tipo);
    string reactivo = "Reactivo" + Nombre + to_string(Pos_BandejaQ);

    arrayMut = sem_open(mutex.c_str(), 0);
    arrayVacio = sem_open(vacio.c_str(), 0);
    arrayLleno = sem_open(lleno.c_str(), 0);
    arrayReact = sem_open(reactivo.c_str(), 0);

    int Recorrido = 0;

    char *Pos = DirQ + sizeof(HeaderQ) + (Pos_BandejaQ * q * sizeof(RegistroSalida));

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