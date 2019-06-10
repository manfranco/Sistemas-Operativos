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
    //accede a la memoria compartida
    // posición inicial
    char *DirQ = abrirQ(Nombre);
    HeaderQ *PosHeaderQ = (HeaderQ *)DirQ;

    int q = PosHeaderQ->q;
    int i = PosHeaderQ->i;
    int b = PosHeaderQ->b;
    int d = PosHeaderQ->d;
    int s = PosHeaderQ->s;

    //Llama los 3 semaforo requeridos, mutex, vacio lleno para el productor consumidor de las bandejas
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

    // variable para recorrer la bandeja
    int Recorrido = 0;

    // posición inicial de la bandeja B|D|S
    char *Pos = DirQ + sizeof(HeaderQ) + (Pos_BandejaQ * q * sizeof(RegistroSalida));

    //Crear el Registro de salida que d
    RegistroSalida Registro;

    //hasta que no logre insertar intentar
    // Espera la semaforo para insertar, vacio para saber si hay cupo y el mutex
    //Soy productor

    sem_wait(arrayLleno);
    sem_wait(arrayMut);
    // ciclo que avanza dentro de una bandeja usando n, recorre bandeja
    while (Recorrido < q)
    {

        //posición en la bandeja
        char *PosN = (Pos + (Recorrido * sizeof(RegistroSalida)));
        RegistroSalida *PosRegistro = (RegistroSalida *)PosN;

        //si encuentro elemento a retirar
        if (PosRegistro->cantidad > 0)
        {

            //GENERACION del Costo en reativo según tipo
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
            // Si es tipo B, Si no tengo suficiente reactivo libero el mutex y lo espero de nuevo
            if (TipoDelRegistro == 'B')
            {
                while (PosHeaderQ->b < Costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                PosHeaderQ->b -= Costo;
            }
            // Si es tipo D, Si no tengo suficiente reactivo libero el mutex y lo espero de nuevo
            if (TipoDelRegistro == 'D')
            {
                while (PosHeaderQ->d < Costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                PosHeaderQ->d -= Costo;
            }
            // Si es tipo S, Si no tengo suficiente reactivo libero el mutex y lo espero de nuevo
            if (TipoDelRegistro == 'S')
            {
                while (PosHeaderQ->s < Costo)
                {
                    sem_post(arrayMut);
                    sem_wait(arrayMut);
                }
                PosHeaderQ->s -= Costo;
            }

            //asigno los valores a devolver
            Registro.cantidad = PosRegistro->cantidad;
            Registro.id = PosRegistro->id;
            Registro.tipo = PosRegistro->tipo;
            Registro.bandeja = PosRegistro->bandeja;

            //Pongo basura donde estaba
            PosRegistro->id = 0;
            PosRegistro->tipo = '0';
            PosRegistro->cantidad = 0;
            PosRegistro->bandeja = 0;
            //Soy consumidor
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