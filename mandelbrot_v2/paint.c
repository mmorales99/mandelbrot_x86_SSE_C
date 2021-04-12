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
#ifdef paint
const bool not_using_pragma_once = true;
#else
#define paint
#ifndef __cplusplus
#ifndef stdio_h
#include <stdio.h>
#endif
#else
#include <iostream>
using namespace std;
#endif

const bool not_using_pragma_once = false;

#ifndef xpixels
#define xpixels 1920
#endif
#ifndef ypixels
#define ypixels 1080
#endif

#ifndef int_arr
typedef struct int_arr { int m[xpixels + ypixels]; }IntArr;
#endif

// paint

#endif