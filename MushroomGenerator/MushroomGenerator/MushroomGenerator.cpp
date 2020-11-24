/* Goal: to spawn procedurally generated mushroom sprites */
#define OLC_PGE_APPLICATION
#include "stb_image.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"
#include <cmath>
#include<fstream>
#include <Windows.h>
#include <random>
#define OLC_PGEX_SOUND_H
#define _USE_MATH_DEFINES
using namespace std;

struct Mushroomobj
{
	double height = 0.0;
	double width = 0.0;
	// A = pi * r^2/2 for semi-circle aka Mushroom Cap
	double radius = 0.0;
	double distance = 0.0;
	double population = 0.0;
	bool ring = false;
	
};

struct Mushie
{
	double population = 0.0;


};


class cValley
{




public:
	cValley(uint32_t x, uint32_t y, bool bGenerateFullSystem = false)
	{
		
		// Set seed based on location of star system
		nProcGen = (x & 0xFFFF) << 16 | (y & 0xFFFF);

		// Not all locations contain a system
		
		mushExists = (rndInt(0, 70) == 1);
		mushtype2 = (rndInt(0, 2) == 1);
		mushtype3 = (rndInt(0, 6) == 1);
		if (!mushExists) return;

		// Generate Mushroom
		mushStipeWidth = rndDouble(10.0, 12);
		mushStipeHeight = rndDouble(10.0, 12.0);
		mushCapRadius = rndDouble(1.0, 50.0);

		// When viewing the galaxy map, we only care about the star
		// so abort early
		if (!bGenerateFullSystem) return;

		// If we are viewing the system map, we need to generate the
		// full system

		// Generate Planets
		double dDistanceFromAnother = rndDouble(100.0, 200.0);
		int nPlanets = rndInt(0, 10);
		for (int i = 0; i < nPlanets; i++)
		{
			Mushroomobj m;
			m.distance = dDistanceFromAnother;
			dDistanceFromAnother += rndDouble(20.0, 200.0);
			m.width = rndDouble(4.0, 200.0);
			m.height = rndDouble(4.0, 200.0);
			//m.sprMush = new olc::Sprite("mushgen.png");
			m.population = std::max(rndInt(-5000000, 20000000), 0);
			// 30% of mushrooms have ring
			m.ring = rndInt(0, 10) == 3;

			// Children (Baby mushrooms)
			/*int nMoons = std::max(rndInt(-5, 5), 0);
			for (int n = 0; n < nMoons; n++)
			{
				// A moon is just a diameter for now, but it could be
				// whatever you want!
				p.vMoons.push_back(rndDouble(1.0, 5.0));
			}

			// Add planet to vector
			vPlanets.push_back(p);*/
		}
	}

	~cValley(){}

public:
	//std::vector<sPlanet> vPlanets;

	

public:
	
	bool        mushtype3 = false;
	bool        mushtype2 = false;
	bool		mushExists = false;
	double		mushStipeWidth = 0.0f;
	double		mushStipeHeight = 0.0f;
	double		mushCapRadius = 0.0f;
	olc::Pixel	mushCapColour = olc::DARK_RED, mushStipeColour = olc::WHITE;
	
private:
	uint32_t nProcGen = 0;

	double rndDouble(double min, double max)
	{
		return ((double)rnd() / (double)(0x7FFFFFFF)) * (max - min) + min;
	}

	int rndInt(int min, int max)
	{
		return (rnd() % (max - min)) + min;
	}

	// Modified from this for 64-bit systems:
	// https://lemire.me/blog/2019/03/19/the-fastest-conventional-random-number-generator-that-can-pass-big-crush/
	// Now I found the link again - Also, check out his blog, it's a fantastic resource!
	uint32_t rnd()
	{
		nProcGen += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nProcGen * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}
};



class cMushroomHeaven : public olc::PixelGameEngine
{

/*private:
	olc::vi2d vBlockSize = { 16,16 };
	std::unique_ptr<int[]> blocks;*/
public:
	
	cMushroomHeaven()
	{

		sAppName = "Mushroom Heaven";

	}

public:
	
	olc::Sprite* sprMush = nullptr;
	olc::Sprite* sprMush2 = nullptr;
	olc::Sprite* sprMush3 = nullptr;
	olc::Sprite* water = nullptr;
	
	

	bool OnUserCreate() override
	{
		
		sprMush = new olc::Sprite("C:/Users/Colorian/Desktop/Projects/H4M/res/mushgen.png");
		sprMush2 = new olc::Sprite("C:/Users/Colorian/Desktop/Projects/H4M/res/gen.png");
		sprMush3 = new olc::Sprite("C:/Users/Colorian/Desktop/Projects/H4M/res/gen2.png");
		water = new olc::Sprite("C:/Users/Colorian/Desktop/Projects/H4M/res/water.png");
		

		
		return true;
	}

