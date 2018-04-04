#pragma once

#include "Hitable.h"
#include "Material.h"

class MovingSphere : public Hitable {
public:
	MovingSphere() {
	}

	MovingSphere(Vec3 cen0, Vec3 cen1, double t0, double t1, double rad, Material* mat) : radius(rad) {
		material = mat;
		positionStart = cen0;
		positionEnd = cen1;
		timeStart = t0;
		timeEnd = t1;
	}

	Vec3 getPosition(double t) const;
	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const;

	double radius;
	double timeStart, timeEnd;
	Vec3 positionStart,positionEnd;
};

Vec3 MovingSphere::getPosition(double t) const {
	return positionStart + ((t - timeStart) / (timeEnd - timeStart))*(positionEnd - positionStart);
}

bool MovingSphere::hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const {
	Vec3 center = getPosition(r.time);
	Vec3 oc = r.origin - center;
	double a = dot(r.direction, r.direction);
	double b = dot(oc, r.direction);
	double c = dot(oc, oc) - radius * radius;
	double discriminant = b * b - a * c;
	if (discriminant > 0.0) {		
		double temp = (-b - sqrt(discriminant)) / a;
		if (temp > t_min && temp < t_max) {
			hitInfo.dist = temp;
			hitInfo.point = r.pointAt(hitInfo.dist);
			hitInfo.normal = (hitInfo.point - getPosition(r.time)) / radius;
			hitInfo.pMat = material;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp > t_min && temp < t_max) {
			hitInfo.dist = temp;
			hitInfo.point = r.pointAt(hitInfo.dist);
			hitInfo.normal = (hitInfo.point - getPosition(r.time)) / radius;
			hitInfo.pMat = material;
			return true;
		}
	}
	return false;
}

bool MovingSphere::boundingBox(double t0, double t1, AABB& Box) const {
	AABB Box0, Box1;
	
	Box0 = AABB(positionStart - Vec3(radius, radius, radius),
				positionStart + Vec3(radius, radius, radius));
	Box1 = AABB(positionEnd - Vec3(radius, radius, radius),
				positionEnd + Vec3(radius, radius, radius));

	Box = combineAABB(Box0, Box1);

	return true;
}
