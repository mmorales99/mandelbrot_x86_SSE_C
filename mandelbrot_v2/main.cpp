#include <iostream>
using namespace std;
#include <string>
#include <chrono>
#include <fstream>
#include "paint.c"
#include "mandel_C.c"
#include "mandelbrot_SSE.cpp"
#include "mandelbrot_x86_64.cpp"

#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

#ifndef int_arr
typedef struct int_arr { int m[xpixels + ypixels]; }IntArr;
#endif

int main(int argc, char** argv) {

	string fname = "mandlebrot_bench";
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
	ff << "RUNNING CODE OF MANDLEBROT BENCHMARK\N\N";
	cout << "RUNNING CODE OF MANDLEBROT BENCHMARK\N\N";
#ifndef difftime
#define difftime(x,y) (double)(y - x)
#endif
	struct int_arr gradientMap;
	time_t start, end;
	start = clock();
	gradientMap = mandelbrot_SSE(0,xpixels,0,ypixels,3000);
	end = clock();
	ff << "SSE NEEDED: " << difftime(end, start) << "SECONDS.\n";
	cout << "SSE NEEDED: " << difftime(end, start) << "SECONDS.\n";
	draw(gradientMap,"mandelbrot_sse");
	start = clock();
	gradientMap = mandelbrot_x86(0, xpixels, 0, ypixels, 3000);
	end = clock();
	ff << "x86 NEEDED: " << difftime(end, start) << "SECONDS.\n";
	cout << "x86 NEEDED: " << difftime(end, start) << "SECONDS.\n";
	draw(gradientMap,"mandelbrot_x86");
	start = clock();
	gradientMap = mandelbrot_C(0, xpixels, 0, ypixels, 3000);
	end = clock();
	ff << "C++ NEEDED: " << difftime(end, start) << "SECONDS.\n";
	cout << "C++ NEEDED: " << difftime(end, start) << "SECONDS.\n";
	draw(gradientMap,"mandelbrot_cpp");
	ff.close();
}