#include "IniciadorMemoria.cpp"
#include "IngresoElemento.cpp"
#include "AbrirSemaforos.cpp"
#include "RetirarElemento.cpp"
#include "RetiroBandejaQ.cpp"
#include "BandejasQ.cpp"
#include "AbrirHilo.cpp"
#include "ManejoOE.cpp"
using namespace std;

int main()
{
    string nombre = "evaluator";
    int i = 3;
    int ie = 5;
    int oe = 10;
    int q = 5;
    int b = 5;
    int d = 5;
    int s = 5;

    cout << "Inicio" << endl;
    crearEspacio(nombre, i, ie, oe, q, b, d, s);
    crearSemaforo(nombre);
    crearQ(nombre);
    crearHilo(nombre);
    sleep(1);
    crearHiloProcesadores(nombre);


    RegistroEntrada testD;
    testD.bandeja = 0;
    testD.cantidad = 5;
    testD.id = 1;
    testD.tipo = 'D';

    RegistroEntrada testB;
    testB.bandeja = 1;
    testB.cantidad = 4;
    testB.id = 2;
    testB.tipo = 'B';

    RegistroEntrada testS;
    testS.bandeja = 2;
    testS.cantidad = 8;
    testS.id = 3;
    testS.tipo = 'S';

    ingresarRegistro(testD, nombre);
    ingresarRegistro(testB, nombre);
    ingresarRegistro(testS, nombre);
 
    sleep(5);
    cout << "" << endl;
    cout << " Bandejas OE Resultado" << endl;
    cout << "" << endl;
    recorrerOE(nombre);

    return 0;
}