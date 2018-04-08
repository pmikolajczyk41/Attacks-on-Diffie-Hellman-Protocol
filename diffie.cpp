#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include <gmpxx.h>

using namespace std;

#define big mpz_class

big Exponentiation_Di( big& a, big k, big& p )     //a^k % p
{
    if( k == 0 ) return 1;
    big half = Exponentiation_Di( a, k / 2, p ) % p;
    half = ( half * half ) % p;
    return ( k % 2 == 0 )? half : ( ( half * a ) % p );
}

tuple<big,big> diffie( big p, big g, ofstream& html )
{
    html<<"\t\t<br><br><h2>Protokół Diffiego-Hellmana</h4>"<<endl;
    fstream plikA; plikA.open("A's.txt");
    fstream plikB; plikB.open("B's.txt");
    ofstream plikout; plikout.open("to_enc.txt");
    big a, b, gA, gB, key;
    plikA>>a; plikB>>b;
    gA = Exponentiation_Di( g, a, p );
    gB = Exponentiation_Di( g, b, p );
    key = Exponentiation_Di( gA, b, p );
    string S; plikA>>S;
    plikout<<S<<" "<<key<<" "<<p;
    plikA.close(); plikB.close(); plikout.close();

    html<<"\t\t<table cellpadding = 10>"<<endl<<"\t\t\t<thead>"<<endl<<"\t\t\t\t<tr>"<<endl;
    html<<"\t\t\t\t\t<th scope = col colspan = 3>Wojtek</th>"<<endl<<"\t\t\t\t\t<th scope = col>Julka</th>"<<endl<<"\t\t\t\t\t<th scope = col colspan = 3>Piotrek</th>"<<endl<<"\t\t\t\t</tr>"<<endl<<"\t\t\t</thead>"<<endl;

    html<<"\t\t\t<tbody>"<<endl<<"\t\t\t\t<tr>"<<endl;
    html<<"\t\t\t\t\t<th scope = col>Tajne</th>"<<endl<<"\t\t\t\t\t<th scope = col>Publiczne</th>"<<endl<<"\t\t\t\t\t<th scope = col>Obliczane</th>"<<endl<<"\t\t\t\t\t<th scope = col>Wysyłane</th>"<<endl<<"\t\t\t\t\t<th scope = col>Obliczane</th>"<<endl<<"\t\t\t\t\t<th scope = col>Publiczne</th>"<<endl<<"\t\t\t\t\t<th scope = col>Tajne</th>"<<endl;
    html<<"\t\t\t\t</tr>"<<endl;

    html<<"\t\t\t\t<tr>"<<endl<<"\t\t\t\t\t<td>a</td>"<<endl<<"\t\t\t\t\t<td>P, g</td>"<<endl<<"\t\t\t\t\t<td> </td>"<<endl<<"\t\t\t\t\t<td>P, g -></td>"<<endl<<"\t\t\t\t\t<td> </td>"<<endl<<"\t\t\t\t\t<td> </td>"<<endl<<"\t\t\t\t\t<td>b</td>"<<endl<<"\t\t\t\t</tr>"<<endl;
    html<<"\t\t\t\t<tr>"<<endl<<"\t\t\t\t\t<td>a</td>"<<endl<<"\t\t\t\t\t<td>P, g, A</td>"<<endl<<"\t\t\t\t\t<td>g^a ≡ A (mod P)</td>"<<endl<<"\t\t\t\t\t<td>A -></td>"<<endl<<"\t\t\t\t\t<td> </td>"<<endl<<"\t\t\t\t\t<td>P, g</td>"<<endl<<"\t\t\t\t\t<td>b</td>"<<endl<<"\t\t\t\t</tr>"<<endl;
    html<<"\t\t\t\t<tr>"<<endl<<"\t\t\t\t\t<td>a</td>"<<endl<<"\t\t\t\t\t<td>P, g, A</td>"<<endl<<"\t\t\t\t\t<td> </td>"<<endl<<"\t\t\t\t\t<td><- B</td>"<<endl<<"\t\t\t\t\t<td>g^b ≡ B (mod P)</td>"<<endl<<"\t\t\t\t\t<td>P, g, A, B</td>"<<endl<<"\t\t\t\t\t<td>b</td>"<<endl<<"\t\t\t\t</tr>"<<endl;
    html<<"\t\t\t\t<tr>"<<endl<<"\t\t\t\t\t<td>a, key</td>"<<endl<<"\t\t\t\t\t<td>P, g, A, B</td>"<<endl<<"\t\t\t\t\t<td>B^a ≡ key (mod P)</td>"<<endl<<"\t\t\t\t\t<td> </td>"<<endl<<"\t\t\t\t\t<td>A^b ≡ key (mod P)</td>"<<endl<<"\t\t\t\t\t<td>P, g, A, B</td>"<<endl<<"\t\t\t\t\t<td>b, key</td>"<<endl<<"\t\t\t\t</tr>"<<endl;

    html<<"\t\t\t</tbody>"<<endl<<"\t\t</table>"<<endl;
    html<<"\t\t<p> a = "<<a<<", b = "<<b<<", g^a ≡ "<<gA<<" (mod P), g^b ≡ "<<gB<<" (mod P), key = "<<key<<" </p>"<<endl;

    return make_tuple(key, gA);
}
