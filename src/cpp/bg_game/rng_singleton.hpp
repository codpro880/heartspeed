#pragma once

#include <random>

// Special thanks to Scott Meyers

class RngSingleton
{
private:
    // Private Constructor
    RngSingleton() {};
    // // Stop the compiler generating methods of copy the object
    RngSingleton(RngSingleton const& copy);            // Not Implemented
    RngSingleton& operator=(RngSingleton const& copy); // Not Implemented
    bool initialized = false;
    std::mt19937 rng;
    int seed;
    std::uniform_int_distribution<int> int_dist;

public:
    static RngSingleton& getInstance(int seed_value=0, bool force_reset=false)
    {
	// The only instance
	// Guaranteed to be lazy initialized
	// Guaranteed that it will be destroyed correctly
	static RngSingleton instance;
	if (!instance.initialized || force_reset) {
	    instance.rng.seed(seed_value);
	    instance.initialized = true;
	    instance.seed = seed_value;
	}
	return instance;
    }

    int get_seed() { return seed; }

    int get_rand_int() { return int_dist(rng); };

    // Return between start and stop (inclusive)
    int get_rand_int(int start, int stop) {
	std::uniform_int_distribution<uint32_t> int_dist_interval(start,stop);
	return int_dist_interval(rng);
    }
};
