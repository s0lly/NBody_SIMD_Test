/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
 /*******************************************************************************************
 *	Code and amendments by s0lly														   *
 *	https://www.youtube.com/c/s0lly							                               *
 *	https://s0lly.itch.io/																   *
 *	https://www.instagram.com/s0lly.gaming/												   *
 ********************************************************************************************/

#include "MainWindow.h"
#include "Game.h"

Game::Game( MainWindow& wnd )
	:
	wnd( wnd ),
	gfx( wnd )
{
	entities2.locX = new float[NUM_OBJECTS];
	entities2.locY = new float[NUM_OBJECTS];
	entities2.mass = new float[NUM_OBJECTS];
	entities2.velX = new float[NUM_OBJECTS];
	entities2.velY = new float[NUM_OBJECTS];

	entities3.locX = new float[NUM_OBJECTS];
	entities3.locY = new float[NUM_OBJECTS];
	entities3.mass = new float[NUM_OBJECTS];
	entities3.velX = new float[NUM_OBJECTS];
	entities3.velY = new float[NUM_OBJECTS];

	entities4.locX = new float[NUM_OBJECTS];
	entities4.locY = new float[NUM_OBJECTS];
	entities4.mass = new float[NUM_OBJECTS];
	entities4.velX = new float[NUM_OBJECTS];
	entities4.velY = new float[NUM_OBJECTS];

	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		entities2.locX[i] = (float)(rand() % 1000);
		entities2.locY[i] = (float)(rand() % 1000);
		entities2.mass[i] = (float)(rand() % 50);
		entities2.velX[i] = 0.0f;
		entities2.velY[i] = 0.0f;

		entities3.locX[i] = entities2.locX[i];
		entities3.locY[i] = entities2.locY[i];
		entities3.mass[i] = entities2.mass[i];
		entities3.velX[i] = entities2.velX[i];
		entities3.velY[i] = entities2.velY[i];

		entities4.locX[i] = entities2.locX[i];
		entities4.locY[i] = entities2.locY[i];
		entities4.mass[i] = entities2.mass[i];
		entities4.velX[i] = entities2.velX[i];
		entities4.velY[i] = entities2.velY[i];

		entity.push_back(Entity{ entities2.locX[i] , entities2.locY[i] , entities2.mass[i] , entities2.velX[i] , entities2.velY[i] });
	}

}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	dt1 = 0.0f;
	dt2 = 0.0f;
	dt3 = 0.0f;
	dt4 = 0.0f;

	for (int t = 0; t < timeStep; t++)
	{

		// Stage 1: AOS Approach
		{
			start1 = std::chrono::system_clock::now();

			for (int i = 0; i < NUM_OBJECTS; i++)
			{
				for (int j = 0; j < NUM_OBJECTS; j++)
				{
					float distX = (entity[j].locX - entity[i].locX);
					float distY = (entity[j].locY - entity[i].locY);

					float distMagSqrd = distX * distX + distY * distY;

					float distMag = sqrt(distMagSqrd);

					if (distMag > MIN_RADIUS)
					{
						float accelMag = (GRAV * entity[j].mass) / distMagSqrd;

						float vecXAdd = (accelMag * distX) / distMag;
						float vecYAdd = (accelMag * distY) / distMag;

						entity[i].velX += vecXAdd;
						entity[i].velY += vecYAdd;
					}
				}
			}

			for (int i = 0; i < NUM_OBJECTS; i++)
			{
				entity[i].locX += entity[i].velX;
				entity[i].locY += entity[i].velY;
			}

			end1 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime1 = end1 - start1;
			dt1 += elapsedTime1.count();
		}



		// Stage 2: SOA Approach
		{
			start2 = std::chrono::system_clock::now();

			for (int i = 0; i < NUM_OBJECTS; i++)
			{
				for (int j = 0; j < NUM_OBJECTS; j++)
				{
					float distX = (entities2.locX[j] - entities2.locX[i]);
					float distY = (entities2.locY[j] - entities2.locY[i]);

					float distMagSqrd = distX * distX + distY * distY;

					float distMag = sqrt(distMagSqrd);

					if (distMag > MIN_RADIUS)
					{
						float accelMag = GRAV * entities2.mass[j] / distMagSqrd;

						float vecXAdd = (accelMag * distX) / distMag;
						float vecYAdd = (accelMag * distY) / distMag;

						entities2.velX[i] += vecXAdd;
						entities2.velY[i] += vecYAdd;
					}
				}
			}

			for (int i = 0; i < NUM_OBJECTS; i++)
			{
				entities2.locX[i] += entities2.velX[i];
				entities2.locY[i] += entities2.velY[i];
			}

			end2 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime2 = end2 - start2;
			dt2 += elapsedTime2.count();
		}


		// Stage 3: SOA + SIMD Approach
		{
			start3 = std::chrono::system_clock::now();

			__m256 gravConst = _mm256_set1_ps(GRAV);

			__m256 minRadiusConst = _mm256_set1_ps(MIN_RADIUS);

			for (int i = 0; i < NUM_OBJECTS; i++)
			{
				__m256 locXi = _mm256_set1_ps(entities3.locX[i]);
				__m256 locYi = _mm256_set1_ps(entities3.locY[i]);

				for (int j = 0; j < NUM_OBJECTS - 7; j += 8)
				{
					__m256 locDistX = _mm256_sub_ps(*(__m256*)(&entities3.locX[j]), locXi);
					__m256 locDistY = _mm256_sub_ps(*(__m256*)(&entities3.locY[j]), locYi);

					__m256 distMagSqrd = _mm256_add_ps(_mm256_mul_ps(locDistX, locDistX), _mm256_mul_ps(locDistY, locDistY));

					__m256 accelMag = _mm256_div_ps(_mm256_mul_ps(gravConst, *(__m256*)(&entities3.mass[j])), distMagSqrd);

					__m256 distMag = _mm256_sqrt_ps(distMagSqrd);

					__m256 velX = _mm256_div_ps(_mm256_mul_ps(accelMag, locDistX), distMag);
					__m256 velY = _mm256_div_ps(_mm256_mul_ps(accelMag, locDistY), distMag);

					__m256 distComp = _mm256_cmp_ps(distMag, minRadiusConst, _CMP_GT_OS);

					velX = _mm256_and_ps(velX, distComp);
					velY = _mm256_and_ps(velY, distComp);

					for (int k = 0; k < 2; k++)
					{
						__m128 velXk = ((__m128*)&velX)[k];
						velXk = _mm_hadd_ps(velXk, velXk);
						velXk = _mm_hadd_ps(velXk, velXk);
						entities3.velX[i] += velXk.m128_f32[0];

						__m128 velYk = ((__m128*)&velY)[k];
						velYk = _mm_hadd_ps(velYk, velYk);
						velYk = _mm_hadd_ps(velYk, velYk);
						entities3.velY[i] += velYk.m128_f32[0];
					}
				}
			}

			for (int i = 0; i < NUM_OBJECTS; i += 8)
			{
				*(__m256*)(&entities3.locX[i]) = _mm256_add_ps(*(__m256*)(&entities3.locX[i]), *(__m256*)(&entities3.velX[i]));
				*(__m256*)(&entities3.locY[i]) = _mm256_add_ps(*(__m256*)(&entities3.locY[i]), *(__m256*)(&entities3.velY[i]));
			}

			end3 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime3 = end3 - start3;
			dt3 += elapsedTime3.count();
		}

		// Stage 4: SOA + SIMD + MT Approach
		{
			start4 = std::chrono::system_clock::now();

			__m256 gravConst = _mm256_set1_ps(GRAV);

			__m256 minRadiusConst = _mm256_set1_ps(MIN_RADIUS);

			int numThreads = 8;
			int threadSize = NUM_OBJECTS / numThreads + 1;

			auto entities4Ptr = &entities4;

			std::vector<std::thread> threadList;

			for (int k = 0; k < numThreads; k++)
			{
				threadList.push_back(std::thread([k, threadSize, entities4Ptr, gravConst, minRadiusConst]()
				{
					for (int i = k * threadSize; (i < (k + 1) * threadSize) && (i < NUM_OBJECTS); i++)
					{
						__m256 locXi = _mm256_set1_ps(entities4Ptr->locX[i]);
						__m256 locYi = _mm256_set1_ps(entities4Ptr->locY[i]);

						for (int j = 0; j < NUM_OBJECTS - 7; j += 8)
						{
							__m256 locDistX = _mm256_sub_ps(*(__m256*)(&entities4Ptr->locX[j]), locXi);
							__m256 locDistY = _mm256_sub_ps(*(__m256*)(&entities4Ptr->locY[j]), locYi);

							__m256 distMagSqrd = _mm256_add_ps(_mm256_mul_ps(locDistX, locDistX), _mm256_mul_ps(locDistY, locDistY));

							__m256 accelMag = _mm256_div_ps(_mm256_mul_ps(gravConst, *(__m256*)(&entities4Ptr->mass[j])), distMagSqrd);

							__m256 distMag = _mm256_sqrt_ps(distMagSqrd);

							__m256 velX = _mm256_div_ps(_mm256_mul_ps(accelMag, locDistX), distMag);
							__m256 velY = _mm256_div_ps(_mm256_mul_ps(accelMag, locDistY), distMag);

							__m256 distComp = _mm256_cmp_ps(distMag, minRadiusConst, _CMP_GT_OS);

							velX = _mm256_and_ps(velX, distComp);
							velY = _mm256_and_ps(velY, distComp);

							for (int k = 0; k < 2; k++)
							{
								__m128 velXk = ((__m128*)&velX)[k];
								velXk = _mm_hadd_ps(velXk, velXk);
								velXk = _mm_hadd_ps(velXk, velXk);
								entities4Ptr->velX[i] += velXk.m128_f32[0];

								__m128 velYk = ((__m128*)&velY)[k];
								velYk = _mm_hadd_ps(velYk, velYk);
								velYk = _mm_hadd_ps(velYk, velYk);
								entities4Ptr->velY[i] += velYk.m128_f32[0];
							}
						}
					}
				}));
			}
			std::for_each(threadList.begin(), threadList.end(), std::mem_fn(&std::thread::join));

			for (int i = 0; i < NUM_OBJECTS; i += 8)
			{
				*(__m256*)(&entities4.locX[i]) = _mm256_add_ps(*(__m256*)(&entities4.locX[i]), *(__m256*)(&entities4.velX[i]));
				*(__m256*)(&entities4.locY[i]) = _mm256_add_ps(*(__m256*)(&entities4.locY[i]), *(__m256*)(&entities4.velY[i]));
			}

			end4 = std::chrono::system_clock::now();
			std::chrono::duration<float> elapsedTime4 = end4 - start4;
			dt4 += elapsedTime4.count();
		}
	}

	dt1 /= (float)timeStep;
	dt2 /= (float)timeStep;
	dt3 /= (float)timeStep;
	dt4 /= (float)timeStep;

}

