#ifndef __cplusplus
#ifndef bool
#define false 0
#define FALSE false
#define TRUE !FALSE
#define true !false
typedef char bool;
#endif
#endif
#ifdef mandel_c
	const bool not_using_pragma_once = true;
#else
#define mandel_c

#include <iostream>
#include "paint.cpp"
using namespace std;

#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

inline vector<int> mandelbrot_C(
	float xmin,	// minimo en el eje x	->	minimo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float xmax,	// maximo en el eje x	->	maximo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float ymin,	// minimo en el eje y	->	minimo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	float ymax, // maximo en el eje y	->	maximo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	unsigned int iteraciones	// numero de iteraciones a hacer
) {
	vector<int> m;
	//memset(&m.m, 0, sizeof(m.m));
	float p0, q0, x, y, xx, yy;

	unsigned int p = 0, q = 0;
	unsigned k = 0;

	/*
	CONSTANTS
		PR -> ESCALA REAL
		QI -> ESCALA IMAGINARIA
		R  -> CONDICION DE ESCAPE
	*/
	const float pr = (xmax - xmin) / (xpixels - 1);
	const float qi = (ymax - ymin) / (ypixels - 1);
	const unsigned int r = 4;

	for (p = 0; p < xpixels; p++) {		
		p0 = xmin + p * pr;
		for (q = 0; q < ypixels; q++) {
			q0 = ymin + q * qi;
			xx = yy = 0;
			m.push_back(0);
			for (k = 0; k < iteraciones; k++) {	
				m[q]+=k;
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
#endif
