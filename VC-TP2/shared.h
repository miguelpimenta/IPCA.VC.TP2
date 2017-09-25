//?  app.config/settings alternative
#ifndef SHARED_H
#define SHARED_H

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include <locale.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>
#include <windows.h>

#include "vc.h"

// Structs
struct FruitsInfo
{
	int numFruits = 0, numOranges = 0, numRedApples = 0, numGreenApples = 0;
};

// MAX / MIN
#define MAX2(a, b) ((a) > (b) ? a : b)
#define MIN2(a, b) ((a) < (b) ? a : b)
#define MAX3(a, b, c) (a > b ? (a > c ? a : c) : (b > c ? b : c))
#define MIN3(a, b, c) (a < b ? (a < c ? a : c) : (b < c ? b : c))

// Constantes
#define FILETOPROCESS ".\\input\\video-tp2.avi"
#define FILESDESTINATIONPATH ".\\Output\\"
#define WINDOWSNAME "CV-TP2"

#define BLOBPIXELSIZE 45000

#define SPLITSCREENLINESIZE 10
#define SPLITSCREEN_R 255
#define SPLITSCREEN_G 255
#define SPLITSCREEN_B 255

//! White HSV Params
static const int HSVWHITEHUE = 0;
static const int HSVWHITESAT = 50;
static const int HSVWHITEVALUE = 100;

//! Orange HSV Params
static const int HSVORANGEHUESTART = 16;
static const int HSVORANGEHUEEND = 35;
static const int HSVORANGESAT = 80;
static const int HSVORANGEVALUE = 50;
//! Red Apple HSV Params
static const int HSVREDAPPHUESTART = 0;
static const int HSVREDAPPHUEEND = 12;
static const int HSVREDAPPSAT = 40;
static const int HSVREDAPPVALUE = 50;
//! Green Apple HSV Params
static const int HSVGREENAPPHUESTART = 35;
static const int HSVGREENAPPHUEEND = 60;
static const int HSVGREENAPPSAT = 35;
static const int HSVGREENAPPVALUE = 20;

// Percentagem da imagem a ignorar (Top/Button/Left/Right)
static const int PERCENTIGNORE = 5;

// Mostrar Info dos Passos dados (Info na consola + mostrar imagens)
static int showSteps = 0;

// Cores para texto
#define CMAGEN   "\x1b[35m"
#define CRED     "\x1b[31m"
#define CGREEN   "\x1b[32m"
#define CYELLOW  "\x1b[33m"
#define CBLUE    "\x1b[34m"
#define CCYAN    "\x1b[36m"
#define CRESET   "\x1b[0m"

#endif