	olc::vf2d vMushroomHeavenOffset = { 0,0 };
	bool bMushSelected = false;
	uint32_t nSelectedMushSeed1 = 0;
	uint32_t nSelectedMushSeed2 = 0;
	
	
	/*uint32_t nLehmer = 0;
	uint32_t Lehmer32()
	{
		nLehmer += 0xe120fc15;
		uint64_t tmp;
		tmp = (uint64_t)nLehmer * 0x4a39b70d;
		uint32_t m1 = (tmp >> 32) ^ tmp;
		tmp = (uint64_t)m1 * 0x12fad5c9;
		uint32_t m2 = (tmp >> 32) ^ tmp;
		return m2;
	}*/

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		
		//play_music();
		Mushroomobj m;
		if (fElapsedTime <= 0.0001f) return true;
		Clear(olc::DARK_GREEN);

		//if (GetKey(olc::SPACE).bReleased)
		//{

		//	//srand(1000);

		//	std::random_device rd;
		//	std::mt19937 mt(1000);
		//	std::uniform_int_distribution<int> dist(0, 256);

		//	auto tp1 = std::chrono::system_clock::now();
		//	// Ranomness Tests
		//	for (int x = 0; x < ScreenWidth(); x++)
		//	{
		//		for (int y = 0; y < ScreenHeight(); y++)
		//		{
		//			bool bIsStar = false;
		//			int nSeed = y << 16 | x;
		//
		//			// Standard C++ rand()
		//			//srand(nSeed);
		//			//bIsStar = rand() % 256 < 32;

		//			// std::random
		//			//mt.seed(nSeed);
		//			//bIsStar = dist(mt) < 32;

		//			// Lehmer32
		//			nLehmer = nSeed;
		//			bIsStar = Lehmer32() % 256 < 32;

		//			Draw(x, y, bIsStar ? olc::WHITE : olc::BLACK);
		//		}
		//	}
		//	auto tp2 = std::chrono::system_clock::now();
		//	std::chrono::duration<float> elapsedTime = tp2 - tp1;
		//	DrawString(3, 3, "Time: " + std::to_string(elapsedTime.count()), olc::RED, 2);
		//}


		//return true;

		
		if (GetKey(olc::W).bHeld) vMushroomHeavenOffset.y -= 50.0f * fElapsedTime;
		if (GetKey(olc::S).bHeld) vMushroomHeavenOffset.y += 50.0f * fElapsedTime;
		if (GetKey(olc::A).bHeld) vMushroomHeavenOffset.x -= 50.0f * fElapsedTime;
		if (GetKey(olc::D).bHeld) vMushroomHeavenOffset.x += 50.0f * fElapsedTime;

		int nSectorsX = ScreenWidth() / 16;
		int nSectorsY = ScreenHeight() / 16;

		olc::vi2d mouse = { GetMouseX() / 16, GetMouseY() / 16 };
		olc::vi2d galaxy_mouse = mouse + vMushroomHeavenOffset;
		olc::vi2d screen_sector = { 0,0 };

		for (screen_sector.x = 0; screen_sector.x < nSectorsX; screen_sector.x++)
			for (screen_sector.y = 0; screen_sector.y < nSectorsY; screen_sector.y++)
			{
				uint32_t seed1 = (uint32_t)vMushroomHeavenOffset.x + (uint32_t)screen_sector.x;
				uint32_t seed2 = (uint32_t)vMushroomHeavenOffset.y + (uint32_t)screen_sector.y;

				cValley Mushroom(seed1, seed2);
				
				if (Mushroom.mushExists && !Mushroom.mushtype2 && !Mushroom.mushtype3)
				{
					double pi = 2 * acos(0.0);
					float power = 2.0;
					float base = Mushroom.mushCapRadius;
					float square = pow(base, power);
					float squareroot = sqrt(square);
					
					SetPixelMode(olc::Pixel::ALPHA);
					DrawSprite({ screen_sector.x * 16 - 8, screen_sector.y * 16 - 8 }, sprMush);

					//DrawSprite({ screen_sector.x * 16 + 8, screen_sector.y * 16 + 8 }, sprMush2);
					SetPixelMode(olc::Pixel::NORMAL);
					//DrawPartialSprite(screen_sector.x * 16 + 8,screen_sector.y * 16+8, sprMush, 1 * 9, 0, 44, 60);


					// For convenience highlight hovered mushroom
					if (mouse.x == screen_sector.x && mouse.y == screen_sector.y)
					{
						DrawRect(screen_sector.x * 16, screen_sector.y * 16 - 8, 44, 60, olc::YELLOW);
					}


				}

				else if (Mushroom.mushExists && Mushroom.mushtype2 && !Mushroom.mushtype3) {
					SetPixelMode(olc::Pixel::ALPHA);
					DrawSprite({ screen_sector.x * 16 - 8, screen_sector.y * 16 - 8 }, sprMush2);

					//DrawSprite({ screen_sector.x * 16 + 8, screen_sector.y * 16 + 8 }, sprMush2);
					SetPixelMode(olc::Pixel::NORMAL);
					//DrawPartialSprite(screen_sector.x * 16 + 8,screen_sector.y * 16+8, sprMush, 1 * 9, 0, 44, 60);


					// For convenience highlight hovered mushroom
					if (mouse.x == screen_sector.x && mouse.y == screen_sector.y)
					{
						DrawRect(screen_sector.x * 16, screen_sector.y * 16 - 8, 44, 60, olc::YELLOW);
					}

				}
				else if (Mushroom.mushExists && Mushroom.mushtype3 && !Mushroom.mushtype2) {
					SetPixelMode(olc::Pixel::ALPHA);
					DrawSprite({ screen_sector.x * 16 - 8, screen_sector.y * 16 - 8 }, sprMush3);

					//DrawSprite({ screen_sector.x * 16 + 8, screen_sector.y * 16 + 8 }, sprMush2);
					SetPixelMode(olc::Pixel::NORMAL);
					//DrawPartialSprite(screen_sector.x * 16 + 8,screen_sector.y * 16+8, sprMush, 1 * 9, 0, 44, 60);


					// For convenience highlight hovered mushroom
					if (mouse.x == screen_sector.x && mouse.y == screen_sector.y)
					{
						DrawRect(screen_sector.x * 16, screen_sector.y * 16 - 8, 44, 60, olc::YELLOW);
					}

				}
			}

