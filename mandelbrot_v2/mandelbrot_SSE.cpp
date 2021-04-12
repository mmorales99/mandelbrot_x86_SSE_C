#ifdef mandel_SSE
const bool not_using_pragma_once = true;
#else
#define mandel_SSE

#include <iostream>
#include <xmmintrin.h>
#include "paint.cpp"
using namespace std;

#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

// SSE
inline /*struct int_arr*/ void mandelbrot_SSE(
	float xmin,	// minimo en el eje x	->	minimo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float xmax,	// maximo en el eje x	->	maximo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float ymin,	// minimo en el eje y	->	minimo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	float ymax, // maximo en el eje y	->	maximo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	unsigned int iteraciones	// numero de iteraciones a hacer
) {
	/*struct int_arr m;
	memset(&m.m, 0, sizeof(m.m));*/
	float x2 = 0, y2 = 0, xx = 0, yy = 0, xx2 = 0, yy2 = 0, xy2 = 0, temp = 0;

	float p = 0, q = 0, arrp = 0;
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
			__m128 p0 = _mm_add_ps(_mm_mul_ps(fp,pr), rmin);
			__m128 q0 = _mm_add_ps(_mm_mul_ps(vq,qi), imin);
			__m128 xx = _mm_set_ps1(0);
			__m128 yy = _mm_set_ps1(0);
			__m128 outerk = _mm_set_ps1(0);
			for (k = 0; k < iteraciones; k++) {
				__m128 x = _mm_add_ps(_mm_sub_ps(_mm_mul_ps(xx, xx), _mm_mul_ps(yy, yy)), p0);
				__m128 y = _mm_add_ps(_mm_mul_ps(_mm_set_ps1(2),_mm_mul_ps(xx,yy)), q0);
				xx = x;
				yy = y;
				__m128 x2 = _mm_mul_ps(x,x);
				__m128 y2 = _mm_mul_ps(y,y);
				__m128 temp = _mm_add_ps(x2,y2);
				__m128 mask = _mm_cmpeq_ps(temp, r);
				
				if (_mm_movemask_ps(mask) == 0) break;
			}
		}
	}
	_mm_empty();
	//return m;
}
#endif