#include "rand.h"

/**
 * Implementation for rand.h
 */


/**
 * Default constructor. Default seed is 1.
 */
Rand::Rand()
{
    this->seed(1);
}

/**
 * Constructor.
 *
 * @param seed The seen for this Rand.
 */
Rand::Rand(unsigned int seed)
{
    this->seed(seed);
}

/**
 * Destructor.
 */
Rand::~Rand()
{

}

/**
 * Seed the Rand.
 *
 * @param seed The seed.
 */
void Rand::seed(unsigned int seed)
{
    gen.seed(seed);
}

/**
 * Returns the next double in the sequence.
 */
double Rand::rand()
{
    return dis(gen);
}

/**
 * Returns the underlying URNG.
 */
std::mt19937 Rand::get_urng()
{
    return gen;
}
