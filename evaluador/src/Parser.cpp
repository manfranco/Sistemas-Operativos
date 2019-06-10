#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>

using namespace std;

string * parser(string Linea){
    int i = 0;
    static string Arreglo[3];
    stringstream ssin(Linea);

    while (ssin.good() && i < 3){
        ssin >> Arreglo[i];
        ++i;
    }

    return Arreglo;
}