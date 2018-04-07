#pragma once

#include "Vec3.h"
#include "Hitable.h"
#include "Texture.h"

class Ray;

class Material {
public:
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const = 0;
	virtual Vec3 emitted(double u, double v, const Vec3& p) const {
		return Vec3(0.0, 0.0, 0.0);
	}
};

class Lambertian : public Material {
public:
	Lambertian(Texture* alb) : albedo(alb) {}
		
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 target = rec.point + rec.normal + getRandomInUnitSphere();
		scattered = Ray(rec.point, target - rec.point, rayIn.time);
		attenuation = albedo->value(rec.u, rec.v, rec.point);
		return true;
	}

	Texture* albedo;
};

class Metal : public Material {
public:
	Metal(Texture* alb) : albedo(alb) {
	}
	
	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 reflected = reflect(unitVector(rayIn.direction), rec.normal);
		scattered = Ray(rec.point, reflected, rayIn.time); // +fuzziness*random_in_unit_sphere());
		attenuation = albedo->value(rec.u, rec.v, rec.point);
		return (dot(scattered.direction, rec.normal) > 0.0);
	}

	Texture* albedo;
	double fuzziness;
};

class Dielectric : public Material {
public:
	Dielectric(double ri) : ref_idx(ri) {}

	virtual bool scatter(const Ray& r_in, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		Vec3 outward_normal;
		Vec3 reflected = reflect(r_in.direction, rec.normal);
		double ni_over_nt;
		attenuation = Vec3(1.0, 1.0, 1.0);
		Vec3 refracted;
		double reflect_prob;
		double cosine;

		if (dot(r_in.direction, rec.normal) > 0.0) {
			outward_normal = -rec.normal;
			ni_over_nt = ref_idx;
			cosine = ref_idx * dot(r_in.direction, rec.normal) / r_in.direction.length();
		}
		else {
			outward_normal = rec.normal;
			ni_over_nt = 1.0 / ref_idx;
			cosine = -dot(r_in.direction, rec.normal) / r_in.direction.length();
		}
		if (refract(r_in.direction, outward_normal, ni_over_nt, refracted)) {
			reflect_prob = schlick(cosine, ref_idx);
		}
		else {
			reflect_prob = 1.0;
		}
		if (drand() < reflect_prob) {
			scattered = Ray(rec.point, reflected, r_in.time);
		}
		else {
			scattered = Ray(rec.point, refracted, r_in.time);
		}
		return true;
	}

	double ref_idx;
};

class DiffuseLight : public Material {
public:
	DiffuseLight(Texture* a) : emit(a) { }

	virtual bool scatter(const Ray& rayIn, const HitRecord& rec, Vec3& attenuation, Ray& scattered) const {
		return false;
	}
	virtual Vec3 emitted(double u, double v, const Vec3& p) const {
		return emit->value(u, v, p);
	}

	Texture* emit;
};