void Game::ComposeFrame()
{
	for (int i = 0; i < NUM_OBJECTS; i++)
	{
		// Doesn't work yet!!!
		//gfx.DrawCircleSIMD(entities.locX[i], entities.locY[i], entities.mass[i] / 4.0f, Colors::Red);
	}

	RetroContent::DrawString(gfx, std::string("N BODY TEST"), 800.0f, 50.0f, 6, Colors::Red);

	RetroContent::DrawString(gfx, std::string("BRUTE FORCE APPROACH"), 800.0f, 150.0f, 3, Colors::Red);

	RetroContent::DrawString(gfx, std::string("NUM OBJECTS ") + std::to_string(NUM_OBJECTS), 800.0f, 300.0f, 4, Colors::Yellow);

	RetroContent::DrawString(gfx, std::string("CALC TIME IN MILLISECS"), 800.0f, 400.0f, 4, Colors::Magenta);

	RetroContent::DrawString(gfx, std::string("AOS APPROACH"), 600.0f, 500.0f, 3, Colors::Red);
	RetroContent::DrawString(gfx, std::string("SOA APPROACH"), 600.0f, 600.0f, 3, Colors::Green);
	RetroContent::DrawString(gfx, std::string("SOA AND SIMD"), 600.0f, 700.0f, 3, Colors::Cyan);
	RetroContent::DrawString(gfx, std::string("ABOVE AND MT"), 600.0f, 800.0f, 3, Colors::Yellow);

	RetroContent::DrawString(gfx, std::to_string(int(dt1*1000)), 800.0f, 500.0f, 3, Colors::Red);
	RetroContent::DrawString(gfx, std::to_string(int(dt2*1000)), 800.0f, 600.0f, 3, Colors::Green);
	RetroContent::DrawString(gfx, std::to_string(int(dt3*1000)), 800.0f, 700.0f, 3, Colors::Cyan);
	RetroContent::DrawString(gfx, std::to_string(int(dt4*1000)), 800.0f, 800.0f, 3, Colors::Yellow);

	RetroContent::DrawString(gfx, std::to_string(int((dt2 / dt1) * 100)), 950.0f, 600.0f, 3, Colors::Green);
	RetroContent::DrawString(gfx, std::to_string(int((dt3 / dt1) * 100)), 950.0f, 700.0f, 3, Colors::Cyan);
	RetroContent::DrawString(gfx, std::to_string(int((dt4 / dt1) * 100)), 950.0f, 800.0f, 3, Colors::Yellow);

	RetroContent::DrawString(gfx, "PCNT OF AOS", 1150.0f, 600.0f, 3, Colors::Green);
	RetroContent::DrawString(gfx, "PCNT OF AOS", 1150.0f, 700.0f, 3, Colors::Cyan);
	RetroContent::DrawString(gfx, "PCNT OF AOS", 1150.0f, 800.0f, 3, Colors::Yellow);

	RetroContent::DrawString(gfx, std::to_string(int(entity[0].velX*100.0f)), 1400.0f, 500.0f, 3, Colors::Red);
	RetroContent::DrawString(gfx, std::to_string(int(entities2.velX[0]*100.0f)), 1400.0f, 600.0f, 3, Colors::Green);
	RetroContent::DrawString(gfx, std::to_string(int(entities3.velX[0]*100.0f)), 1400.0f, 700.0f, 3, Colors::Cyan);
	RetroContent::DrawString(gfx, std::to_string(int(entities4.velX[0]*100.0f)), 1400.0f, 800.0f, 3, Colors::Yellow);

	RetroContent::DrawString(gfx, std::to_string(int(entity[0].velY*100.0f)), 1500.0f, 500.0f, 3, Colors::Red);
	RetroContent::DrawString(gfx, std::to_string(int(entities2.velY[0]*100.0f)), 1500.0f, 600.0f, 3, Colors::Green);
	RetroContent::DrawString(gfx, std::to_string(int(entities3.velY[0]*100.0f)), 1500.0f, 700.0f, 3, Colors::Cyan);
	RetroContent::DrawString(gfx, std::to_string(int(entities4.velY[0]*100.0f)), 1500.0f, 800.0f, 3, Colors::Yellow);

	

}
