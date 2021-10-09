/* Goal: to spawn procedurally generated mushroom sprites */
#define OLC_PGE_APPLICATION
#include "stb_image.h"
#include "olcPixelGameEngine.h"
#include "olcPGEX_Sound.h"
#include <cmath>
#include <fstream>
#include <Windows.h>
#include <random>
#define OLC_PGEX_SOUND_H
#define _USE_MATH_DEFINES
using namespace std;

class cValley
{

public:
	cValley(uint32_t x, uint32_t y, bool bGenerateFullSystem = false)
	{
		// Set seed based on location of shrooms
		nProcGen = (x & 0xFFFF) << 16 | (y & 0xFFFF);

		// Not all locations contain a cluster of shrooms
		
		mushExists = (rndInt(0, 70) == 1);
		mushtype2 = (rndInt(0, 2) == 1);
		mushtype3 = (rndInt(0, 6) == 1);

		if (!mushExists) return;
	}

	~cValley(){}	

public:
	
	bool        mushtype3 = false;
	bool        mushtype2 = false;
	bool		mushExists = false;
	
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
		
		sprMush = new olc::Sprite("../../res/mushgen.png");
		sprMush2 = new olc::Sprite("../../res/gen.png");
		sprMush3 = new olc::Sprite("../../res/gen2.png");

		return true;
	}

	olc::vf2d vMushroomHeavenOffset = { 0,0 };
	bool bMushSelected = false;
	uint32_t nSelectedMushSeed1 = 0;
	uint32_t nSelectedMushSeed2 = 0;

	bool OnUserUpdate(float fElapsedTime) override
	{
		
		if (fElapsedTime <= 0.0001f) return true;
		Clear(olc::DARK_GREEN);
		
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

					SetPixelMode(olc::Pixel::ALPHA);
					DrawSprite({ screen_sector.x * 16 - 8, screen_sector.y * 16 - 8 }, sprMush);
					SetPixelMode(olc::Pixel::NORMAL);

					// Highlight on mushroom
					if (mouse.x == screen_sector.x && mouse.y == screen_sector.y)
					{
						DrawRect(screen_sector.x * 16, screen_sector.y * 16 - 8, 44, 60, olc::YELLOW);
					}

				}

				else if (Mushroom.mushExists && Mushroom.mushtype2 && !Mushroom.mushtype3) {
					SetPixelMode(olc::Pixel::ALPHA);
					DrawSprite({ screen_sector.x * 16 - 8, screen_sector.y * 16 - 8 }, sprMush2);

					SetPixelMode(olc::Pixel::NORMAL);

					// Highlight on mushroom
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

		// Draw info window
		if (bMushSelected)
		{
			// Generate full mushroom valley
			cValley Mushroom(nSelectedMushSeed1, nSelectedMushSeed2, true);

			// Draw Window
			if (Mushroom.mushExists && !Mushroom.mushtype2 && !Mushroom.mushtype3) {
				FillRect(8, 352, 140, 120, olc::DARK_BLUE);
				DrawRect(8, 352, 140, 120, olc::WHITE);
				DrawString(76, 362, "Bolete", olc::WHITE);
				DrawString(76, 378, "Common", olc::GREEN);
				// Draw Mushroom
				olc::vi2d vBody = { 14, 356 };
				SetPixelMode(olc::Pixel::ALPHA);
				DrawSprite(vBody, sprMush);
				SetPixelMode(olc::Pixel::NORMAL);

			};

		}
		if (bMushSelected)
		{
			// Generate full mushroom valley
			cValley Mushroom(nSelectedMushSeed1, nSelectedMushSeed2, true);
			if (Mushroom.mushExists && Mushroom.mushtype2 && !Mushroom.mushtype3) {
				// Draw Window
				FillRect(8, 352, 148, 120, olc::DARK_BLUE);
				DrawRect(8, 352, 148, 120, olc::WHITE);
				DrawString(76, 362, "Roundhead", olc::WHITE);
				DrawString(76, 378, "Common", olc::GREEN);
				// Draw Mushroom
				olc::vi2d vBody = { 14, 356 };
				SetPixelMode(olc::Pixel::ALPHA);
				DrawSprite(vBody, sprMush2);
				SetPixelMode(olc::Pixel::NORMAL);
			}
		}
		if (bMushSelected)
		{
			// Generate full mushroom valley
			cValley Mushroom(nSelectedMushSeed1, nSelectedMushSeed2, true);
			if (Mushroom.mushExists && !Mushroom.mushtype2 && Mushroom.mushtype3) {
				// Draw Window
				FillRect(8, 352, 160, 120, olc::DARK_BLUE);
				DrawRect(8, 352, 160, 120, olc::WHITE);
				DrawString(76, 362, "Chanterelle", olc::WHITE);
				DrawString(76, 378, "Uncommon", olc::YELLOW);
				// Draw Mushroom
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
	if (demo.Construct(512, 480, 2, 2, true, false))
		demo.Start();
	return 0;
}
