#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <cmath>            	//sqrt, ceil
#include <unordered_map>        //for baby-step giant-step
#include <gmpxx.h>
#include <ctime>

using namespace std;

#define ll signed long long
#define big mpz_class

big Exponentiation_Re( big& a, big k, big& p )     //a^k % p
{
    if( k == 0 ) return 1;
    big half = Exponentiation_Re( a, k / 2, p ) % p;
    half = ( half * half ) % p;
    return ( k % 2 == 0 )? half : ( ( half * a ) % p );
}

big Baby_Re( big& p, big& g, big h, big m )  // h = g^x (mod p)
{
    big inverse = Exponentiation_Re( g, p - 2, p );        //g ^ -1
    big alfa = Exponentiation_Re( inverse, m, p );        //g^-m = ( g^p - 2 )^m (mod p)  ==>  h*alfa^q = g^r (mod p), inverse ^ m
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
        if( iter != M.end() ) return ( q*m + iter->second );        //collision
        h = ( h * alfa ) % p;
    }
    return -1;
}

tuple<big,big> Euclidean_Re( big a, big b )         //returns e,f : a*e + b*f = gcd(a,b)
{
    big p_1 = 0, q_1 = 1, p_2 = 1, q_2 = 0;      //we keep p_2 * a + q_2 * b = gcd(a,b)
    while( b != 0 )
    {
        big alfa = a/b, beta = a % b;
        a = b; b = beta;
        big p_temp = p_1, q_temp = q_1;
        p_1 = p_2 - alfa * p_1;
        q_1 = q_2 - alfa * q_1;
        p_2 = p_temp; q_2 = q_temp;
    }
    return make_tuple( p_2, q_2 );
}

big Chinese_Re( vector<tuple<big,big>>& Reminds, big Order )
{
    big X = 0;
    for( ll i = 0; i < Reminds.size(); i++ )
        X += get<1>( Reminds[i] ) * get<1>( Euclidean_Re( get<0>(Reminds[i]), Order / get<0>(Reminds[i]) ) ) * ( Order / get<0>(Reminds[i]) );
    if( X % Order > 0 ) return (X % Order);
    else return ( Order - abs( X % Order ) );
}

void Pohlig_Hellman( big& p, big& g, big h, vector<tuple<big,big,big>>& Factors, ofstream& html )     // h = g^x (mod p)
{
    html<<"\t\t<br><br><h2>Redukcja Pohliga-Hellmana - O(faktoryzacja P-1) + O( alfa x sqrt(q) )</h2> <h3>  (q^alfa - największy dzielnik P taki, że q jest pierwsze)</h3>"<<endl;
    big q ("340282366920938463463374607431768223907"); if( p == q ) html<<"\t\t<p>Pesymistyczny czas ~ 11 500 lat"<<endl;
    if( p == q ) { html<<"\t\t<img src = pic/obama.jpg>"<<endl; return; }
    clock_t beginning = clock();
    const big Order = p - 1;
    vector<tuple<big,big>> Reminds( Factors.size() );     // 0 - p_i ^ a_i, 1 - x % 0
    for( ll i = 0; i < Factors.size(); i++ )
    {
        big factor = Exponentiation_Re( get<0>(Factors[i]), get<1>(Factors[i]), p );        //factor = (p_i ^ a_i)
        get<2>(Factors[i]) = Order / factor;        //we compute r_i = (p-1) / (p_i ^ a_i)
                                                                //h ^ r_i  =  ( g ^ r_i ) ^ Remind_i
        big H = Exponentiation_Re( h, get<2>(Factors[i]), p );      //h ^ r_i
        big G = Exponentiation_Re( g, get<2>(Factors[i]), p ), G_prim = 1;      //g ^ r_i
        mpf_class m_f = sqrt(get<0>(Factors[i])); ceil( m_f );    //x = q*m + r  ==>  h = g^( q*m + r ) (mod p)  <==>   h*( g^-m )^q = g^r (mod p)
        big m; mp_exp_t E;
        string S = m_f.get_str(E);
        m = S;
        if( get<1>(Factors[i]) == 1 ) Reminds[i] = make_tuple( factor, Baby_Re( p, G, H, m ) ); continue;

        for( big j = 0; j < factor; j++ )
        {
            if( H == G_prim ) { Reminds[i] = make_tuple( factor, j ); break; }
            G_prim = ( G_prim * G ) % p;
        }
    }
    html<<"\t\t<p>Otrzymujemy układ kongruencji: <br></p>"<<endl;
    html<<"\t\t<ul>"<<endl;

    for( ll i = 0; i < Reminds.size(); i++ )
        html<<"\t\t<li>x ≡ "<<get<1>(Reminds[i])<<" (mod "<<get<0>(Reminds[i])<<")"<<"</li>"<<endl;
    html<<"\t\t</ul>"<<endl<<"\t\tKtóry rozwiązujemy za pomocą chińskiego twierdzenia o resztach<br>"<<endl;
    big x = Chinese_Re( Reminds, Order );

    clock_t ending = clock();
    double elapsed_secs = double(ending - beginning) / CLOCKS_PER_SEC;
    html<<"\t\tx = "<<x<<"  - Znalezione w "<<elapsed_secs<<"s"<<endl;
}
