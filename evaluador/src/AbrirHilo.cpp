#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdio.h>
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "Elementos.h"
#include "AbrirMemoria.cpp"
#include "BandejasQ.cpp"
#include "ManejoOE.cpp"
#include "RetirarElemento.cpp"
#include "RetiroBandejaQ.cpp"

using namespace std;

struct BanHilo
{
    int Bandeja;
    string Nombre;
};

struct BandejaProcesos
{
    char tipo;
    string Nombre;
};
void *procesador(void *Bandeja_j)
{

    BanHilo *Producto = (BanHilo *)Bandeja_j;
    int NumBandeja = Producto->Bandeja;
    string NombreMemoria = Producto->Nombre;

    for (;;)
    {
        RegistroSalida Registro = retirarRegistro(NumBandeja, NombreMemoria);
        ingresarBandejaQ(Registro, NombreMemoria);

    }

    cout << "Salida" << endl;

    pthread_exit(NULL);
}

void crearHilo(string n)
{

    char *dir = abrirMemoria(n);

    struct Header *PosHeader = (struct Header *)dir;

    int i = PosHeader->i;


    pthread_t PosHilo[i];
    BanHilo Bande;
    Bande.Nombre = n;
    string NombreHilo = "Hilo" + n;

    for (int m = 0; m < i; ++m)
    {
        Bande.Bandeja = m;
        ostringstream namellen;
        namellen << NombreHilo << m;
        string realNameLlen(namellen.str());
        pthread_create(&PosHilo[m], NULL, procesador, (void *)&Bande);
        sleep(0.1);
    }


    return;
}

void *procesadorOE(void *bandej)
{

    BandejaProcesos *producto = (BandejaProcesos *)bandej;
    char tipo = producto->tipo;
    string NombreMemoria2 = producto->Nombre;

    for (;;)
    {   
        RegistroSalida Registro = retirarRegistroDeQ(tipo, NombreMemoria2);
        ingresarSalida(Registro, NombreMemoria2);
    }

    cout << "Salida" << endl;

    pthread_exit(NULL);
}

void crearHiloProcesadores(string n)
{

    char *dirq = abrirQ(n);

    HeaderQ *PosHeaderQ = (HeaderQ *)dirq;

    int q = PosHeaderQ->q;


    pthread_t HiloQ[q];
    BandejaProcesos Bande;
    Bande.Nombre = n;
    char TipoProceso = 'B';
    string NombreHilo = "Hilo" + n;
    for (int m = 0; m < 3; ++m)
    {
        if(m == 1) TipoProceso = 'D';
        if(m == 2) TipoProceso = 'S';
        Bande.tipo = TipoProceso;
        ostringstream namellen;
        namellen << NombreHilo << m;
        string realNameLlen(namellen.str());
        pthread_create(&HiloQ[m], NULL, procesadorOE, (void *)&Bande);
        sleep(1);
    }

    return;
}