#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "shared.h"

int BGR2RGB(IVC *srcdst);

int RGB2BGR(IVC *srcdst);

int RGB2BIN(IVC *srcImg, IVC *dstImgt);

int BIN2RGB(IVC *srcImg, IVC *dstImgt);

int HSVFilter(IVC *srcImg, IVC *dstImg);

int SplitScreen(IVC *srcdst);

int CopyBlobPixels(IVC *srcImg, IVC *dstImg, int x, int y);

int CountColorPixels(IVC *srcImg);

int ErodeDilate(IVC * srcDst, int erodeSize, int dilateSize);

int Erode(IVC * srcDst, int erodeSize);

#endif FUNCTIONS_H