#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <cmath>            //sqrt, ceil
#include <unordered_map>        //baby
#include <gmpxx.h>
#include <ctime>

using namespace std;

#define ll signed long long
#define big mpz_class

big Exponentiation_ReX( big& a, big k, big& p )     //a^k % p
{
    if( k == 0 ) return 1;
    big half = Exponentiation_ReX( a, k / 2, p ) % p;
    half = ( half * half ) % p;
    return ( k % 2 == 0 )? half : ( ( half * a ) % p );
}

big Baby_ReX( big& p, big& g, big h, big m )  // h = g^x (mod p)
{
    big inverse = Exponentiation_ReX( g, p - 2, p );        //g ^ -1
    big alfa = Exponentiation_ReX( inverse, m, p );        //g^-m = ( g^p - 2 )^m (mod p)  ==>  h*alfa^q = g^r (mod p), inverse ^ m
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

tuple<big,big> Euclidean_ReX( big a, big b )            //returns e,f : a*e + b*f = gcd(a,b)
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

big Chinese_ReX( vector<tuple<big,big>>& Reminds, big Order )
{
    big X = 0;
    for( ll i = 0; i < Reminds.size(); i++ )
        X += get<1>( Reminds[i] ) * get<1>( Euclidean_ReX( get<0>(Reminds[i]), Order / get<0>(Reminds[i]) ) ) * ( Order / get<0>(Reminds[i]) );
    if( X % Order > 0 ) return (X % Order);
    else return ( Order - abs( X % Order ) );
}

void Pohlig_Hellman_Superstar( big& p, big& g, big h, vector<tuple<big,big,big>>& Factors, ofstream& html )     // h = g^x (mod p)
{
    html<<"\t\t<br><br><h2>Redukcja Pohliga-Hellmana - O(faktoryzacja P-1) + O( alfa x sqrt(q) )</h2> <h3>  (q^alfa - największy dzielnik P taki, że q jest pierwsze)</h3>"<<endl;
    big q ("340282366920938463463374607431768223907"); if( p == q ) html<<"\t\t<p>Pesymistyczny czas ~ 11 500 lat"<<endl;
    if( p == q ) html<<"\t\t<img src = pic/obama.jpg>"<<endl;
    clock_t beginning = clock();
    const big Order = p - 1;
    vector<tuple<big,big>> Reminds( Factors.size() );     // 0 - p_i ^ a_i, 1 - x % 0
    for( ll i = 0; i < Factors.size(); i++ )
    {
        big factor = Exponentiation_ReX( get<0>(Factors[i]), get<1>(Factors[i]), p );        //factor = (p_i ^ a_i)
                                                                //h ^ r_i  =  ( g ^ r_i ) ^ Remind_i
        big Remind = 0, temp_factor = 1;      //temp_factor - p_i ^ a_j; Remind - polynomial W(p_i)
        big inverse = Exponentiation_ReX( g, p - 2, p );        //g ^ -1
        big G = Exponentiation_ReX( g, Order / get<0>(Factors[i]), p );      //g ^ (Order / p_i)

        for( big j = 0; j < get<1>(Factors[i]); j++ )
        {
            mpf_class m_f = sqrt(get<0>(Factors[i])); ceil( m_f );    //x = q*m + r  ==>  h = g^( q*m + r ) (mod p)  <==>   h*( g^-m )^q = g^r (mod p)
            big m; mp_exp_t E;
            string S = m_f.get_str(E);
            m = S;

            big expo = Order / Exponentiation_ReX( get<0>(Factors[i]), j + 1, p );
            big H = Exponentiation_ReX( h, expo, p );      //H = h ^ (Order / p-i ^ j+1)
            big new_inverse = Exponentiation_ReX( inverse, Remind * expo, p);     // = (g^-1) ^ [ Poly * (p-i ^ a-i - 1 - j) * expo ]
            big m_j = Baby_ReX( p, G, (H * new_inverse) % p, m );
            Remind += m_j * temp_factor;
            temp_factor *= get<0>(Factors[i]);
        }
        Reminds[i] = make_tuple( factor, Remind );
    }

    html<<"\t\t<p>Otrzymujemy układ kongruencji: <br></p>"<<endl;
    html<<"\t\t<ul>"<<endl;

    for( ll i = 0; i < Reminds.size(); i++ )
        html<<"\t\t<li>x ≡ "<<get<1>(Reminds[i])<<" (mod "<<get<0>(Reminds[i])<<")"<<"</li>"<<endl;
    html<<"\t\t</ul>"<<endl<<"\t\tKtóry rozwiązujemy za pomocą chińskiego twierdzenia o resztach<br>"<<endl;
    big x = Chinese_ReX( Reminds, Order );

    clock_t ending = clock();
    double elapsed_secs = double(ending - beginning) / CLOCKS_PER_SEC;
    html<<"\t\tx = "<<x<<"  - Znalezione w "<<elapsed_secs<<"s"<<endl;
}
