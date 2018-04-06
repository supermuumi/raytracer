#pragma once

#define drand() (double(rand())/(1.0+RAND_MAX))

int clamp(int n, int min, int max) {
	if (n < min) n = min;
	if (n > max) n = max;
	return n;
}