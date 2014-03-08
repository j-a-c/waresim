#ifndef SIM_RANDOM_RAND_H
#define SIM_RANDOM_RAND_H

#include <random>

/**
 * A random number generator that uses a 32-bit Mersenne twister
 * to return real numbers in the uniform distribution [0,1). These Rand's are
 * initially seeded using std::rand() in order to allow multithreaded
 * replays.
 */
class Rand
{
    public:
        Rand();
        Rand(unsigned int);
        ~Rand();
        // Returns the next double in the sequence.
        double rand();
    private:
        // Seed this generator. Should only be called once.
        void seed(unsigned int);

        std::mt19937 gen;
        std::uniform_real_distribution<> dis;
};

#endif
