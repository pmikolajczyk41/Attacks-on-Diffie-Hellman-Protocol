#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

#define ll signed long long

vector<ll> T = {7, 11, 13, 17, 19, 23, 29, 31};

ll F( ll C )
{
    if( (C - 1) % 30 == 0 ) return ( 8 * (C/30 - 1) + 7 );
    ll quotient = 8 * (C/30);
    for( ll i = 0; i < T.size(); i++ )
        if( C % 30 == T[i] ) return ( quotient + i);
}

void sieve( ll Mem_limit, ofstream& html )      //wiil find primes <= 3,75 * Mem_limit + 31
{
    html<<endl<<"\t\t<h2>Wheel factorization (2/3/5)</h2>"<<endl;
    html<<"\t\t<p>Znalezienie liczb pierwszych w zakresie [2; 3,75n], n = "<<Mem_limit<<"  -->  zł. czas. ~ 1/4 x O(norm sito), zł. pam. ~ O(n/4)</p>"<<endl;
    vector<bool> V( Mem_limit, true );
    ofstream plik; plik.open("primes.txt");
    plik<<2<<" "<<3<<" "<<5<<" ";

    ///V[p]:   30 * (p/8) + T[ p%8 ]
    ///   {  30 | (C-1)   ->  8 * (C/30 - 1) + 7
    ///C ={
    ///   {  !30 | (C-1)  ->  8 * C/30 + F( C%8 )

    for( ll p = 0; p < V.size(); p++ )
    {
        ll P = 30 * (p/8) + T[ p%8 ];
        if( V[p] ) plik<<P<<" ";
        for( ll q = p; ; q++ )
        {
            ll Q = 30 * (q/8) + T[ q%8 ];
            ll PQ = P * Q, pq = F(PQ);
            if( pq >= Mem_limit ) break;
            V[pq] = false;
        }
    }
    plik.close();
}
