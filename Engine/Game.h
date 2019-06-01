/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.h																				  *
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

#pragma once

#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Entities.h"
#include "RetroContent.h"
#include <vector>
#include <thread>
#include <chrono>

class Game
{
public:
	Game( class MainWindow& wnd );
	Game( const Game& ) = delete;
	Game& operator=( const Game& ) = delete;
	void Go();
private:
	void ComposeFrame();
	void UpdateModel();
	/********************************/
	/*  User Functions              */
	/********************************/
private:
	MainWindow& wnd;
	Graphics gfx;
	/********************************/
	/*  User Variables              */
	/********************************/

	std::vector<Entity> entity;
	Entities entities2;
	Entities entities3;
	Entities entities4;

	std::chrono::system_clock::time_point start1;
	std::chrono::system_clock::time_point end1;
	float dt1 = 0.0f;

	std::chrono::system_clock::time_point start2;
	std::chrono::system_clock::time_point end2;
	float dt2 = 0.0f;

	std::chrono::system_clock::time_point start3;
	std::chrono::system_clock::time_point end3;
	float dt3 = 0.0f;

	std::chrono::system_clock::time_point start4;
	std::chrono::system_clock::time_point end4;
	float dt4 = 0.0f;

	int timeStep = 4;

};