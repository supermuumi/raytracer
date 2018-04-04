#pragma once

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

	ImageTexture(unsigned char* pixels, int a, int b) { 
		data = pixels;
		nx = a;
		ny = b;
	}

	Vec3 value(double u, double v, const Vec3& p) {
		int i = u * nx;
		int j = (1.0 - v)*ny - 0.001;
		if (i < 0) {
			i = 0;
		}
		if (j < 0) {
			j = 0;
		}

	}

	unsigned char* data;
	int nx, ny;
};