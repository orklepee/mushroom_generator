#pragma once

#include <set>

#include "../src/lib/olcPixelGameEngine.h"

enum class MushSpecies
{
	Unassigned,
	Bolete,
	Chanterelle,
	Roundhead
};

// Struct that handles basic Mushroom characteristics
// Accessed via Mushroom
struct MushBio
{
	MushSpecies Species;
	int mushCapSize;
	int mushStemLength;

	// Declaring default MushSpecies to be unassigned
	MushBio(MushSpecies species = MushSpecies::Unassigned)
		:Species(species)
	{
	}

	bool operator< (const MushBio& spec) const { return Species < spec.Species; }
};

/* Mushroom class handles everything relating to a single instance of a mushroom:
*		- Cap size, stem length, species
*		- Sprite image
*		- Chances of spawning
*		- Sound when selecting
*/
class Mushroom
{
private:
	std::set<MushBio> MushroomBiography;

public:
	MushBio GetMushSpecies(MushSpecies);
};