#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include "common.h"

#define VEC3_ZERO Vec3(0.0, 0.0, 0.0)
#define VEC3_ONE  Vec3(1.0, 1.0, 1.0)
#define VEC3_UP   Vec3(0.0, 1.0, 0.0)

class Vec3 {
public:
	Vec3() {
		x = y = z = 0.0;
	}
	Vec3(double _x, double _y, double _z) {
		x = _x;
		y = _y;
		z = _z;
	}

	inline const Vec3& operator+() const {
		return *this;
	}
	inline Vec3 operator-() const {
		return Vec3(-x, -y, -z);
	}
	inline Vec3& operator+=(const Vec3& v2);
	inline Vec3& operator-=(const Vec3& v2);
	inline Vec3& operator*=(const Vec3& v2);
	inline Vec3& operator/=(const Vec3& v2);
	inline Vec3& operator*=(const double t);
	inline Vec3& operator/=(const double t);

	inline double length() const {
		return sqrt(x * x + y * y + z * z);
	}
	inline double sqLength() const {
		return x * x + y * y + z * z;
	}
	inline void normalize();

	double x;
	double y;
	double z;
};

inline Vec3 operator+(const Vec3 &v1, const Vec3& v2) {
	return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3& v2) {
	return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3& v2) {
	return Vec3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3& v2) {
	return Vec3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
}

inline Vec3 operator*(double t, const Vec3& v) {
	return Vec3(v.x*t, v.y*t, v.z*t);
}

inline Vec3 operator*(const Vec3& v, double t) {
	return Vec3(v.x*t, v.y*t, v.z*t);
}

inline Vec3 operator/(const Vec3& v, double t) {
	return Vec3(v.x / t, v.y / t, v.z / t);
}

inline double dot(const Vec3& v1, const Vec3& v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

inline Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(
		  v1.y*v2.z - v1.z*v2.y,
		-(v1.x*v2.z - v1.z*v2.x),
  		  v1.x*v2.y - v1.y*v2.x
	);
}

inline Vec3& Vec3::operator+=(const Vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3& v) {
	x *= v.x;
	y *= v.y;
	z *= v.z;
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3& v) {
	x /= v.x;
	y /= v.y;
	z /= v.z;
	return *this;
}

inline Vec3& Vec3::operator*=(const double t) {
	x *= t;
	y *= t;
	z *= t;
	return *this;
}

inline Vec3& Vec3::operator/=(const double t) {
	double k = 1.0 / t;
	x *= k;
	y *= k;
	z *= k;
	return *this;
}


void Vec3::normalize() {
	double k = 1.0 / length();
	x *= k;
	y *= k;
	z *= k;
}

inline Vec3 unitVector(Vec3 v) {
	return v / v.length();
}

Vec3 getRandomInUnitSphere() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(drand(), drand(), drand()) - Vec3(1.0, 1.0, 1.0);
	} while (p.sqLength() >= 1.0);
	return p;
}

Vec3 getRandomInUnitDisc() {
	Vec3 p;
	do {
		p = 2.0 * Vec3(drand(), drand(), 0.0) - Vec3(1.0, 1.0, 0.0);
	} while (dot(p, p) >= 1.0);
	return p;

}

Vec3 reflect(const Vec3& v, const Vec3& n) {
	return v - 2 * dot(v, n) * n;
}

bool refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted) {
	Vec3 uv = unitVector(v);
	double dt = dot(uv, n);
	double discriminant = 1.0 - ni_over_nt * ni_over_nt*(1 - dt * dt);
	if (discriminant > 0.0) {
		refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
		return true;
	}
	return false;
}

double schlick(double cosine, double refIdx) {
	double r0 = (1.0 - refIdx) / (1.0 + refIdx);
	r0 = r0 * r0;
	return r0 + (1 - r0)*pow((1.0 - cosine), 5);
}

void getSphereUV(const Vec3& p, double& u, double& v) {
	double phi = atan2(p.z, p.x);
	double theta = asin(p.y);
	u = 1 - (phi + M_PI) - (2 * M_PI);
	v = (theta + M_PI / 2.0) / M_PI;
}