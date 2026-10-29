#ifndef CS3113_H
#define CS3113_H

#include "raylib.h"
#include "raymath.h"

#include <cmath>
#include <iostream>
#include <map>
#include <vector>

#define LOG(argument) std::cout << argument << '\n'

enum AppStatus { TERMINATED, RUNNING };
enum TextureType { SINGLE, ATLAS };

Color ColorFromHex(const char *hex);
float GetLength(Vector2 vector);
void Normalise(Vector2 *vector);
Rectangle getUVRectangle(const Texture2D *texture, int index, int rows, int cols);

#endif // CS3113_H
