#ifndef PRIMITIVE_ROOT_H_INCLUDED
#define PRIMITIVE_ROOT_H_INCLUDED

mpz_class primitive_root( mpz_class p, std::vector<std::tuple<mpz_class, mpz_class, mpz_class>>& Factors, std::ofstream& html );

#endif // PRIMITIVE_ROOT_H_INCLUDED
