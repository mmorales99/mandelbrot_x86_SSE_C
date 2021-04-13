#ifdef mandel_x86
const bool not_using_pragma_once = true;
#else
#define mandel_x86

#include <iostream>
#include "paint.cpp"
using namespace std;

#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

// ASM
// EJE X === Reales --- EJE Y === Imaginarios
inline /*struct int_arr*/ void mandelbrot_x86(
	float xmin,	// minimo en el eje x	->	minimo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float xmax,	// maximo en el eje x	->	maximo en la recta real o la recta de representacion	->	Coincide con el eje X del ambito de representacion
	float ymin,	// minimo en el eje y	->	minimo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	float ymax, // maximo en el eje y	->	maximo en la recta imaginaria o la recta de representacion	->	Coincide con el eje Y del ambito de representacion
	int iteraciones	// numero de iteraciones a hacer
) {
	/*struct int_arr m;
	* int mm[xpixels*ypixels];
	memset(mm, 0, sizeof(mm));*/
	float p0=0, q0=0, x=0, y=0, x2 = 0, y2 = 0, xx=0, yy=0, xx2 = 0, yy2 = 0, xy2 = 0, temp = 0;

	int p=0, q=0, arrp = 0;
	int k = 0;

	const int rmin = xmin;
	const int rmax = xmax;
	const int imin = ymin;
	const int imax = ymax;
	/*
	CONSTANTS
		PR -> ESCALA REAL
		QI -> ESCALA IMAGINARIA
		R  -> CONDICION DE ESCAPE
	*/
	const float pr = (rmax - rmin) / (xpixels - 1);
	const float qi = (imax - imin) / (ypixels - 1);
	const int r = 4;

	__asm {
		fldz
		mov p, 0
		L_P:
			mov q, 0
			fst p0
			fld p0
			fiadd p
			fmul pr
			fadd xmin
			fstp p0

			mov EAX, p
			mov arrp, EAX
			mov EAX, 0

			fldz
			
			
			L_Q:
			fst q0
			fld q0
			fiadd q
			fmul qi
			fadd ymin
			fstp q0

			fldz
			fst xx
			fst yy

			mov k, 0

			mov EAX, q
			add arrp, EAX
			mov EAX, 0

			L_K:
			fld xx
			fstp xx2
			fld xx2
			fmul xx2
			fstp xx2

			fld yy
			fstp yy2
			fld yy2
			fmul yy2
			fstp yy2

			fld xx2
			fsub yy2
			fadd p0
			fstp x

			fld xx
			fstp xy2
			fld xy2
			fmul yy
			fadd xy2
			fadd q0
			fstp y

			fld x
			fstp xx
			fld y
			fstp yy

			fld x
			fstp x2
			fld x2
			fmul x2
			fstp x2
			fld y
			fstp y2
			fld y2
			fmul y2
			fstp y2

			fld x2
			fstp temp
			fld temp
			fadd y2
			ficomp r
			ja Salida_S

			mov EDX, k
			add k, 1
			cmp edx, iteraciones
			jb L_K

		Salida_S :
			mov EDX, q
			add q, 1
			cmp edx, imax
			jb L_Q

		mov EDX, p
		add p, 1
		cmp edx, rmax
		jb L_P
	}
	/*
	for (size_t i = 0; i < (xpixels + ypixels); i++) {
		m.m[i] = mm[i];
	}
	return m;*/
}
#endif