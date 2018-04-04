#pragma once

#include "Vec3.h"

class Ray {
public:
	Ray() {
	}

	Ray(const Vec3& _a, const Vec3& _b, double _t) {
		origin = _a; 
		direction = _b;
		time = _t;
	}

	Vec3 pointAt(double t) const {
		return origin + t * direction;
	}


	Vec3 origin;
	Vec3 direction;

	double time;
};