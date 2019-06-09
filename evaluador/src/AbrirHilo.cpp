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
#include "RetirarElemento.cpp"
#include "BandejasQ.cpp"
#include "RetiroBandejaQ.cpp"
#include "ManejoOE.cpp"

using namespace std;

// Estructura para  pasarle más de un argumento a un Hilo
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

    cout << "Sali?" << endl;

    pthread_exit(NULL);
}

void crearHilo(string n)
{
    //accede a la memoria compartida
    // posición inicial
    char *dir = abrirMemoria(n);

    struct Header *PosHeader = (struct Header *)dir;

    int i = PosHeader->i;

    // Instancia los elementos que van a hacer parte de los hilos
    // Instancia el arreglo de los hilos.
    pthread_t PosHilo[i];
    BanHilo Bande;
    Bande.Nombre = n;
    string NombreHilo = "Hilo" + n;

    // Crea los hilos y les asigna la funcion
    for (int m = 0; m < i; ++m)
    {
        Bande.Bandeja = m;
        ostringstream namellen;
        namellen << NombreHilo << m;
        string realNameLlen(namellen.str());
        pthread_create(&PosHilo[m], NULL, procesador, (void *)&Bande);
        sleep(0.1);
    }

    //ESTO SE BORRA, SE DEJA AHORA POR DEBUGGER

    // if (pthread_join(PosHilo[0], NULL))
    // {
    //     fprintf(stderr, "Error joining thread\n");
    //     return;
    // }

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

    cout << "Sali?" << endl;

    pthread_exit(NULL);
}

void crearHiloProcesadores(string n)
{
    //accede a la memoria compartida
    // posición inicial
    char *dirq = abrirQ(n);

    HeaderQ *PosHeaderQ = (HeaderQ *)dirq;

    int q = PosHeaderQ->q;

    // Instancia los elementos que van a hacer parte de los hilos
    // Instancia el arreglo de los hilos.
    pthread_t HiloQ[q];
    BandejaProcesos Bande;
    Bande.Nombre = n;
    char TipoProceso = 'B';
    string NombreHilo = "Hilo" + n;
    // Crea los hilos y les asigna la funcion
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


    //ESTO SE BORRA, SE DEJA AHORA POR DEBUGGER

    // if (pthread_join(hiloP[0], NULL))
    // {
    //     fprintf(stderr, "Error joining thread\n");
    //     return;
    // }

    return;
}