#pragma once

#include "Ray.h"

class Material;
class AABB;

struct HitRecord {
	double dist;
	Vec3 point;
	Vec3 normal;
	Material* pMat;
	double u, v;
};

class Hitable {
public:
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const = 0;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const = 0;

	Material* material;
};

class FlipNormals : public Hitable {
public: 
	FlipNormals(Hitable* p) : ptr(p) {
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const {
		if (ptr->hit(r, t_min, t_max, HitInfo)) {
			HitInfo.normal = -HitInfo.normal;
			return true;
		}
		else {
			return false;
		}
	}

	virtual bool boundingBox(double t0, double t1, AABB& Box) const {
		return ptr->boundingBox(t0, t1, Box);
	}

	Hitable* ptr;
};