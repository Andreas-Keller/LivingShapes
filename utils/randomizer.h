/* 	Header only file which supports some functions for randomized number generation 
	
	we use the c++11 feastures here - ie we need a random device as seed, a random engine, and a distributor
*/

#pragma once

#include <random>

class Randomizer {
public:

static inline int randInt(int min, int max) {
	std::uniform_int_distribution<int> dist(min, max);
	return dist(e);
}

static inline float randFloat(float min, float max) {
	std::uniform_real_distribution<float> dist(min, max);
	return dist(e);
}

static inline float randBool() {
	std::uniform_int_distribution<int> dist(0, 1);
	if (dist(e)) return true;
	return false;
}

private:
    static std::random_device r;	//seeder for the engine
    static std::mt19937_64 e;       //mersenne-twister engine

};
