#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <gmpxx.h>
#include <ctime>

using namespace std;

#define big mpz_class

big Exponentiation_Br( big& a, big k, big& p )     //a^k % p
{
    if( k == 0 ) return 1;
    big half = Exponentiation_Br( a, k / 2, p ) % p;
    half = ( half * half ) % p;
    return ( k % 2 == 0 )? half : ( ( half * a ) % p );
}

void brut( big& p, big& g, big& h, ofstream& html )  // h = g^x (mod p)
{
    html<<"\t\t<br><br><h2>Brut - O(P)</h2>"<<endl;
    html<<"\t\t<p>Sprawdzamy wszystkie potencjalne rozwiązania kongruencji: "<<g<<"^x ≡ "<<h<<" (mod P)</p>"<<endl;

    big q1 ("33452526613163807108170062053440751665152000000001"); if( p == q1 ) html<<"\t\t<p>Pesymistyczny czas ~ 3 x 10^34 lat"<<endl;
    big q2 ("340282366920938463463374607431768223907"); if( p == q2 ) html<<"\t\t<p>Pesymistyczny czas ~ 3 x 10^23 lat"<<endl;
    big q3 ("909090909090909090909090909091"); if( p == q3 ) html<<"\t\t<p>Pesymistyczny czas ~ 8 x 10^14 lat"<<endl;
    big q4 ("666666777777788888888999999999"); if( p == q4 ) html<<"\t\t<p>Pesymistyczny czas ~ 6 x 10^14 lat"<<endl;

    if( p == q1 || p == q2 || p == q3 || p == q4 ) html<<"\t\t<img src = pic/panda.jpg>"<<endl;
    else if( p > 1000000009 ) { html<<"\t\t<img src = pic/laugh.jpg>"<<endl; return; }
    clock_t beginning = clock();

    for( big x = 0; x < p; x++ )
        if( Exponentiation_Br( g, x, p ) == h )
        {
            clock_t ending = clock();
            double elapsed_secs = double(ending - beginning) / CLOCKS_PER_SEC;
            html<<"\t\tx = "<<x<<"  - Znalezione w "<<elapsed_secs<<"s"<<endl;
            return;
        }
}
