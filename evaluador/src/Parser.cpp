#include <iostream>
#include <cstdlib>
#include <string.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <fstream>
#include <sstream>

using namespace std;

string * parser(string line){
    int i = 0;
    static string arr[3];
    stringstream ssin(line);

    while (ssin.good() && i < 3){
        ssin >> arr[i];
        ++i;
    }

    return arr;
}