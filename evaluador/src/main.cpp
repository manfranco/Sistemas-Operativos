#include "IniciadorMemoria.cpp"
#include "ManejoBandejaEntrada.cpp"
#include "AbrirSemaforos.cpp"
#include "BandejasQ.cpp"
#include "AbrirHilo.cpp"
#include "ManejoOE.cpp"
#include "Parser.cpp"
#include "DeleteEvaluador.cpp"
#include "RetirarElemento.cpp"
#include "RetiroBandejaQ.cpp"

using namespace std;

int main(int argc, char* argv[])
{
    string NombreMemoria;


    if(!strcmp(argv[1], "init"))
    {
        


        int i = 5;
        int ie = 6, q = 6;
        int oe = 10;
        int d = 100, b = 100, s = 100;
        bool Escuchando = true;
        string n = "evaluator";
        vector<string> Argumentos{"-i", "-ie", "-oe", "-n", "-d", "-b", "-s", "-q"};        

        for (int x = 2; x < argc; ++x){

            string t_argument(argv[x]);
            bool exists = find(begin(Argumentos), end(Argumentos), t_argument) != end(Argumentos);

            if (!exists){
                cout << "Error en argumento" << endl;
                cout << "El Error en argumento es: " << argv[x] << endl;
                return 1;
            }

           if(!strcmp(argv[x], "-i")) {
                i = stoi(argv[x + 1]);
                ++x;

                if(i == 0){
                    cout << "Valor inválido para i" << endl;
                    return 1;            
                }
            }

           if(!strcmp(argv[x], "-ie")) {
                ie = stoi(argv[x + 1]);
                ++x;

                if(ie == 0){
                    cout << "Valor inválido para ie" << endl;
                    return 1;            
                }
            }

            if(!strcmp(argv[x], "-oe")) {
                oe = stoi(argv[x + 1]);
                ++x;

                if(oe == 0){
                    cout << "Valor inválido para oe" << endl;
                    return 1;            
                }
            }

            if(!strcmp(argv[x], "-n")) {
                string Temp(argv[x + 1]);
                n = Temp;
                ++x;
            }

            if(!strcmp(argv[x], "-d")) {
                d = stoi(argv[x + 1]);
                ++x;

                if(d == 0 || d > 100  || 0 > d){
                    cout << "Valor inválido para d" << endl;
                    return 1;            
                }
            }      

            if(!strcmp(argv[x], "-b")) {
                b = stoi(argv[x + 1]);
                ++x;

                if(b == 0 || b > 100 || 0 > b){
                    cout << "Valor inválido para b" << endl;
                    return 1;            
                }
            }

            if(!strcmp(argv[x], "-s")) {
                s = stoi(argv[x + 1]);
                ++x;

                if(s == 0 || s > 100 || 0 > s){
                    cout << "Valor inválido para s" << endl;
                    return 1;            
                }
            }

            if(!strcmp(argv[x], "-q")) {
                q = stoi(argv[x + 1]);
                ++x;

                if(q == 0){
                    cout << "Valor inválido para q" << endl;
                    return 1;            
                }
            }                                                                
        }

        crearEspacio(n, i, ie, oe, q, b, d, s);
        crearSemaforo(n);
        crearQ(n);
        crearHilo(n);
        crearHiloProcesadores(n);

        while(true){}


    } else if(!strcmp(argv[1], "reg")) {

        RegistroEntrada Registro;       
        int id;
        bool Escuchando = true;
        bool isCiclo = true;
        bool Repetido;
        vector<int> IDExistentes;

        if(!strcmp(argv[2], "-n"))
        {
            string Temp(argv[3]);
            NombreMemoria = Temp;
        } else {
            cout << "Variable invalida" << endl;
            return 1;
        }

        if(!strcmp(argv[4], "-"))
        {

            cout << "bienvenido a la opcion interactiva" << endl;            
            string EntradaUsuario;

            while(Escuchando)
            {
                cout << ">";
                getline(cin, EntradaUsuario);
                cout << "La entrada fue: ";
                cout << EntradaUsuario << endl;

                if(EntradaUsuario == "exit"){
                    Escuchando = false;
                }

                string *EntradaAnalizada = parser(EntradaUsuario);

                Registro.bandeja = stoi(EntradaAnalizada[0]);
                Registro.tipo = EntradaAnalizada[1][0];
                Registro.cantidad = stoi(EntradaAnalizada[2]);


                while(isCiclo){        
                   id = rand() % ((10000 + 1) - 1);
                   Repetido = find(begin(IDExistentes), end(IDExistentes), id) != end(IDExistentes);

                   if (!Repetido){
                    cout << "ingreso unico" << endl;
                    IDExistentes.insert(begin(IDExistentes), id);
                    isCiclo = false;
                   }
                }

                Registro.id = id;
                ingresarRegistro(Registro, NombreMemoria);


            }

        } else {
            
            string NombreFichero(argv[4]);
            ifstream file(NombreFichero);
            if(file.fail())
            {
                cout << "Error en lectura de archivo" << endl;
            } else {
                cout << "lectura exitosa" << endl;
            }

            for(string line; getline(file, line); )
            {
                cout << line << endl;

                string *args = parser(line);

                Registro.bandeja = stoi(args[0]);
                Registro.bandeja = args[1][0];
                Registro.cantidad = stoi(args[2]);

                while(isCiclo){        
                   id = rand() % ((10000 + 1) - 1);
                   Repetido = find(begin(IDExistentes), end(IDExistentes), id) != end(IDExistentes);

                   if (!Repetido){
                    cout << "unico" << endl;
                    IDExistentes.insert(begin(IDExistentes), id);
                    isCiclo = false;
                   }
                }

                Registro.id = id;
                ingresarRegistro(Registro, NombreMemoria);            

            }
        }

    } else if(!strcmp(argv[1], "ctrl")) {

        string command;
        bool Escuchando = true;

        if(!strcmp(argv[2], "-n"))
        {
            string Temp(argv[3]);
            NombreMemoria = Temp;
        } else {
            cout << "Variable invalida" << endl;
            return 1;
        }


        while(Escuchando)
        {
            cout << ">";
            getline(cin, command);

            string *SegundaEntrada = parser(command);

            if(SegundaEntrada[0] == "list"){
                if(SegundaEntrada[1] == "processing"){

                    cout << "Processing:" << endl;
                    recorrerQ(NombreMemoria);

                } else if(SegundaEntrada[1] == "waiting"){

                    cout << "Waiting: " << endl;
                    recorrer(NombreMemoria);


                } else if(SegundaEntrada[1] == "reported"){

                    cout << "Reported: " << endl;
                    recorrerOE(NombreMemoria);

                } else if(SegundaEntrada[1] == "reactive"){

                    ImprimirReactivo(NombreMemoria);

                } else if(SegundaEntrada[1] == "all"){

                    cout << "Processing:" << endl;
                    recorrerQ(NombreMemoria);
                    cout << "Waiting: " << endl;
                    recorrer(NombreMemoria);
                    cout << "Reported: " << endl;
                    recorrerOE(NombreMemoria);
                    ImprimirReactivo(NombreMemoria);
                    retornarContador(NombreMemoria);
                }

            } else if(SegundaEntrada[0] == "update"){
                char Tipo = SegundaEntrada[1][0];
                int Valor = stoi(SegundaEntrada[2].c_str());

                IngresarReactivo(NombreMemoria, Valor, Tipo );
            } 
        }

    } else if(!strcmp(argv[1], "rep")){


        if(!strcmp(argv[2], "-n"))
        {
            string Temp(argv[3]);
            NombreMemoria = Temp;
        } else {
            cout << "Variable invalida" << endl;
            return 1;
        }

        if(!strcmp(argv[4], "-i")){
        
        } else if(!strcmp(argv[4], "-m")){
         
        }

    } else if(!strcmp(argv[1], "stop")){
        
        if(!strcmp(argv[2], "-n"))
        {
            string Temp(argv[3]);
            NombreMemoria = Temp;

        } else {
            cout << "Variable invalida" << endl;
            return 1;
        }

        stop(NombreMemoria);

    }

    return 0;
}