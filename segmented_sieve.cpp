/// @file     segmented_sieve.cpp
/// @author   Kim Walisch, <kim.walisch@gmail.com> 
/// @brief    This is a simple implementation of the segmented sieve of
///           Eratosthenes with a few optimizations. It generates the
///           primes below 10^9 in 0.9 seconds (single-threaded) on an
///           Intel Core i7-4770 CPU (3.4 GHz) from 2013.
/// @license  Public domain.

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <stdint.h>

/// Set your CPU's L1 data cache size (in bytes) here
const int64_t L1D_CACHE_SIZE = 32768;

/// Generate primes using the segmented sieve of Eratosthenes.
/// This algorithm uses O(n log log n) operations and O(sqrt(n)) space.
/// @param limit  Sieve primes <= limit.
///
void segmented_sieve(int64_t limit)
{
  int64_t sqrt = (int64_t) std::sqrt(limit);
  int64_t segment_size = std::max(sqrt, L1D_CACHE_SIZE);
  int64_t count = (limit < 2) ? 0 : 1;

  // we sieve primes >= 3
  int64_t i = 3;
  int64_t n = 3;
  int64_t s = 3;

  std::vector<char> sieve(segment_size);
  std::vector<char> is_prime(sqrt + 1, true);
  std::vector<int64_t> primes;
  std::vector<int64_t> indexes;

  for (int64_t low = 0; low <= limit; low += segment_size)
  {
    std::fill(sieve.begin(), sieve.end(), true);

    // current segment = [low, high]
    int64_t high = std::min(low + segment_size - 1, limit);
    int64_t sqrt_high = (int64_t) std::sqrt(high);

    // generate sieving primes using simple sieve of Eratosthenes
    for (; i <= sqrt_high; i += 2)
      if (is_prime[i])
        for (int64_t j = i * i; j <= sqrt; j += i)
          is_prime[j] = false;

    // initialize sieving primes for segmented sieve
    for (; s <= sqrt_high; s += 2)
    {
      if (is_prime[s])
      {
         primes.push_back(s);
        indexes.push_back(s * s - low);
      }
    }

    // segmented sieve
    for (std::size_t i = 0; i < primes.size(); i++)
    {
      int64_t j = indexes[i];
      for (int64_t k = primes[i] * 2; j < segment_size; j += k)
        sieve[j] = false;
      indexes[i] = j - segment_size;
    }

    for (; n <= high; n += 2)
      if (sieve[n - low]) // n is a prime
        count++;
  }

  std::cout << count << " primes found." << std::endl;
}

/// Usage: ./segmented_sieve n
/// @param n  Sieve the primes up to n.
///
int main(int argc, char** argv)
{
  if (argc >= 2)
    segmented_sieve(std::atol(argv[1]));
  else
    segmented_sieve(1000000000);

  return 0;
}
