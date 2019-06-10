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
                cout << "Argumento invalido" << endl;
                cout << "El argumento invalido es: " << argv[x] << endl;
                return 1;
            }

           if(!strcmp(argv[x], "-i")) {
                i = stoi(argv[x + 1]);
                ++x;

                if(i == 0){
                    cout << "Valor no autorizado para i" << endl;
                    return 1;            
                }
            }

           if(!strcmp(argv[x], "-ie")) {
                ie = stoi(argv[x + 1]);
                ++x;

                if(ie == 0){
                    cout << "Valor no autorizado para ie" << endl;
                    return 1;            
                }
            }

            if(!strcmp(argv[x], "-oe")) {
                oe = stoi(argv[x + 1]);
                ++x;

                if(oe == 0){
                    cout << "Valor no autorizado para oe" << endl;
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
                    cout << "Valor no autorizado para d" << endl;
                    return 1;            
                }
            }      

            if(!strcmp(argv[x], "-b")) {
                b = stoi(argv[x + 1]);
                ++x;

                if(b == 0 || b > 100 || 0 > b){
                    cout << "Valor no autorizado para b" << endl;
                    return 1;            
                }
            }

            if(!strcmp(argv[x], "-s")) {
                s = stoi(argv[x + 1]);
                ++x;

                if(s == 0 || s > 100 || 0 > s){
                    cout << "Valor no autorizado para s" << endl;
                    return 1;            
                }
            }

            if(!strcmp(argv[x], "-q")) {
                q = stoi(argv[x + 1]);
                ++x;

                if(q == 0){
                    cout << "Valor no autorizado para q" << endl;
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

        // Crea un objeto reg, y crea una var Escuchando para el ciclo donde escucha al usuario.
        // Si la flag -n es correcta, asigna la memoria a la varible NombreMemoria
        // En caso contrario, cierra el programa.
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

        // Verifica si el argumento 4 es '-' para entrar en modo interactivo.
        if(!strcmp(argv[4], "-"))
        {

            cout << "Ingresando a modo interactivo" << endl;            
            string EntradaUsuario;

            // Ciclo donde se escucha hasta que el usuario ingrese 'exit'
            while(Escuchando)
            {
                cout << ">";
                getline(cin, EntradaUsuario);
                cout << "El usuario ingreso: ";
                cout << EntradaUsuario << endl;

                if(EntradaUsuario == "exit"){
                    Escuchando = false;
                }

                // Se parsea el input del usuario.
                string *EntradaAnalizada = parser(EntradaUsuario);

                Registro.bandeja = stoi(EntradaAnalizada[0]);
                Registro.tipo = EntradaAnalizada[1][0];
                Registro.cantidad = stoi(EntradaAnalizada[2]);


                while(isCiclo){        
                   id = rand() % ((10000 + 1) - 1);
                   Repetido = find(begin(IDExistentes), end(IDExistentes), id) != end(IDExistentes);

                   if (!Repetido){
                    cout << "Es unico" << endl;
                    IDExistentes.insert(begin(IDExistentes), id);
                    isCiclo = false;
                   }
                }

                Registro.id = id;
                ingresarRegistro(Registro, NombreMemoria);


            }

        // Si el cuarto argumento es un path.
        } else {
            
            // Forma estandar de abrir files.
            string NombreFichero(argv[4]);
            ifstream file(NombreFichero);
            if(file.fail())
            {
                cout << "No se puede abrir el archivo" << endl;
            } else {
                cout << "El archivo se abrio con exito" << endl;
            }

            // Forma estandar de recibir lineas de texto de un fichero.
            for(string line; getline(file, line); )
            {
                cout << line << endl;

                // Se parsea el input del fichero.
                string *args = parser(line);

                // Se llama al metodo agregar para ingresar en la memoria el Registro.
                Registro.bandeja = stoi(args[0]);
                Registro.bandeja = args[1][0];
                Registro.cantidad = stoi(args[2]);

                while(isCiclo){        
                   id = rand() % ((10000 + 1) - 1);
                   Repetido = find(begin(IDExistentes), end(IDExistentes), id) != end(IDExistentes);

                   if (!Repetido){
                    cout << "Es unico" << endl;
                    IDExistentes.insert(begin(IDExistentes), id);
                    isCiclo = false;
                   }
                }

                Registro.id = id;
                ingresarRegistro(Registro, NombreMemoria);            

            }
        }

    } else if(!strcmp(argv[1], "ctrl")) {

        // Crea un objeto ctrl, y crea una var Escuchando para el ciclo donde escucha los subcomandos.
        // Si la flag -n es correcta, asigna la memoria a la varible NombreMemoria
        // En caso contrario, cierra el programa.
        // La string command almacena el comando del usuario.

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

            // Se parsea el comando introducido por el usuario.
            string *SegundaEntrada = parser(command);

            // Si el comando es 'list', se llama el metodo list junto con el nombre del espacio de memoria
            // y el comando que lo acompana.
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


            // Si el comando es 'update', asigna las siguientes variables y las manda al metodo update.
            } else if(SegundaEntrada[0] == "update"){
                char Tipo = SegundaEntrada[1][0];
                int Valor = stoi(SegundaEntrada[2].c_str());

                IngresarReactivo(NombreMemoria, Valor, Tipo );
            } 
        }

    // Verifica si el argumento principal es rep
    } else if(!strcmp(argv[1], "rep")){

        // Crea un objeto rep. Si la flag -n es correcta, asigna la memoria a la varible NombreMemoria
        // En caso contrario, cierra el programa.

        if(!strcmp(argv[2], "-n"))
        {
            string Temp(argv[3]);
            NombreMemoria = Temp;
        } else {
            cout << "Variable invalida" << endl;
            return 1;
        }

        // Revisa si el comando '-i' y manda al metodo mensajei junto con la cantidad de segundos.
        if(!strcmp(argv[4], "-i")){
        

        // Revisa si el comando '-m' y manda al metodo mensajej junto con la cantidad de examenes.
        } else if(!strcmp(argv[4], "-m")){
         
        }

    // Revisa si el comando principal es stop.
    } else if(!strcmp(argv[1], "stop")){
        
        // Crea un objeto stop. Si la flag -n es correcta, asigna la memoria a la varible NombreMemoria
        // En caso contrario, cierra el programa.

        if(!strcmp(argv[2], "-n"))
        {
            string Temp(argv[3]);
            NombreMemoria = Temp;

        } else {
            cout << "Variable invalida" << endl;
            return 1;
        }

        // Recibe el nombre de la memoria que va borrar.
        stop(NombreMemoria);

    }


    return 0;
}