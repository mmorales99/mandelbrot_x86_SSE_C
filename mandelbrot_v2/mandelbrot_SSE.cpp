#ifdef mandel_SSE
const bool not_using_pragma_once = true;
#else
#define mandel_SSE

#include <iostream>
#include <xmmintrin.h>
using namespace std;

#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

#ifndef int_arr
typedef struct int_arr { int m[xpixels + ypixels]; }IntArr;
#endif

// SSE
struct int_arr mandelbrot_SSE(
	float xmin,	// minimo en el eje x	->	minimo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float xmax,	// maximo en el eje x	->	maximo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float ymin,	// minimo en el eje y	->	minimo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	float ymax, // maximo en el eje y	->	maximo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	unsigned int iteraciones	// numero de iteraciones a hacer
) {
	IntArr m;
	memset(&m.m, 0, sizeof(m.m));
	float x = 0, y = 0, x2 = 0, y2 = 0, xx = 0, yy = 0, xx2 = 0, yy2 = 0, xy2 = 0, temp = 0;

	unsigned int p = 0, q = 0, arrp = 0;
	unsigned k = 0;

	__m128 rmin = _mm_set_ps1(xmin);
	__m128 rmax = _mm_set_ps1(xmax);
	__m128 imin = _mm_set_ps1(ymin);
	__m128 imax = _mm_set_ps1(ymax);
	/*
	CONSTANTS
		PR -> ESCALA REAL
		QI -> ESCALA IMAGINARIA
		R  -> CONDICION DE ESCAPE
	*/
	__m128 pr = _mm_set_ps1((xmax - xmin) / (xpixels - 1));
	__m128 qi = _mm_set_ps1((ymax - ymin) / (ypixels - 1));
	__m128 r = _mm_set_ps1(4);
	__m128 uno = _mm_set_ps1(1);

#pragma omp parallel for schedule(dynamic, 1)
	for (p = 0; p < xpixels; p++) {
		//p0 = xmin + p * pr;
		for (q = 0; q < ypixels; q+=4) {
			//q0 = ymin + q * qi;
			__m128 vq = _mm_set_ps(q + 3, q + 2, q + 1, q);
			__m128 fp = _mm_set_ps1(p);
			__m128 p0 = _mm_add_ps(_mm_mul_ps());
			__m128 q0
			xx = yy = 0;
			for (k = 0; k < iteraciones; k++) {
				m.m[p + q] += 1;
				x = xx * xx - yy * yy + p0;
				y = 2 * xx * yy + q0;
				xx = x;
				yy = y;
				if (x * x + y * y > r) {
					break;
				}
			}
		}
	}
	return m;
}
struct int_arr mandelbrot_SSE_def() {
	return mandelbrot_SSE(1, xpixels, 1, ypixels, 500);
}
#ifndef main
#include <string>
#include <chrono>
#include <fstream>
#include "paint.c"
int main(int argc, char** argv) {
	string fname = "mandlebrot_sse";
	string fterm = ".times";
	string aux;
	aux = fname + fterm;
	ifstream f;
	f.open(aux, ios::in);
	unsigned existing_files = 0;
	while (f.is_open()) {
		f.close();
		existing_files++;
		aux.clear();
		aux += fname;
		aux += existing_files;
		aux += fterm;
		f.open(aux, ios::in);
	}
	f.close();
	ofstream ff;
	ff.open(aux, ios::out);
	ff << "RUNNING SSE CODE OF MANDLEBROT AS STANDALONE\N\N";
	time_t start, end;
	start = clock();
	IntArr gradientMap = mandelbrot_SSE_def();
	end = clock();
#ifndef difftime
#define difftime(x,y) (double)(y - x)
#endif
	ff << "C NEEDED: " << difftime(end, start) << "SECONDS.\n";
	ff.close();
	draw(gradientMap,"mandelbrot_sse");
}
#endif
#endif