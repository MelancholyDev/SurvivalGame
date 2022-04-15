﻿#pragma once
#include "Point.h"
#include "vector"
#include <iostream>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;
class BezierComputations
{
public:
	static float BezierLut(float X1,float Y1,float X2,float Y2,float A,float X);
	static Point Bezier(float X1, float Y1, float X2, float Y2, float A, float T);
	static float FilterMap(float HeightMap,float SmoothMap,float X1,float Y1,float X2,float Y3,float A,float B);
};


int nearestNeighbourIndex(std::vector<float> &x, float &value);
std::vector<float> interp1(std::vector<float> &x, std::vector<float> &y, std::vector<float> &x_new);
std::vector<float> linspace(float start_in, float end_in, int num_in);
// int main() {
//     vector<float> x{1, 2, 3, 4, 5};
//     vector<float> y{5, 6, 7, 8, 9};
//     vector<float> newx{0, 5, 6.123, 12.123, 2, 4};
//
//     auto res = interp1(x, y, newx);
//     for (auto i: res)
//         cout << i << " ";
//     cout << endl;
// }