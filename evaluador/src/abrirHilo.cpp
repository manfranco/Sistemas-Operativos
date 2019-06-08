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
#include "elementos.h"
#include "AbrirMemoria.cpp"
#include "retirarElemento.cpp"
#include "BandejasQ.cpp"
#include "retiroBandejaQ.cpp"

using namespace std;
struct banNam
{
    int bandeja;
    string nombre;
};
void *procesador(void *bandej)
{

    banNam *producto = (banNam *)bandej;
    int num_bandeja = producto->bandeja;
    string nom_memoria = producto->nombre;

    for (;;)
    {
        registrosalida registro = retirarRegistro(num_bandeja, nom_memoria);
        ingresarBandejaQ(registro, nom_memoria);

    }

    cout << "Sali?" << endl;

    pthread_exit(NULL);
}

void crearHilo(string n)
{
    //accede a la memoria compartida
    // posición inicial
    char *dir = abrirMemoria(n);

    struct header *pHeader = (struct header *)dir;

    int i = pHeader->i;

    // Instancia los elementos que van a hacer parte de los hilos
    // Instancia el arreglo de los hilos.
    pthread_t hiloP[i];
    banNam bande;
    bande.nombre = n;
    string n_Hilo = "Hilo" + n;

    // Crea los hilos y les asigna la funcion
    for (int m = 0; m < i; ++m)
    {
        bande.bandeja = m;
        ostringstream namellen;
        namellen << n_Hilo << m;
        string realNameLlen(namellen.str());
        pthread_create(&hiloP[m], NULL, procesador, (void *)&bande);
        sleep(0.1);
    }

    //ESTO SE BORRA, SE DEJA AHORA POR DEBUGGER

    // if (pthread_join(hiloP[0], NULL))
    // {
    //     fprintf(stderr, "Error joining thread\n");
    //     return;
    // }

    return;
}