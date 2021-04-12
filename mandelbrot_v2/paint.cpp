#ifndef __cplusplus
	#ifndef bool
	#define false 0
	#define FALSE false
	#define TRUE !FALSE
	#define true !false
	typedef char bool;
	#endif
#endif
#ifdef paint
#else
#define paint

#include <iostream>
#include <vector>
using namespace std;


#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

#ifndef INT_ARR
#define INT_ARR
struct int_arr { vector<int> m; };
#endif
// paint
 // el bit map me da segmentation fault, lo estoy probando, en saber como ponerlo, lo subo
#endif