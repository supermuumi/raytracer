#pragma once

#include "Vec3.h"
#include "Ray.h"

inline double ffmin(double a, double b) {
	return a < b ? a : b;
}

inline double ffmax(double a, double b) {
	return a > b ? a : b;
}

class AABB {

public:
	AABB() {
	}
	AABB(const Vec3& a, const Vec3& b) {
		min = a;
		max = b;
	}

	inline bool hit(const Ray& r, double tMin, double tMax) const {
		double invD, t0, t1;

		invD = 1.0 / r.direction.x;
		t0 = (min.x - r.origin.x) * invD;
		t1 = (max.x - r.origin.x) * invD;
		if (invD < 0.0) {
			double temp = t0;
			t0 = t1;
			t1 = temp;
		}
		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;
		if (tMax <= tMin) {
			return false;
		}

		invD = 1.0 / r.direction.y;
		t0 = (min.y - r.origin.y) * invD;
		t1 = (max.y - r.origin.y) * invD;
		if (invD < 0.0) {
			double temp = t0;
			t0 = t1;
			t1 = temp;
		}
		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;
		if (tMax <= tMin) {
			return false;
		}

		invD = 1.0 / r.direction.z;
		t0 = (min.z - r.origin.z) * invD;
		t1 = (max.z - r.origin.z) * invD;
		if (invD < 0.0) {
			double temp = t0;
			t0 = t1;
			t1 = temp;
		}
		tMin = t0 > tMin ? t0 : tMin;
		tMax = t1 < tMax ? t1 : tMax;
		if (tMax <= tMin) {
			return false;
		}

		return true;
	}

	Vec3 min;
	Vec3 max;
};

AABB combineAABB(AABB a, AABB b) {
	Vec3 smallBox(fmin(a.min.x, b.min.x),
				  fmin(a.min.y, b.min.y),
				  fmin(a.min.z, b.min.z));
	Vec3 largeBox(fmax(a.max.x, b.max.x),
				  fmax(a.max.y, b.max.y),
				  fmax(a.max.z, b.max.z));

	return AABB(smallBox, largeBox);
}