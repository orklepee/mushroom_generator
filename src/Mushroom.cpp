#include "Mushroom.h"

MushBio Mushroom::GetMushSpecies(MushSpecies species)
{
	//return MushroomBiography.begin();
	return MushSpecies::Bolete;
}

// In the main MushroomGenerator.cpp file you would create a mushroom like so:
// Call where mushExists percentages are calculated. Instead, pass:
//		Mushroom(mushroom, MushBio(MushSpecies::Bolete))