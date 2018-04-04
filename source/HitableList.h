#pragma once

#include "Hitable.h"

class HitableList : public Hitable {
public:
	HitableList() {

	}
	HitableList(Hitable** l, int n) {
		list = l;
		listSize = n;
	}

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const;

	Hitable** list;
	int listSize;
};

bool HitableList::hit(const Ray& r, double t_min, double t_max, HitRecord& hitInfo) const {
	HitRecord tempHit;

	bool hitAnything = false;
	double closestHit = t_max;
	for (int i = 0; i < listSize; i++) {
		if (list[i]->hit(r, t_min, closestHit, tempHit)) {
			hitAnything = true;
			closestHit = tempHit.dist;
			hitInfo = tempHit;
		}
	}
	return hitAnything;
}

bool HitableList::boundingBox(double t0, double t1, AABB& box) const {

	if (listSize < 1) {
		return false;
	}

	AABB tempBox;
	bool firstTrue = list[0]->boundingBox(t0, t1, tempBox);
	if (!firstTrue) {
		return false;
	}
	else {
		box = tempBox;
	}

	for (int i = 1; i < listSize; i++) {
		if (list[i]->boundingBox(t0, t1, tempBox)) {
			box = combineAABB(box, tempBox);
		}
		else {
			return false;
		}
	}

	return true;
}
