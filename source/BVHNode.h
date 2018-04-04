#pragma once

#include "common.h"
#include "Hitable.h"
#include "AABB.h"
#include "BVHNode.h"
#include <stdio.h>

int boxCompareX(const void* a, const void* b) {
	AABB boxLeft, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight)) {
		printf("ERROR: No bounding box in BVH constructor!\n");
	}
	if (boxLeft.min.x - boxRight.min.x < 0.0) {
		return -1;
	}
	return 1;
}

int boxCompareY(const void* a, const void* b) {
	AABB boxLeft, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight)) {
		printf("ERROR: No bounding box in BVH constructor!\n");
	}
	if (boxLeft.min.y - boxRight.min.y < 0.0) {
		return -1;
	}
	return 1;
}

int boxCompareZ(const void* a, const void* b) {
	AABB boxLeft, boxRight;
	Hitable* ah = *(Hitable**)a;
	Hitable* bh = *(Hitable**)b;
	if (!ah->boundingBox(0, 0, boxLeft) || !bh->boundingBox(0, 0, boxRight)) {
		printf("ERROR: No bounding box in BVH constructor!\n");
	}
	if (boxLeft.min.z - boxRight.min.z < 0.0) {
		return -1;
	}
	return 1;
}

class BVHNode : public Hitable {
public:
	BVHNode() { }
	BVHNode(Hitable** list, int n, double time0, double time1);

	virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& HitInfo) const;
	virtual bool boundingBox(double t0, double t1, AABB& Box) const;

	Hitable* left;
	Hitable* right;
	AABB box;
};

bool BVHNode::boundingBox(double t0, double t1, AABB& b) const {
	b = box;
	return true;
}

BVHNode::BVHNode(Hitable** list, int n, double time0, double time1) {
	int axis = int(3 * drand());
	if (axis == 0) {
		qsort(list, n, sizeof(Hitable*), boxCompareX);
	}
	else if (axis == 1) {
		qsort(list, n, sizeof(Hitable*), boxCompareY);
	}
	else if (axis == 2) {
		qsort(list, n, sizeof(Hitable*), boxCompareZ);
	}

	if (n == 1) {
		left = right = list[0];
	}
	else if (n == 2) {
		left = list[0];
		right = list[1];
	}
	else {
		left = new BVHNode(list, n / 2, time0, time1);
		right = new BVHNode(list+n/2, n-n/2, time0, time1);
	}
	AABB boxLeft, boxRight;
	if (!left->boundingBox(time0, time1, boxLeft) || !right->boundingBox(time0, time1, boxRight)) {
		printf("ERROR: No bounding box in BVHNode constructor!\n");
	}
	box = combineAABB(boxLeft, boxRight);
}

bool BVHNode::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
	if (box.hit(r, t_min, t_max)) {
		HitRecord leftRec, rightRec;
		bool hitLeft = left->hit(r, t_min, t_max, leftRec);
		bool hitRight = right->hit(r, t_min, t_max, rightRec);
		if (hitLeft && hitRight) {
			if (leftRec.dist < rightRec.dist) {
				rec = leftRec;
			}
			else {
				rec = rightRec;
			}
			return true;
		}
		else if (hitLeft) {
			rec = leftRec;
			return true;
		}
		else if (hitRight) {
			rec = rightRec;
			return true;
		}
		return false;
	}
	return false;
}