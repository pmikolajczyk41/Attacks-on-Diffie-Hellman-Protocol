#ifndef RED_H_INCLUDED
#define RED_H_INCLUDED

void Pohlig_Hellman( mpz_class& p, mpz_class& g, mpz_class h, std::vector<std::tuple<mpz_class, mpz_class, mpz_class>>& Factors, std::ofstream& html );

#endif // RED_H_INCLUDED
