#ifdef mandel_x86
const bool not_using_pragma_once = true;
#else
#define mandel_x86

#include <iostream>
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

// ASM
// EJE X === Reales --- EJE Y === Imaginarios
struct int_arr mandelbrot_x86(
	float xmin,	// minimo en el eje x	->	minimo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float xmax,	// maximo en el eje x	->	maximo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float ymin,	// minimo en el eje y	->	minimo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	float ymax, // maximo en el eje y	->	maximo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	unsigned int iteraciones	// numero de iteraciones a hacer
) {
	IntArr m;
	int mm[xpixels + ypixels];
	memset(&m.m,0,sizeof(m.m));
	float p0=0, q0=0, x=0, y=0, x2 = 0, y2 = 0, xx=0, yy=0, xx2 = 0, yy2 = 0, xy2 = 0, temp = 0;

	unsigned int p=0, q=0, arrp = 0;
	unsigned k = 0;

	const float rmin = xmin;
	const float rmax = xmax;
	const float imin = ymin;
	const float imax = ymax;
	/*
	CONSTANTS
		PR -> ESCALA REAL
		QI -> ESCALA IMAGINARIA
		R  -> CONDICION DE ESCAPE
	*/
	const float pr = (rmax - rmin) / (xpixels - 1);
	const float qi = (imax - imin) / (ypixels - 1);
	const unsigned int r = 4;

	__asm {
		fldz;
		mov p,0;
	loopp:; for p = 0; p < xpixels; p++
		fst p0;
		fld p0;
		fiadd p;
		fmul pr;
		fadd xmin;
		fstp p0;	// p0 = p*pr + xmin

		mov EAX, p;
		mov arrp, EAX;
		mov EAX, 0;

		fldz;
		mov q, 0;
	loopq:; for q = 0; q < ypixels; q++
		fst q0;
		fld q0;
		fiadd q;
		fmul qi;
		fadd ymin;
		fstp q0;	// q0 = q*qi + ymin

		fldz;
		fst xx;
		fst yy;

		mov EAX, q;
		add arrp, EAX;
		mov EAX, 0;
		
		mov k, 0;
	loopk:			; for k = 0; k < iteraciones; k++
		
		mov esi, mm;
		mov edi, DWORD[esi+arrp*8];
		add edi, 1;
		mov DWORD[esi + arrp * 8], edi; // m[p+q]++

		fld xx;
		fstp xx2;
		fld xx2;
		fmul xx2;
		fstp xx2; // xx*xx

		fld yy;
		fstp yy2;
		fld yy2;
		fmul yy2;
		fstp yy2; // yy*yy

		fld xx2;
		fsub yy2;
		fadd p0;
		fstp x; // x = xx*xx - yy*yy +p0

		fld xx;
		fstp xy2;
		fld xy2;
		fmul yy;
		fadd xy2; // x + x = 2*x
		fadd q0;
		fstp y; // y = 2*xx*yy+q0

		fld x;
		fstp xx;
		fld y;
		fstp yy;

		fld x;
		fstp x2;
		fld x2;
		fmul x2;
		fstp x2; // x*x
		fld y;
		fstp y2;
		fld y2;
		fmul y2;
		fstp y2; // y*y
		
		fld x2;
		fstp temp;
		fld temp;
		fadd y2;
		fstp temp; // temp = x** + y**
		fld temp;
		ficomp r; // if temp > r
		ja out;
		mov EAX, iteraciones;
		comp EAX, k;
		add k, 1;
		jb loopk;
	out:
		mov EAX, ypixels;
		comp EAX, q;
		jb loopq;
		mov EAX, xpixels;
		comp EAX, p;
		jb loopp;
	}
	for (size_t i = 0; i < (xpixels + ypixels); i++) {
		m.m[i] = mm[i];
	}
	return m;
}
struct int_arr mandelbrot_x86_def() {
	return mandelbrot_x86(1, xpixels, 1, ypixels, 500);
}

#ifndef main
#include <string>
#include <chrono>
#include <fstream>
#include "paint.c"
int main(int argc, char** argv) {
	string fname = "mandlebrot_x86";
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
	ff << "RUNNING x86 CODE OF MANDLEBROT AS STANDALONE\N\N";
	time_t start, end;
	start = clock();
	IntArr gradientMap = mandelbrot_x86_def();
	end = clock();
#ifndef difftime
#define difftime(x,y) (double)(y - x)
#endif
	ff << "C NEEDED: " << difftime(end, start) << "SECONDS.\n";
	ff.close();
	draw(gradientMap,"mandelbrot_x86");
}
#endif
#endif