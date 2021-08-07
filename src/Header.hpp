//
//  Header.hpp
//  Cities Box
//
//  Created by YotioSoft on 2019/11/27.
//
#pragma once

#include <Siv3D.hpp> // OpenSiv3D v0.4.3
#if defined(_WIN32) || defined(_WIN64)
	#include <Siv3D/Windows.hpp>
#endif

#include <stdio.h>
#include <fstream>
#include <algorithm>
#include <random>

#include "Enums.hpp"
#include "Structs.hpp"

#define TITLE			U"Albus Box"
#define VERSION			U"0.1"
#define RELEASE_NUM		1

using namespace std;

class CityMap;
class Addon;
class ImagesStruct;
