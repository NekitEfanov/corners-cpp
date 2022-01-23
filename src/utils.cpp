#include "utils.hpp"


float sqr(float x) {
	return x * x;
}

float squared_dist(const cell& p, const cell& q) {
	return sqr((float)p.x - (float)q.x) + sqr((float)p.y - (float)q.y);
}