#pragma once

#include "Hitable.h"
#include "HitableList.h"
#include "Vec3.h"
#include "AABB.h"
#include "Rect.h"

class Box : public Hitable {
public:
	Box() {}
	Box(const Vec3& p0, const Vec3& p1, Material* mat);

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& box) const {
		box = AABB(pMin, pMax);
		return true;
	}

	Vec3 pMin, pMax;
	Hitable* list;
};

Box::Box(const Vec3& p0, const Vec3& p1, Material* mat) {
	pMin = p0;
	pMax = p1;
	Hitable** tmp = new Hitable*[6];

	tmp[0] = new XYRect(p0.x, p1.x, p0.y, p1.y, p1.z, mat);
	tmp[1] = new FlipNormals(new XYRect(p0.x, p1.x, p0.y, p1.y, p0.z, mat));
	tmp[2] = new XZRect(p0.x, p1.x, p0.z, p1.z, p1.y, mat);
	tmp[3] = new FlipNormals(new XZRect(p0.x, p1.x, p0.z, p1.z, p0.y, mat));
	tmp[4] = new YZRect(p0.y, p1.y, p0.z, p1.z, p1.x, mat);
	tmp[5] = new FlipNormals(new YZRect(p0.y, p1.y, p0.z, p1.z, p0.x, mat));

	list = new HitableList(tmp, 6);
}

bool Box::hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const {
	return list->hit(r, t_min, t_max, hitInfo);
}
