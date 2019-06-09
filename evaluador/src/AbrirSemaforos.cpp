#pragma once
#include <semaphore.h>
#include <cerrno>
#include <cstring>
#include "AbrirMemoria.cpp"
#include <sstream>

int crearSemaforo(string nombre)
{
    char *dir = abrirMemoria(nombre);
    struct Header *PosHeader = (struct Header *)dir;

    int i  = PosHeader->i;
    int ie = PosHeader->ie;
    int oe = PosHeader->oe;
    int q  = PosHeader->q;
    int b  = PosHeader->b;
    int d  = PosHeader->d;
    int s  = PosHeader->s;

    int totalsems = 4 + i;
    sem_t **arraySemMut = new sem_t *[totalsems];
    sem_t **arraySemVacio = new sem_t *[totalsems];
    sem_t **arraySemLleno = new sem_t *[totalsems];
    sem_t **arraySemReactivo = new sem_t *[3];

    string Mutix = "Mut" + nombre;
    string Lleno = "Lleno" + nombre;
    string Vacio = "Vacio" + nombre;
    string Reactivo = "Reactivo" + nombre;
    int CantidadReactivos = 0;
    int Espacios = ie;

    for (int j = 0; j < totalsems; j++)
    {
        if (j == 0)
        {
            CantidadReactivos = b;
        }

        else if (j == 1)
        {
            CantidadReactivos = d;
        }

        else if (j == 2)
        {
            CantidadReactivos = s;
        }

        else if (j >= i)
        {
            Espacios = oe;
        }

        ostringstream namemut;
        namemut << Mutix << j;
        string realNameMut(namemut.str());
        arraySemMut[j] = sem_open(realNameMut.c_str(), O_CREAT | O_EXCL, 0660, 1);

        ostringstream namellen;
        namellen << Lleno << j;
        string realNameLlen(namellen.str());
        arraySemVacio[j] = sem_open(realNameLlen.c_str(), O_CREAT | O_EXCL, 0660, 0);

        ostringstream namevac;
        namevac << Vacio << j;
        string realNameVac(namevac.str());
        arraySemLleno[j] = sem_open(realNameVac.c_str(), O_CREAT | O_EXCL, 0660, Espacios);

        if (j < 3)
        {
            ostringstream namereact;
            namereact << Reactivo << j;
            string realNameReact(namereact.str());
            arraySemReactivo[j] = sem_open(realNameReact.c_str(), O_CREAT | O_EXCL, 0660, CantidadReactivos);
        }

        if (arraySemMut[j] == SEM_FAILED)
        {
            cerr << "Error creando los semaforos 'Mut'" << endl;
        }
        if (arraySemVacio[j] == SEM_FAILED)
        {
            cerr << "Error creando los semaforos 'Vacio'" << endl;
        }
        if (arraySemLleno[j] == SEM_FAILED)
        {
            cerr << "Error creando los semaforos 'Lleno'" << endl;
        }

        if (j < 3)
        {
            if (arraySemReactivo[j] == SEM_FAILED)
            {
                cerr << "Error creando los semaforos 'Reactivo'" << endl;
            }
        }
    }

    return 0;
}