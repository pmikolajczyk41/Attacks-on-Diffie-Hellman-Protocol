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

void go_deep( vector<big>& V, big& p, ll down, ll up )   //[down; up)
{
    if( up - down == basic_length )     //leaf
    {
        for( ll i = up - 2; i >= down; i-- )
            V[i] += V[i + 1];
        return;
    }

    ll mid = ( down + up ) / 2;
    go_deep( V, p, down, mid );
    go_deep( V, p, mid, up );

    for( ll i = down; i < mid; i++ )        //swapping halves
        swap( V[i], V[i + mid - down] );

    for( ll i = down + 1; i < up; i++ )     //prefix products
        V[i] = ( V[i] * V[i - 1] ) % p;
}

void encryption( ofstream& html )
{
    ifstream plikin; plikin.open("to_enc.txt");
    ofstream plikout; plikout.open("enc.txt");
    string S; plikin>>S;
    big key, p; plikin>>key>>p;
    html<<"\t\t<br><br><h2>Szyfrowanie</h2>"<<endl;
    html<<"\t\t<p>Szyfrujemy za pomocą jawnej funkcji E(S, key):"<<endl;
    html<<"\t\t<ol>"<<endl;
    html<<"\t\t\t<li>Dociągamy długość tekstu do potęgi 2</li>"<<endl;
    html<<"\t\t\t<li>Zamieniamy znaki na kod ASCII</li>"<<endl;
    html<<"\t\t\t<li>Schodzimy rekurencyjnie do liści o ustalonej długości 2^k</li>"<<endl;
    html<<"\t\t\t<ul>"<<endl<<"\t\t\t\t<li>Zamiana na sumy sufiksowe</li>"<<endl<<"\t\t\t</ul>"<<endl;
    html<<"\t\t\t<li>Przy wychodzeniu poziom wyżej:</li>"<<endl;
    html<<"\t\t\t<ul>"<<endl;
    html<<"\t\t\t\t<li>Zamiana połowek</li>"<<endl;
    html<<"\t\t\t\t<li>Zamiana na iloczyny prefiksowe</li>"<<endl;
    html<<"\t\t\t</ul>"<<endl;
    html<<"\t\t\t\t<li>XOR wszystkich elementów z key</li>"<<endl;
    html<<"\t\t</ol>"<<endl;
///1. pad text to length of 2^k
///2. change into -> vector <ll>
///3. go down to leaves - make suffix sums
///4. go up:
///     - swap halves
///     - prefix products % p
///5. XOR z key
///6. output

    html<<"\t\t<br>E( "<<S<<", "<<key<<" ) = { ";
    ll Length = ceil ( log2 ( S.size() ) );     //1.
    Length = 1 << Length;
    vector<big> V ( Length, 37 );        //pad with '%'
    for( ll i = 0; i < S.size(); i++ )      //2.
        V[i] = S[i];

    go_deep( V, p, 0, Length );        //3. & 4.

    for( auto i : V )       //5. & 6.
    {
        plikout<<(i^key)<<" ";
        html<<(i^key)<<" ";
    }
    html<<"}</p>"<<endl;
    plikout<<key<<" "<<p;
    plikin.close(); plikout.close();
}

