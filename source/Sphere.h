#pragma once

#include "Hitable.h"
#include "Material.h"
#include "AABB.h"

class Sphere : public Hitable {
public:
	Sphere() {
	}

	Sphere(Vec3 cen, double rad, Material* mat) : center(cen), radius(rad) {
		material = mat;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const;

	double radius;
	Vec3 center;
};

bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	Vec3 oc = r.origin - center;
	double a = dot(r.direction, r.direction);
	double b = dot(oc, r.direction);
	double c = dot(oc, oc) - radius*radius;
	double discriminant = b*b - a*c;

	if (discriminant > 0.0) {
		double temp = (-b - sqrt(b*b - a*c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.dist = temp;
			rec.point = r.pointAt(rec.dist);
			rec.normal = (rec.point - center) / radius;
			rec.pMat = material;
			getSphereUV((rec.point - center) / radius, rec.u, rec.v);
			return true;
		}
		temp = (-b + sqrt(b*b - a*c)) / a;
		if (temp > t_min && temp < t_max) {
			rec.dist = temp;
			rec.point = r.pointAt(rec.dist);
			rec.normal = (rec.point - center) / radius;
			rec.pMat = material;
			getSphereUV((rec.point - center) / radius, rec.u, rec.v);
			return true;
		}
	}
	return false;
}

bool Sphere::boundingBox(double t0, double t1, AABB& Box) const {
	Box = AABB(center - Vec3(radius, radius, radius),
			   center + Vec3(radius, radius, radius));

	return true;
}