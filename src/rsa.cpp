#include <iostream>
#include <math.h>
#include <vector>

#include <rsa.hpp>
#include <cryptopp/osrng.h>
#include <cryptopp/integer.h>

using namespace CryptoPP;

void RSA::generate_keys(Integer& n, Integer& d, Integer& e) {
    AutoSeededRandomPool rng;
    AlgorithmParameters params = MakeParameters("BitLength", 512)
                                                ("RandomNumberType", Integer::PRIME);

    Integer p, q;

    p.GenerateRandom(rng, params);
    q.GenerateRandom(rng, params);

    n = p * q;

    Integer phi_n = (p - 1) * (q - 1);

    e = 65537;

    d = e.InverseMod(phi_n);
}

std::pair<std::vector<byte_t>, std::vector<byte_t>> RSA::encrypt_half_split(
    const byte_t* data,
    unsigned size,
    const CryptoPP::Integer& n,
    const CryptoPP::Integer& e,
    unsigned short& padding_length
) {
    const unsigned k_bytes = n.ByteCount() / 2u;
    const unsigned n_bytes = 2u * k_bytes;

    padding_length = static_cast<unsigned short>((k_bytes - (size % k_bytes)) % k_bytes);
    const unsigned total = size + padding_length;
    const unsigned blocks = total / k_bytes;

    std::vector<byte_t> first;
    first.reserve(total);

    std::vector<byte_t> second;
    second.reserve(total);

    for (unsigned b = 0; b < blocks; ++b) {
        byte_t plain_buffer[512] = {0};
        std::memcpy(
            plain_buffer,
            data + b * k_bytes,
            (b == blocks - 1) ? k_bytes - padding_length : k_bytes
        );

        Integer m(plain_buffer, k_bytes);
        Integer c = a_exp_b_mod_c(m, e, n);

        byte_t c_bytes[1024];
        c.Encode(c_bytes, n_bytes);

        first.insert(first.end(), c_bytes, c_bytes + k_bytes);
        second.insert(second.end(), c_bytes + k_bytes, c_bytes + n_bytes);
    }

    return {std::move(first), std::move(second)};
}

std::vector<byte_t> RSA::decrypt_half_join(
    const byte_t* first_half,
    const byte_t* second_half,
    unsigned cipher_pairs,
    unsigned short padding_length,
    const CryptoPP::Integer& n,
    const CryptoPP::Integer& d
) {
    const unsigned k_bytes = n.ByteCount() / 2;
    const unsigned n_bytes = 2u * k_bytes;

    std::vector<byte_t> plain;
    plain.reserve(cipher_pairs * k_bytes);

    for (unsigned b = 0; b < cipher_pairs; ++b) {
        byte_t c_buffer[1024];
        std::memcpy(c_buffer, first_half + b * k_bytes, k_bytes);
        std::memcpy(c_buffer + k_bytes, second_half + b * k_bytes, k_bytes);

        Integer c(c_buffer, n_bytes);
        Integer m = a_exp_b_mod_c(c, d, n);

        byte_t m_bytes[512];
        m.Encode(m_bytes, k_bytes);

        plain.insert(plain.end(), m_bytes, m_bytes + k_bytes);
    }

    plain.resize(plain.size() - padding_length);
    return plain;
}

std::vector<byte_t> RSA::encrypt_block(
    const byte_t* m,
    const unsigned int& in_block_size,
    const Integer& e,
    const Integer& n
) {
    Integer m_int(m, in_block_size);
    Integer c_int = a_exp_b_mod_c(m_int, e, n);

    unsigned int out_block_size = 2 * in_block_size;
    std::vector<byte_t> buffer(out_block_size);
    c_int.Encode(buffer.data(), out_block_size);

    return buffer;
}

std::vector<byte_t> RSA::decrypt_block(
    const byte_t* const c,
    const unsigned int& out_block_size,
    const Integer& d,
    const Integer& n
) {
    Integer c_int(c, 2u * out_block_size);
    Integer m_int = a_exp_b_mod_c(c_int, d, n);

    std::vector<byte_t> buffer(out_block_size);
    m_int.Encode(buffer.data(), buffer.size());
    return buffer;
}
