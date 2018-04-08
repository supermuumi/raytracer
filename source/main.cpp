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
#include "Rect.h"
#include "Hitable.h"
#include "Box.h"
#include "Instances.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#include "stb_image_write.h"

int gImageWidth = 200;
int gImageHeight = 112;
int gNumSamplesPerRay = 100;
int gMaxRayRecursion = 50;

Hitable* gWorld;
Camera gCamera;

const char* gCommandLineHelp = "\n\
Raytracer 3000 - Commandline help\n\
=================================\n\
\n\
-x N\tImage width in pixels (200)\n\
-y N\tImage height in pixels (112)\n\
-s N\tNumber of samples per pixel (100)\n\
-r N\tMax recursion steps per ray (50)\n\
";

Vec3 getRayColor(const Ray& r, Hitable* world, int depth) {
	HitRecord hit;
	if (world->hit(r, 0.0001, DBL_MAX, hit)) {
		Ray scattered;
		Vec3 attenuation;
		Vec3 emitted = hit.pMat->emitted(hit.u, hit.v, hit.point);
		if (depth < gMaxRayRecursion && hit.pMat->scatter(r, hit, attenuation, scattered)) {
			return emitted + attenuation * getRayColor(scattered, world, depth + 1);
		}
		else {
			return emitted;
		}
	}
	else {
		return  Vec3(0.0, 0.0, 0.0);
	}
}

Hitable* lightingTest() {
	Texture* noise = new ConstantTexture(Vec3(0.8, 0.0, 0.0));
	Hitable** list = new Hitable*[500];
	int i = 0;
	list[i++] = new Sphere(Vec3(0.0, -1000.0, 0.0), 1000.0, new Lambertian(noise));
	list[i++] = new Sphere(Vec3(0.0, 2.0, 0.0), 2.0, new Lambertian(noise));
	list[i++] = new Sphere(Vec3(0.0, 7.0, 0.0), 2.0, new DiffuseLight(new ConstantTexture(Vec3(4.0, 4.0, 4.0))));
	list[i++] = new XYRect(3, 5, 1, 3, -2, new DiffuseLight(new ConstantTexture(Vec3(4.0, 4.0, 4.0))));

	return new HitableList(list, i);
}

void createCornellBox() {
	Hitable** list = new Hitable*[500];

	Material* red = new Lambertian(new ConstantTexture(Vec3(0.65, 0.05, 0.05)));
	Material* white = new Lambertian(new ConstantTexture(Vec3(0.73, 0.73, 0.73)));
	Material* green = new Lambertian(new ConstantTexture(Vec3(0.12, 0.45, 0.15)));
	Material* light = new DiffuseLight(new ConstantTexture(Vec3(15.0, 15.0, 15.0)));

	int i = 0;

	list[i++] = new FlipNormals(new YZRect(0, 555, 0, 555, 555, green));
	list[i++] = new YZRect(0, 555, 0, 555, 0, red);
	list[i++] = new XZRect(213, 343, 227, 332, 554, light);
	list[i++] = new FlipNormals(new XZRect(0, 555, 0, 555, 555, white));
	list[i++] = new XZRect(0, 555, 0, 555, 0, white);
	list[i++] = new FlipNormals(new XYRect(0, 555, 0, 555, 555, white));

	list[i++] = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 165, 165), white), -18), Vec3(130, 0, 65));
	list[i++] = new Translate(new RotateY(new Box(Vec3(0, 0, 0), Vec3(165, 330, 165), white), 15), Vec3(265, 0, 295));

	gWorld = new BVHNode(list, i, 0.0, 1.0); // HitableList(list, i);
	Vec3 lookFrom(278, 278, -800);
	Vec3 lookAt(278, 278, 0);
	double distToFocus = 10.0;
	double aperture = 0.0;
	double vFov = 40.0;
	gCamera = Camera(lookFrom, lookAt, Vec3(0.0, 1.0, 0.0), vFov, double(gImageWidth) / double(gImageHeight), aperture, distToFocus, 0.0, 1.0);
}

