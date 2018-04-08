#include <iostream>
#include <fstream>
#include <vector>
#include <tuple>
#include <cmath>
#include <gmpxx.h>

using namespace std;

#define big mpz_class

vector<tuple<big,big,big>> Factorization( big p, ofstream& html )
{
    big Order = p - 1;
    html<<endl<<"\t\t<br><br><h2>Faktoryzacja rozmiaru grupy multiplikatywnej modulo P</h2>"<<endl;
    html<<"\t\t<p>P - 1 = "<<p - 1<<" = ";
    vector<tuple<big,big,big>> Factors;        // 0 - p_i, 1 - a_i, 2 - r_i
    big q ("340282366920938463463374607431768223907");
    big a ("170141183460469231731687303715884111953");
    if(p==q) {Factors.emplace_back(make_tuple(2,1,0)); Factors.emplace_back(make_tuple(a,1,0)); return Factors;}
    ifstream pierwsze; pierwsze.open("primes.txt");
    big factor;      //current factor, not essentially prime
    for( big i = 0; i <= sqrt( p ) + 1; i++ )
    {
        if( !pierwsze.eof() ) pierwsze>>factor;
        else factor++;
        if( Order % factor != 0 ) continue;     //doesn't divide
        Factors.emplace_back( make_tuple( factor, 0, 0 ) );
        while( Order % factor == 0 )
        {
            get<1>( Factors[Factors.size() - 1] )++;        //power of factor
            Order /= factor;
        }
        if( Factors.size() == 1 && get<1>( Factors[Factors.size() - 1]) == 1 ) html<<get<0>( Factors[Factors.size() - 1] );
        else if( Factors.size() == 1 && get<1>( Factors[Factors.size() - 1]) > 1 ) html<<get<0>( Factors[Factors.size() - 1] )<<"^"<<get<1>( Factors[Factors.size() - 1] );
        else if( get<1>( Factors[Factors.size() - 1]) == 1 ) html<<" x "<<get<0>( Factors[Factors.size() - 1] );
        else html<<" x "<<get<0>( Factors[Factors.size() - 1] )<<"^"<<get<1>( Factors[Factors.size() - 1] );
        if( Order == 1 ) { pierwsze.close(); html<<"</p>"<<endl; return Factors; }        //factorized
    }

    pierwsze.close();
    Factors.emplace_back( make_tuple( Order, 1, 0 ) );
    html<<" x "<<Order<<"</p>"<<endl;
    return Factors;
}
