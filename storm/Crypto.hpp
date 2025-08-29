#ifndef STORM_CRYPTO_HPP
#define STORM_CRYPTO_HPP

#include "storm/crypto/SARC4Key.hpp"

void SARC4PrepareKey(const void* data, uint32_t len, SARC4Key* key);

void SARC4ProcessBuffer(void* data, uint32_t len, const SARC4Key* inKey, SARC4Key* outKey);

#endif
