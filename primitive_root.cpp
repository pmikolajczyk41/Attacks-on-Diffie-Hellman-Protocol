#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>            //sqrt, ceil
#include <gmpxx.h>

using namespace std;

#define ll signed long long
#define big mpz_class

big Exponentiation_Pr( big& a, big k, big& p )     //a^k % p
{
    if( k == 0 ) return 1;
    big half = Exponentiation_Pr( a, k / 2, p ) % p;
    half = ( half * half ) % p;
    return ( k % 2 == 0 )? half : ( ( half * a ) % p );
}

big primitive_root( big p, vector<tuple<big,big,big>>& Factors, ofstream& html )        //returns the smallest primitive root mod p
{
    big Order = p - 1;
    html<<endl<<"\t\t<br><br><h2>Szukanie pierwiastka pierwotnego mod P</h2>"<<endl;
    html<<"\t\tSprawdzamy dla każdej liczby g < P i q będącego pierwszym dzielnikiem P, czy g ^ [(P - 1) / q] ≡ 1  (mod P): <br>"<<endl;
    for( big g = 2; g <= Order; g++ )
    {
        html<<"\t\t<h4>g = "<<g<<":</h4>"<<endl;
        html<<"\t\t<ul>"<<endl;
        bool bull = true;
        for( ll p_i = 0; p_i < Factors.size(); p_i++ )
            {
               html<<"\t\t\t<li>g ^ ["<<Order<<" / "<<get<0>(Factors[p_i])<<"] ≡ "<<Exponentiation_Pr( g, Order / get<0>(Factors[p_i]), p )<<"  (mod "<<p<<")</li>"<<endl;
               if( Exponentiation_Pr( g, Order / get<0>(Factors[p_i]), p ) == 1 ) { bull = false; break; }
            }
        if( bull ) { html<<"\t\t</ul>"<<endl; return g; }
        html<<"\t\t</ul>"<<endl;
    }
}
