#ifndef bool
	#define false 0
	#define FALSE false
	#define TRUE !FALSE
	#define true !false
	typedef char bool;
#endif
#ifdef __GNUG__ || __GNUC__ || __clang__
	#pragma once
#endif
#ifdef mandel_c
	const bool not_using_pragma_once = true;
#else
#define mandel_c

#include <stdio.h>

#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

#ifndef IntArr
	typedef struct int_arr{ int m[xpixels + ypixels] }IntArr;
#endif

struct int_arr mandelbrot_C(
	float xmin,	// minimo en el eje x	->	minimo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float xmax,	// maximo en el eje x	->	maximo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float ymin,	// minimo en el eje y	->	minimo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	float ymax, // maximo en el eje y	->	maximo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	unsigned int iteraciones	// numero de iteraciones a hacer
) {
	IntArr m;
	memset(&m.m, 0, sizeof(m.m));
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
struct int_arr mandelbrot_C_def() {
	return mandelbrot_C(1,xpixels,1,ypixels,500);
}

#ifndef main
int main(int argc, char** argv) {
	#include <string.h>
	#include <time.h>
	#include "paint.c"
	char fname[] = "mandlebrot_c";
	char fterm[] = ".times";
	char* aux;
	sprintf(aux,"%s%s",fname,fterm);
	FILE* f;
	f = fopen(aux, "r");
	unsigned existing_files = 0;
	while (f != NULL) {
		fclose(f);
		existing_files++;
		memset(&aux[0],0,sizeof(aux));
		sprintf(aux,"%s%d%s",fname,existing_files,fterm);
		f = fopen(aux, "r");
	}
	fclose(f);
	f = fopen(aux,"w+");
	fprintf(f,"%s","RUNNING C CODE OF MANDLEBROT AS STANDALONE\N\N");
	time_t start, end;
	start = time();
	struct int_arr gradientMap;
	gradientMap = mandelbrot_C_def();
	end = time();
#ifndef difftime
#define difftime(x,y) (double)(y - x)
#endif
	fprintf(f, "%s%0.2f%s", "C NEEDED: ", difftime(end,start) , "SECONDS.\n");
	fclose(f);
	draw(gradientMap,"mandelbrot_c");
}
#endif

#endif
