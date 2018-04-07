#pragma once

#include <float.h>
#include "Hitable.h"
#include "Vec3.h"
#include "AABB.h"


class Translate : public Hitable {
public:
	Translate(Hitable* p, const Vec3& disp) : ptr(p), offset(disp) {
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const {
		Ray moved(r.origin - offset, r.direction, r.time);
		if (ptr->hit(moved, t_min, t_max, hitInfo)) {
			hitInfo.point += offset;
			return true;
		}
		return false;
	}

	virtual bool boundingBox(double t0, double t1, AABB& box) const {
		if (ptr->boundingBox(t0, t1, box)) {
			box = AABB(box.min + offset, box.max + offset);
			return true;
		}
		return false;
	}

	Hitable* ptr;
	Vec3 offset;
};

class RotateY : public Hitable {
public:
	RotateY(Hitable* p, double angle);

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& box) const {
		box = bbox;
		return hasBox;
	}

	Hitable* ptr;
	double sinTheta;
	double cosTheta;
	bool hasBox;
	AABB bbox;
};

RotateY::RotateY(Hitable* p, double angle) {
	ptr = p;
	double rad = (M_PI / 180.0) * angle;
	sinTheta = sin(rad);
	cosTheta = cos(rad);
	hasBox = ptr->boundingBox(0, 1, bbox);
	Vec3 mmin(DBL_MAX, DBL_MAX, DBL_MAX);
	Vec3 mmax(-DBL_MAX, -DBL_MAX, -DBL_MAX);

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int k = 0; k < 2; k++) {
				double x = i * bbox.max.x + (1 - i)*bbox.min.x;
				double y = j * bbox.max.y + (1 - j)*bbox.min.y;
				double z = k * bbox.max.z + (1 - k)*bbox.min.z;
				double newX = cosTheta * x + sinTheta * z; // TODO shouldn't this be -sinTheta?
				double newZ = -sinTheta * x + cosTheta * z;
				Vec3 test(newX, y, newZ);
				if (test.x > mmax.x) mmax.x = test.x;
				if (test.x < mmin.x) mmin.x = test.x;
				if (test.y > mmax.y) mmax.y = test.y;
				if (test.y < mmin.y) mmin.y = test.y;
				if (test.z > mmax.z) mmax.z = test.z;
				if (test.z < mmin.z) mmin.z = test.z;

			}
		}
	}
	bbox = AABB(mmin, mmax);
}

bool RotateY::hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const {
	Vec3 origin = r.origin;
	Vec3 direction = r.direction;

	origin.x = cosTheta * r.origin.x - sinTheta * r.origin.z;
	origin.z = sinTheta * r.origin.x + cosTheta * r.origin.z;
	direction.x = cosTheta * r.direction.x - sinTheta * r.direction.z;
	direction.z = sinTheta * r.direction.x + cosTheta * r.direction.z;

	Ray rotated(origin, direction, r.time);
	if (ptr->hit(rotated, t_min, t_max, HitInfo)) {
		Vec3 p = HitInfo.point;
		Vec3 n = HitInfo.normal;
		p.x = cosTheta * HitInfo.point.x + sinTheta * HitInfo.point.z;
		p.z = -sinTheta * HitInfo.point.x + cosTheta * HitInfo.point.z;
		n.x = cosTheta * HitInfo.normal.x + sinTheta * HitInfo.normal.z;
		n.z = -sinTheta * HitInfo.normal.x + cosTheta * HitInfo.normal.z;
		HitInfo.point = p;
		HitInfo.normal = n;
		return true;
	}
	else {
		return false;
	}
}
