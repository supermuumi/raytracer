#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

class Vec3;

class Texture {
public:
	virtual Vec3 value(double u, double v, const Vec3& p) const = 0;
};

class ConstantTexture : public Texture {
public:
	ConstantTexture() {
	}

	ConstantTexture(Vec3 c) : color(c) {
	}

	virtual Vec3 value(double u, double v, const Vec3& p) const {
		return color;
	}

	Vec3 color;
};

class CheckerTexture : public Texture {
public:
	CheckerTexture() {
	}

	CheckerTexture(Texture* t0, Texture *t1) {
		odd = t0;
		even = t1;
	}

	virtual Vec3 value(double u, double v, const Vec3& p) const {
		double sines = sin(10.0 * p.x) * sin(10.0*p.y) * sin(10.0*p.z);
		if (sines < 0.0) {
			return odd->value(u, v, p);
		}
		return even->value(u, v, p);
	}

	Texture* odd;
	Texture* even;
};

class ImageTexture : public Texture {
public:
	ImageTexture() {
	}

	ImageTexture(unsigned char* pixels, int w, int h, int b) {
		data = pixels;
		width = w;
		height = h;
		bpp = b;
	}

	ImageTexture(char const *fname) {
		data = stbi_load(fname, &width, &height, &bpp, 0);
	}

	Vec3 value(double u, double v, const Vec3& p) const {
		int i = int(u * width);
		int j = int((1.0 - v)*height - 0.001);
		i = clamp(i, 0, width - 1);
		j = clamp(j, 0, height - 1);
		double r = data[bpp * (i + width * j)] / 255.0;
		double g = data[bpp * (i + width * j)+1] / 255.0;
		double b = data[bpp * (i + width * j)+2] / 255.0;
	
		return Vec3(r, g, b);
	}

	unsigned char* data;
	int width, height, bpp;
};