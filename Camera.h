#pragma once

#include "Ray.h"
#define _USE_MATH_DEFINES
#include <math.h>

class Camera {
public:
	Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 up, 
		   double vFov, double aspect, 
		   double aperture, double focalDist,
		   double t0, double t1) {
		lensRadius = aperture / 2.0;

		double theta = vFov * M_PI / 180.0;
		double halfHeight = tan(theta / 2.0);
		double halfWidth = halfHeight * aspect;
		origin = lookFrom;
		w = unitVector(lookFrom - lookAt);
		u = unitVector(cross(up, w));
		v = cross(w, u);

		lowerLeftCorner = origin - halfWidth * focalDist*u - halfHeight * focalDist*v - focalDist * w;
		horizontal = 2.0 * halfWidth * focalDist*u;
		vertical = 2.0 * halfHeight * focalDist*v;

		tShutterOpen = t0;
		tShutterClose = t1;
	}

	Ray getRay(double s, double t) {
		Vec3 rd = lensRadius * getRandomInUnitDisc();
		Vec3 ofs = u * rd.x + v * rd.y;
		double time = tShutterOpen + drand()*(tShutterClose - tShutterOpen);
		return Ray(origin + ofs, 
				   lowerLeftCorner + s * horizontal + t * vertical - origin - ofs,
				   time);
	}

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	double lensRadius;
	double tShutterOpen, tShutterClose;
};