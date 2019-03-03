#pragma once
//#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <string>
//#include <limits>
using namespace sf;

#define WinX 1024
#define WinY 1024
#define spriteSize 42
//#define pelletSize 8
#define pelletSize 26
#define powerPelletSize 26

#ifdef __APPLE__
#include "ResourcePath.hpp"
#endif

#ifndef __APPLE__
std::string resourcePath();
#endif

static const float WinLength = WinX, WinHeight = WinY;

void ResizeView(const RenderWindow &window, View& view);

class Audio : public Sound {
    SoundBuffer buffer;
public:
    Audio(std::string filename);
};
