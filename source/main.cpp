#include <windows.h>
#include "Sphere.h"
#include "MovingSphere.h"
#include "HitableList.h"
#include "float.h"
#include "Camera.h"
#include "common.h"
#include "Material.h"
#include "BVHNode.h"
#include "Texture.h"
#include <Windows.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"

#define XSIZE 400
#define YSIZE 300
#define NUM_SAMPLES 100
#define MAX_RAY_RECURSION 50

Vec3 getRayColor(const Ray& r, Hitable* world, int depth) {
	HitRecord hit;
	if (world->hit(r, 0.0001, DBL_MAX, hit)) {
		Ray scattered;
		Vec3 attenuation;

		if (depth < MAX_RAY_RECURSION && hit.pMat->scatter(r, hit, attenuation, scattered)) {
			return attenuation * getRayColor(scattered, world, depth + 1);
		}
		else {
			return Vec3(0.0, 0.0, 0.0);
		}
	}
	else {
		Vec3 unitDir = unitVector(r.direction);
		double t = 0.5*(unitDir.y + 1.0);
		Vec3 v = (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
		return v;
	}
}

Hitable* createScene() {
	Hitable** list = new Hitable*[500];
	list[0] = new Sphere(Vec3(0.0, -100.5, -1.0), 100.0, new Lambertian(new CheckerTexture(new ConstantTexture(Vec3(0.8, 0.3, 0.3)), new ConstantTexture(Vec3(0.9, 0.9, 0.9)))));
	int i = 1;
	srand(715517);
	double x = drand();
	int nnn = rand();
	for (; i < 200; i++) {
		Vec3 center(10.0 - drand()*20.0, 0.0, 10.0 - drand()*20.0);
		double radius = 0.4 + drand()*0.2;
		Material* mat;
		double d = drand();
		if (d < 0.6) {
			mat = new Lambertian(new ConstantTexture(Vec3(drand(), drand(), drand())));
		}
		else if (d < 0.8) {
			mat = new Metal(Vec3(drand(), drand(), drand()));
		}
		else {
			mat = new Dielectric(0.75 + drand()*0.5);
		}
		if (d < 0.02) {
			list[i] = new MovingSphere(center, center + Vec3(0.0, 0.1 + drand()*0.9, 0.0), 0.0, 1.0, radius, mat);
		}
		else {
			list[i] = new Sphere(center, radius, mat);

		}
	}

	return new BVHNode(list, i, 0.0, 1.0);
	//return new HitableList(list, i);
}



int main(int argc, char* argv[]) {
	unsigned char* imgData = new unsigned char[XSIZE * YSIZE * 3];

	Hitable* world = createScene();

	Vec3 lookFrom(-5, 3, 1);
	Vec3 lookAt(0, 0, -1);
	double focalDist = (lookFrom - lookAt).length();
	double aperture = 0.2;
	Camera cam(lookFrom, lookAt, Vec3(0.0, -1.0, 0.0), 90.0, double(XSIZE) / double(YSIZE), aperture, focalDist, 0.0, 1.0);


	SYSTEMTIME startTime;
	GetLocalTime(&startTime);

	for (int y = 0; y < YSIZE; y++) {
		for (int x = 0; x < XSIZE; x++) {

			Vec3 col(0.0, 0.0, 0.0);
			for (int s = 0; s < NUM_SAMPLES; s++) {
				double u = double(x + drand()) / double(XSIZE);
				double v = double(y + drand()) / double(YSIZE);

				Ray r = cam.getRay(u, v);
				Vec3 tmp = getRayColor(r, world, 0);
				col += tmp;
			}
			col /= double(NUM_SAMPLES);
			col = Vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

			int ofs = (y*XSIZE + x) * 3;
			imgData[ofs + 0] = int(255.0 * col.x);
			imgData[ofs + 1] = int(255.0 * col.y);
			imgData[ofs + 2] = int(255.0 * col.z);
		}
		printf("%.1f%%...\n", double(y)*100.0 / double(YSIZE));
		if (y % 10 == 0) {
			stbi_write_png("temp.png", XSIZE, YSIZE, 3, imgData, 0);
		}
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	char filename[64] = "";
	sprintf_s(filename, "out_%d%02d%02d_%02d%02d%02d.png", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	printf("Start: %02d:%02d:%02d\n", startTime.wHour, startTime.wMinute, startTime.wSecond);
	printf("End:   %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);

	stbi_write_png(filename, XSIZE, YSIZE, 3, imgData, 0);

	delete world;
	delete[] imgData;
}