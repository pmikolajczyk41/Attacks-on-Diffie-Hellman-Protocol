# Attacks-on-Diffie-Hellman-Protocol
C++ implementation of Diffie-Hellman protocol, 3 different attacks on it (brute force, baby-step giant-step, Pohlig–Hellman reduction) and an auxiliary encryption algorithm 

### Goal
The primary aim of this project was to compare efficiency of some algorithms in solving the discrete logarithm problem as it is the base of security in Diffie-Hellman Protocol. There were 3 methods tested:
  - simple brute force
  - baby-step giant-step (aka Shank's algorithm)
  - Pohlig-Hellman algorithm in two variants (leaving factors with powers (p^a) and reducing exponentiation to multiplication (p\*a))

### Requirements
Project requires GMP library (available at: https://gmplib.org/) and a compilator supporting C++11.

### Auxiliary tools
In order to generate base of prime numbers needed for factorization wheel sieve was used.
To simulate somehow any usage of protocol, I've written a simple encryption (and decryption) algorithm, which for a sequence of letters follows these steps (binary tree of calls):
  1. go recursively down to the blocks of 4 characters (leaves)
  2. compute in every leaf suffix sums (modulo given key)
  3. while returning up the tree: swap children and then compute prefix products (modulo given key)
  4. compute xor with key
  
Decryption algorithm works as an inverse function.

### Annotation
Note that it was my very first project (implemented in high school), so it doesn't necessarily obey coding savoir-vivre. However, all the algorithms work correctly and they are written in such a manner, that one can easily understand the whole work both its general outline and its details.
This project was supposed to be shown to some audience, so there are snippets of raw html between the lines. If you run main.cpp it will produce a .html file so the results and description can be presented in somehow comfortable way.
