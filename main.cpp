#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <gmpxx.h>
#include <string>

#include "include/primitive_root.h"
#include "include/sieve.h"
#include "include/diffie.h"
#include "include/encryption.h"
#include "include/decryption.h"
#include "include/shank.h"
#include "include/red.h"
#include "include/red_pxalfa.h"
#include "include/factorization.h"
#include "include/brut.h"

using namespace std;

#define big mpz_class
#define ll signed long long
#define Mem_limit 50000

int main()
{
    ios_base::sync_with_stdio(0);

    big p, g;
    ifstream plik; plik.open("Prime.txt");
    ofstream html; html.open("krypta.html"); html<<"<!DOCTYPE html>"<<endl<<"<html>"<<endl<<"\t<head>"<<endl<<"\t\t<meta charset = UTF-8>"<<endl<<"\t</head>"<<endl<<"\t<body>"<<endl;

    plik>>p; html<<"\t\t<h4>ustalona liczba pierwsza P = "<<p<<"</h4><br><br>"<<endl;
    sieve( Mem_limit, html );
    vector<tuple<big,big,big>> Factors = move( Factorization( p, html ) );
    g = primitive_root( p, Factors, html );      //g - pierwiastek pierwotny mod p
    html<<"\t\t<h4>Pierwiastkiem pierwotnym mod P jest g = "<<g<<"</h4>"<<endl;

    tuple<big,big> D = diffie( p, g, html );
    big key = get<0>(D), gA = get<1>(D);
    encryption(html);
    brut( p, g, gA, html );
    shank( p, g, gA, html );
    Pohlig_Hellman( p, g, gA, Factors, html );
    decryption(html);

    html<<endl<<"\t</body>"<<endl<<"</html>"<<endl;
    plik.close(); html.close();
    return 0;
}
