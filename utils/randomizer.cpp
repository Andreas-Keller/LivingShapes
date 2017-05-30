#include "randomizer.h"

std::random_device Randomizer::r;	//seeder for the engine
std::mt19937_64 Randomizer::e(r());
