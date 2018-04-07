#pragma once

#include "Hitable.h"
#include "Material.h"
#include "AABB.h"

class XYRect : public Hitable {
public:
	XYRect() {
	}

	XYRect(double _x0, double _x1, double _y0, double _y1, double _k, Material* mat) {
		x0 = _x0;
		x1 = _x1;
		y0 = _y0;
		y1 = _y1;
		k = _k;
		pMat = mat;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const {
		Box = AABB(Vec3(x0, y0, k - 0.0001), Vec3(x1, y1, k + 0.0001));
		return true;
	}

	double x0, x1, y0, y1;
	double k;
	Material *pMat;
};

class XZRect : public Hitable {
public:
	XZRect() {
	}

	XZRect(double _x0, double _x1, double _z0, double _z1, double _k, Material* mat) {
		x0 = _x0;
		x1 = _x1;
		z0 = _z0;
		z1 = _z1;
		k = _k;
		pMat = mat;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const {
		Box = AABB(Vec3(x0, k - 0.0001, z0), Vec3(x1, k + 0.0001, z1));
		return true;
	}

	double x0, x1, z0, z1;
	double k;
	Material *pMat;
};

class YZRect : public Hitable {
public:
	YZRect() {
	}

	YZRect(double _y0, double _y1, double _z0, double _z1, double _k, Material* mat) {
		y0 = _y0;
		y1 = _y1;
		z0 = _z0;
		z1 = _z1;
		k = _k;
		pMat = mat;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const {
		Box = AABB(Vec3(k - 0.0001, y0, z0), Vec3(k + 0.0001, y1, z1));
		return true;
	}

	double y0, y1, z0, z1;
	double k;
	Material *pMat;
};

bool XYRect::hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const {
	double t = (k - r.origin.z) / r.direction.z;
	if (t < t_min || t > t_max) {
		return false;
	}
	double x = r.origin.x + t * r.direction.x;
	double y = r.origin.y + t * r.direction.y;
	if (x < x0 || x > x1 || y < y0 || y > y1) {
		return false;
	}
	HitInfo.u = (x - x0) / (x1 - x0);
	HitInfo.v = (y - y0) / (y1 - y0);
	HitInfo.dist = t;
	HitInfo.pMat = pMat;
	HitInfo.point = r.pointAt(t);
	HitInfo.normal = Vec3(0.0, 0.0, 1.0);
	
	return true;
}

bool XZRect::hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const {
	double t = (k - r.origin.y) / r.direction.y;
	if (t < t_min || t > t_max) {
		return false;
	}
	double x = r.origin.x + t * r.direction.x;
	double z = r.origin.z + t * r.direction.z;
	if (x < x0 || x > x1 || z < z0 || z > z1) {
		return false;
	}
	hitInfo.u = (x - x0) / (x1 - x0);
	hitInfo.v = (z - z0) / (z1 - z0);
	hitInfo.dist = t;
	hitInfo.pMat = pMat;
	hitInfo.point = r.pointAt(t);
	hitInfo.normal = Vec3(0.0, 1.0, 0.0);

	return true;
}

bool YZRect::hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const {
	double t = (k - r.origin.x) / r.direction.x;
	if (t < t_min || t > t_max) {
		return false;
	}
	double y = r.origin.y + t * r.direction.y;
	double z = r.origin.z + t * r.direction.z;
	if (y < y0 || y > y1 || z < z0 || z > z1) {
		return false;
	}
	hitInfo.u = (y - y0) / (y1 - y0);
	hitInfo.v = (z - z0) / (z1 - z0);
	hitInfo.dist = t;
	hitInfo.pMat = pMat;
	hitInfo.point = r.pointAt(t);
	hitInfo.normal = Vec3(1.0, 0.0, 0.0);

	return true;
}