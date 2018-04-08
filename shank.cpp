#include <iostream>
#include <fstream>
#include <unordered_map>
#include <cmath>
#include <gmpxx.h>
#include <ctime>

using namespace std;

#define big mpz_class

big Exponentiation_Sh( big& a, big k, big& p )     //a^k % p
{
    if( k == 0 ) return 1;
    big half = Exponentiation_Sh( a, k / 2, p ) % p;
    half = ( half * half ) % p;
    return ( k % 2 == 0 )? half : ( ( half * a ) % p );
}

/* p - prime */
void shank( big p, big g, big h, ofstream& html )  // h = g^x (mod p)
{
    html<<"\t\t<br><br><h2>Algorytm Baby-Step-Giant-Step - O( sqrt(P) )</h2>"<<endl;
    clock_t beginning = clock();
    mpf_class m_f = sqrt(p); ceil( m_f );    //x = q*m + r  ==>  h = g^( q*m + r ) (mod p)  <==>   h*( g^-m )^q = g^r (mod p)
    big m; mp_exp_t E;
    string S = m_f.get_str(E);
    m = S;

    big inverse = Exponentiation_Sh( g, p - 2, p );        //g ^ -1
    big alfa = Exponentiation_Sh( inverse, m, p );        //g^-m = ( g^p - 2 )^m (mod p)  ==>  h*alfa^q = g^r (mod p), inverse ^ m

    html<<"\t\t<p>Zapisujemy x jako q x m + r, gdzie m = [sqrt(P)] = "<<m<<"<br>g^(-1) = "<<inverse<<" <br>Zapisujemy wszystkie możliwe wartości g^r<br>"<<endl;
    html<<"\t\tSzukamy kolizji zapisanych wartości g^r i h * [ ( g^(-m) ) ^ q ]</p>"<<endl;

    big q1 ("33452526613163807108170062053440751665152000000001"); if( p == q1 ) html<<"\t\t<p>Pesymistyczny czas ~ 5 x 10^9 lat"<<endl;
    big q2 ("340282366920938463463374607431768223907"); if( p == q2 ) html<<"\t\t<p>Pesymistyczny czas ~ 16 000 lat"<<endl;
    big q3 ("909090909090909090909090909091"); if( p == q3 ) html<<"\t\t<p>Pesymistyczny czas ~ 309 dni"<<endl;
    big q4 ("666666777777788888888999999999"); if( p == q4 ) html<<"\t\t<p>Pesymistyczny czas ~ 260 dni"<<endl;

    if( p == q1 || p == q2 || p == q3 || p == q4 ) { html<<"\t\t<img src = pic/seal.jpg>"<<endl; return; }

    big beta = 1;
    unordered_map<string, big> M;
    for( big r = 0; r < m; r++ )     //g^r
    {
        M.insert( make_pair( beta.get_str(10), r ) );
        beta = ( beta * g ) % p;
    }
    for( big q = 0; q <= m; q++ )        //h * alfa^q
    {
        unordered_map<string, big>::const_iterator iter = M.find( h.get_str(10) );
        if( iter != M.end() )        //collision
        {
            clock_t ending = clock();
            double elapsed_secs = double(ending - beginning) / CLOCKS_PER_SEC;
            html<<"\t\tx = "<<q*m + iter->second<<"  - Znalezione w "<<elapsed_secs<<"s"<<endl;
            return;
        }
        h = ( h * alfa ) % p;
    }
}
