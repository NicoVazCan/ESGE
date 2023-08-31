/**
 * \file ESGE_hash.h
 * \brief Contains a hash function implementation.
 */
#ifndef ESGE_HASH_H_
# define ESGE_HASH_H_

# include <SDL2/SDL.h>



/**
 * \def ESGE_Hash
 * \brief Hashes a char string using a djb2 algorithm.
 *
 * Computes a hash value for the provided char string using a djb2 hash algorithm (http://www.cse.yorku.ca/~oz/hash.html).
 *
 * \param STR The input string to be hashed.
 * \return The computed hash value.
 */
#define ESGE_Hash(STR) ESGE_RecHash(STR, 5381)

/**
 * \brief Recursively computes a hash value for a char string.
 *
 * This function is used by the ESGE_Hash macro to compute the hash value of a char string.
 *
 * \param str Pointer to the current character in the char string.
 * \param hash The accumulated hash value.
 * \return The computed hash value.
 */
static constexpr Uint64
ESGE_RecHash(const char *str, Uint64 hash)
{
  return (
    *str ? ESGE_RecHash(str+1, ((hash << 5) + hash) + *str) : hash
  );
}

#endif