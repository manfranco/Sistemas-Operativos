#include "IniciadorMemoria.cpp"
#include "ingresoelemento.cpp"
#include "AbrirSemaforos.cpp"
#include "retirarElemento.cpp"
#include "retiroBandejaQ.cpp"
#include "BandejasQ.cpp"
#include "abrirHilo.cpp"
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

    crearEspacio(nombre, i, ie, oe, q, b, d, s);
    crearSemaforo(nombre);
    crearQ(nombre);
    struct registroentrada test;
    test.bandeja = 0;
    test.cantidad = 3;
    test.id = 8;
    test.tipo = 'D';

    struct registroentrada test2;
    test2.bandeja = 2;
    test2.cantidad = 3;
    test2.id = 5;
    test2.tipo = 'B';
    ingresarRegistro(test, nombre);
    ingresarRegistro(test2, nombre);

    recorrer(nombre);
    crearHilo(nombre);
    cout << "" << endl;
    cout << "" << endl;
    sleep(3);
    recorrerQ(nombre);
    retirarRegistroDeQ('D',nombre);
    cout << "" << endl;
    cout << "" << endl;
    sleep(3);
    recorrerQ(nombre);

    return 0;
}