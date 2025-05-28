#include <iostream>
#include <math.h>

#include <rsa.hpp>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>

using namespace CryptoPP;

void RSA::generate_keys(Integer& n, Integer& d, Integer& e) {
    AutoSeededRandomPool rng;
    AlgorithmParameters params = MakeParameters("BitLength", 1024)
                                                ("RandomNumberType", Integer::PRIME);

    Integer p, q;

    p.GenerateRandom(rng, params);
    q.GenerateRandom(rng, params);

    n = p * q;

    Integer phi_n = (p - 1) * (q - 1);

    e = 65537;

    d = e.InverseMod(phi_n);
}

void RSA::encrypt(
    byte_t* data,
    unsigned int& size,
    CryptoPP::Integer& n,
    CryptoPP::Integer& d,
    CryptoPP::Integer& e,
    const bool& use_ecb
) {
    unsigned int block_size = n.ByteCount() / 2u;
    unsigned int block_count = std::ceil(static_cast<double>(size) / block_size);
    
    byte_t new_data[2u * size];
  
    for (auto i = 0u; i < block_count; ++i) {
        encrypt_block(
            data + (i * block_size), 
            new_data + (i * 2u * block_size), 
            std::min(block_size, size - (i + 1u) * block_size),
            e,
            n
        );
    }

    std::copy(new_data, new_data + size, data);
    // size *= 2u;
}

void RSA::encrypt_block(
    byte_t* const m, 
    byte_t* const c,
    const unsigned int& in_block_size,
    Integer& e,
    Integer& n
) {
    Integer m_int(m, in_block_size);
    Integer c_int = a_exp_b_mod_c(m_int, e, n);
    c_int.Encode(c, 2u * in_block_size);
}
