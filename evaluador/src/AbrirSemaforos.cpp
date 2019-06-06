#pragma once
#include <semaphore.h>
#include <cerrno>
#include <cstring>
#include "AbrirMemoria.cpp"
#include <sstream>

int crearSemaforo(string nombre)
{
    char *dir = abrirMemoria(nombre);
    struct header *pHeader = (struct header *)dir;

    int i  = pHeader->i;
    int ie = pHeader->ie;
    int oe = pHeader->oe;
    int q  = pHeader->q;
    int b  = pHeader->b;
    int d  = pHeader->d;
    int s  = pHeader->s;

    int totalsems = 4 + i;
    sem_t **arraySemMut = new sem_t *[totalsems];
    sem_t **arraySemVacio = new sem_t *[totalsems];
    sem_t **arraySemLleno = new sem_t *[totalsems];
    sem_t **arraySemReactivo = new sem_t *[3];

    string mut = "Mut" + nombre;
    string llen = "Lleno" + nombre;
    string vac = "Vacio" + nombre;
    string reactivo = "Reactivo" + nombre;
    int cantidadReact = 0;
    int espacios = ie;

    for (int j = 0; j < totalsems; j++)
    {
        if (j == 0)
        {
            cantidadReact = b;
        }

        else if (j == 1)
        {
            cantidadReact = d;
        }

        else if (j == 2)
        {
            cantidadReact = s;
        }

        else if (j >= i)
        {
            espacios = oe;
        }

        ostringstream namemut;
        namemut << mut << j;
        string realNameMut(namemut.str());
        arraySemMut[j] = sem_open(realNameMut.c_str(), O_CREAT | O_EXCL, 0660, 1);

        ostringstream namellen;
        namellen << llen << j;
        string realNameLlen(namellen.str());
        arraySemVacio[j] = sem_open(realNameLlen.c_str(), O_CREAT | O_EXCL, 0660, 0);

        ostringstream namevac;
        namevac << vac << j;
        string realNameVac(namevac.str());
        arraySemLleno[j] = sem_open(realNameVac.c_str(), O_CREAT | O_EXCL, 0660, espacios);

        if (j < 3)
        {
            ostringstream namereact;
            namereact << reactivo << j;
            string realNameReact(namereact.str());
            arraySemReactivo[j] = sem_open(realNameReact.c_str(), O_CREAT | O_EXCL, 0660, cantidadReact);
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