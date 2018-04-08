#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <gmpxx.h>

using namespace std;

#define big mpz_class
#define ll signed long long
#define basic_length 4

big Exponentiation_De( big& a, big k, big& p )     //a^k % p
{
    if( k == 0 ) return 1;
    big half = Exponentiation_De( a, k / 2, p ) % p;
    half = ( half * half ) % p;
    return ( k % 2 == 0 )? half : ( ( half * a ) % p );
}

void go_deep( vector<big>& V, big& p, ll down, ll up )   //[down; up)
{
    if( up - down == basic_length )     //leaf
    {
        for( ll i = down; i < up - 1; i++ )
            V[i] -= V[i + 1];
        return;
    }

    ll mid = ( down + up ) / 2;

    for( ll i = up - 1; i > down; i-- )
        V[i] = ( V[i] * Exponentiation_De( V[i - 1], p - 2, p ) ) % p;

    for( ll i = down; i < mid; i++ )
        swap( V[i], V[i + mid - down] );

    go_deep( V, p, down, mid );
    go_deep( V, p, mid, up );
}

void decryption( ofstream& html )
{
    ifstream plikin; plikin.open("enc.txt");
    ofstream plikout; plikout.open("dec.txt");
    html<<"\t\t<br><br><h2>Deszyfrowanie</h2>"<<endl;
    html<<"\t\t<p>Deszyfrujemy za pomocą funkcji D(Z, key)"<<endl;
///1. XOR with key
///2. nesting:
///     - inverse % p
///     - swapping halves
///3. in leaf: suffix differences
///4. -> char
///5. output
    html<<"\t\t<br>D( { ";
    vector<big> V;
    while( !plikin.eof() )
    {
        big val; plikin>>val;
        V.push_back(val);
    }
    big key = V[ V.size() - 2 ], p = V[ V.size() - 1 ];

    for( ll i = 0; i < V.size() - 2; i++ )
        html<<V[i]<<" ";
    html<<"}, "<<key<<" ) = ";

    for( ll i = 0; i < V.size() - 2; i++ )      //1.
        V[i] ^= key;

    go_deep( V, p, 0, V.size() - 2 );        //2. & 3.

    for( ll i = 0; i < V.size() - 2; i++ )       //4. & 5.
    {
        char c = V[i].get_si();
        if( c == '%' ) return;
        plikout<<c;
        html<<c;
    }
    html<<"</p>"<<endl;
    plikin.close(); plikout.close();
}
