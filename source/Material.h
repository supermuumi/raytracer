#pragma once

#include "Vec3.h"
#include "Hitable.h"
#include "Texture.h"

class Ray;

class Material {
public:
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;

	Vec3 albedo;
};

class Lambertian : public Material {
public:
	Lambertian(Texture* alb) {
		albedo = alb;
	}

	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.point + rec.normal + getRandomInUnitSphere();
		scattered = Ray(rec.point, target - rec.point, rayIn.time);
		attenuation = albedo->value(0, 0, rec.point);
		return true;
	}

	Texture* albedo;
};

class Metal : public Material {
public:
	Metal(Vec3 alb) {
		albedo = alb;
	}
	
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 reflected = reflect(unitVector(rayIn.direction), rec.normal);
		scattered = Ray(rec.point, reflected, rayIn.time);
		attenuation = albedo;
		return (dot(scattered.direction, rec.normal) > 0.0);
	}
};

class Dielectric : public Material {
public:
	Dielectric(double ri) : refIdx(ri) {
	}

	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 outwardNormal;
		Vec3 reflected = reflect(rayIn.direction, rec.normal);
		double ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 1.0); // VEC3_ONE;
		Vec3 refracted;
		double reflectProb;
		double cosine;

		if (dot(rayIn.direction, rec.normal) > 0.0) {
			outwardNormal = -rec.normal;
			ni_over_nt = refIdx;
			cosine = refIdx * dot(rayIn.direction, rec.normal) / rayIn.direction.length();
		}
		else {
			outwardNormal = rec.normal;
			ni_over_nt = 1.0 / refIdx;
			cosine = -dot(rayIn.direction, rec.normal) / rayIn.direction.length();
		}
		if (refract(rayIn.direction, outwardNormal, ni_over_nt, refracted)) {
			reflectProb = schlick(cosine, refIdx);
		}
		else {
			reflectProb = 1.0;
		}
		if (drand() < reflectProb) {
			scattered = Ray(rec.point, reflected, rayIn.time);
		}
		else {
			scattered = Ray(rec.point, refracted, rayIn.time);
		}

		return true;
	}

	double refIdx;
};