		// Handle Mouse Click
		if (GetMouse(0).bPressed)
		{
			uint32_t seed1 = (uint32_t)vMushroomHeavenOffset.x + (uint32_t)mouse.x;
			uint32_t seed2 = (uint32_t)vMushroomHeavenOffset.y + (uint32_t)mouse.y;

			cValley Mushroom(seed1, seed2);
			if (Mushroom.mushExists)
			{
				bMushSelected = true;
				nSelectedMushSeed1 = seed1;
				nSelectedMushSeed2 = seed2;
			}
			else
				bMushSelected = false;
		}

		// Draw Details of selected star system
		if (bMushSelected)
		{
			// Generate full star system
			cValley Mushroom(nSelectedMushSeed1, nSelectedMushSeed2, true);

			// Draw Window
			if (Mushroom.mushExists && !Mushroom.mushtype2 && !Mushroom.mushtype3) {
				FillRect(8, 352, 140, 120, olc::DARK_BLUE);
				DrawRect(8, 352, 140, 120, olc::WHITE);
				DrawString(76, 362, "Bolete", olc::WHITE);
				DrawString(76, 378, "Common", olc::GREEN);
				// Draw Star
				olc::vi2d vBody = { 14, 356 };
				SetPixelMode(olc::Pixel::ALPHA);
				DrawSprite(vBody, sprMush);
				SetPixelMode(olc::Pixel::NORMAL);
				//FillCircle(vBody, (int)(Mushroom.mushStipeWidth * 1.375), Mushroom.mushCapColour);
				//vBody.x += (Mushroom.mushStipeWidth * 1.375) + 8;
			};
			


			// Draw Planets
			/*for (auto& planet : star.vPlanets)
			{
				if (vBody.x + planet.diameter >= 496) break;

				vBody.x += planet.diameter;
				FillCircle(vBody, (int)(planet.diameter * 1.0), olc::RED);

				olc::vi2d vMoon = vBody;
				vMoon.y += planet.diameter + 10;

				// Draw Moons
				for (auto& moon : planet.vMoons)
				{
					vMoon.y += moon;
					FillCircle(vMoon, (int)(moon * 1.0), olc::GREY);
					vMoon.y += moon + 10;
				}

				vBody.x += planet.diameter + 8;
			}*/
		}
		if (bMushSelected)
		{
			// Generate full star system
			cValley Mushroom(nSelectedMushSeed1, nSelectedMushSeed2, true);
			if (Mushroom.mushExists && Mushroom.mushtype2 && !Mushroom.mushtype3) {
				// Draw Window
				FillRect(8, 352, 148, 120, olc::DARK_BLUE);
				DrawRect(8, 352, 148, 120, olc::WHITE);
				DrawString(76, 362, "Roundhead", olc::WHITE);
				DrawString(76, 378, "Common", olc::GREEN);
				// Draw Star
				olc::vi2d vBody = { 14, 356 };
				SetPixelMode(olc::Pixel::ALPHA);
				DrawSprite(vBody, sprMush2);
				SetPixelMode(olc::Pixel::NORMAL);
			}
		}
		if (bMushSelected)
		{
			// Generate full star system
			cValley Mushroom(nSelectedMushSeed1, nSelectedMushSeed2, true);
			if (Mushroom.mushExists && !Mushroom.mushtype2 && Mushroom.mushtype3) {
				// Draw Window
				FillRect(8, 352, 160, 120, olc::DARK_BLUE);
				DrawRect(8, 352, 160, 120, olc::WHITE);
				DrawString(76, 362, "Chanterelle", olc::WHITE);
				DrawString(76, 378, "Uncommon", olc::YELLOW);
				// Draw Star
				olc::vi2d vBody = { 14, 356 };
				SetPixelMode(olc::Pixel::ALPHA);
				DrawSprite(vBody, sprMush3);
				SetPixelMode(olc::Pixel::NORMAL);
			}
		}

		
		return true;
	}
};

int main()
{
	cMushroomHeaven demo;
	if (demo.Construct(512, 480, 2, 2, false, false))
		demo.Start();
	
	return 0;
}