void createScene() {
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
			mat = new Lambertian(new ConstantTexture(Vec3(0.8, 0.0, 0.0)));
		}
		else if (d < 0.8) {
			mat = new Metal(new ImageTexture("flowerpower.jpg"));
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

	gWorld = new BVHNode(list, i, 0.0, 1.0);
	Vec3 lookFrom(-5, 3, 1);
	Vec3 lookAt(0, 0, -1);
	double focalDist = (lookFrom - lookAt).length();
	double aperture = 0.0;
	gCamera = Camera(lookFrom, lookAt, Vec3(0.0, -1.0, 0.0), 90.0, double(gImageWidth) / double(gImageHeight), aperture, focalDist, 0.0, 1.0);
}

bool startsWith(const char *str, const char *pre) {
	size_t lenpre = strlen(pre),
		lenstr = strlen(str);
	return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}

void parseCommandLine(int argc, char* argv[]) {
	for (int i = 1; i < argc; i++) {
		if (strlen(argv[i]) > 1 && argv[i][0] == '-' && argc > i) {
			switch (argv[i][1]) {
				int tmp;
				case 'h':
					printf("%s\n", gCommandLineHelp);
					exit(1);
					break;

				case 'x':
					tmp = atoi(argv[i + 1]);
					if (tmp > 0) {
						gImageWidth = tmp;
					}
					i++;
					break;
				case 'y':
					tmp = atoi(argv[i + 1]);
					if (tmp > 0) {
						gImageHeight = tmp;
					}
					i++;
					break;
				case 'r':
					tmp = atoi(argv[i + 1]);
					if (tmp > 0) {
						gMaxRayRecursion = tmp;
					}
					i++;
					break;
				case 's':
					tmp = atoi(argv[i + 1]);
					if (tmp > 0) {
						gNumSamplesPerRay = atoi(argv[i + 1]);
					}
					i++;
					break;
			}
		}
	}
}

int main(int argc, char* argv[]) {
	parseCommandLine(argc, argv);

	//createScene();
	createCornellBox();

	unsigned char* imgData = new unsigned char[gImageWidth * gImageHeight * 3];

	SYSTEMTIME startTime;
	GetLocalTime(&startTime);

	for (int y = 0; y < gImageHeight; y++) {
		for (int x = 0; x < gImageWidth; x++) {
			Vec3 col(0.0, 0.0, 0.0);
			for (int s = 0; s < gNumSamplesPerRay; s++) {
				double u = double(x + drand()) / double(gImageWidth);
				double v = double(y + drand()) / double(gImageHeight);

				Ray r = gCamera.getRay(u, v);
				Vec3 tmp = getRayColor(r, gWorld, 0);
				col += tmp;
			}
			col /= double(gNumSamplesPerRay);
			col = Vec3(sqrt(col.x), sqrt(col.y), sqrt(col.z));

			int ofs = ((gImageHeight - 1 - y)*gImageWidth + x) * 3;
			imgData[ofs + 0] = int(255.0 * col.x);
			imgData[ofs + 1] = int(255.0 * col.y);
			imgData[ofs + 2] = int(255.0 * col.z);
		}
		printf("%.1f%%...\n", double(y)*100.0 / double(gImageHeight));
		if (y % 10 == 0) {
			stbi_write_png("temp.png", gImageWidth, gImageHeight, 3, imgData, 0);
		}
	}

	SYSTEMTIME st;
	GetLocalTime(&st);
	char filename[64] = "";
	sprintf_s(filename, "out_%d%02d%02d_%02d%02d%02d.png", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	printf("Wrote %s\n", filename);
	printf("Start: %02d:%02d:%02d\n", startTime.wHour, startTime.wMinute, startTime.wSecond);
	printf("End:   %02d:%02d:%02d\n", st.wHour, st.wMinute, st.wSecond);

	stbi_write_png(filename, gImageWidth, gImageHeight, 3, imgData, 0);

	delete gWorld;
	delete[] imgData;
}