#include "functions.h"
#include "shared.h"

//! OK
#pragma region BGR to RGB
int BGR2RGB(IVC * srcdst)
{
	auto srcdstData = static_cast<unsigned char *>(srcdst->data);
	auto srcdstWidth = srcdst->width;
	auto srcdstHeight = srcdst->height;
	auto srcdstBytesperline = srcdst->bytesperline;
	auto srcdstChannels = srcdst->channels;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == nullptr))
	{
		return 0;
	}
	if (srcdstChannels != 3)
	{
		return 0;
	}

	for (int y = 0; y < srcdstHeight; y++)
	{
		for (int x = 0; x < srcdstWidth; x++)
		{
			int srcPos = y * srcdstBytesperline + x * srcdstChannels;

			unsigned char temp = srcdstData[srcPos];

			srcdstData[srcPos] = srcdstData[srcPos + 2];
			srcdstData[srcPos + 2] = temp;
		}
	}
	return 1;
}
#pragma endregion

//! OK
#pragma region RGB to BGR
int RGB2BGR(IVC * srcdst)
{
	auto srcdstData = static_cast<unsigned char *>(srcdst->data);
	auto srcdstWidth = srcdst->width;
	auto srcdstHeight = srcdst->height;
	auto srcdstBytesperline = srcdst->bytesperline;
	auto srcdstChannels = srcdst->channels;

	// Verificação de erros
	if ((srcdst->width <= 0) || (srcdst->height <= 0) || (srcdst->data == nullptr))
	{
		return 0;
	}
	if (srcdstChannels != 3)
	{
		return 0;
	}

	for (auto y = 0; y < srcdstHeight; y++)
	{
		for (auto x = 0; x < srcdstWidth; x++)
		{
			auto srcPos = y * srcdstBytesperline + x * srcdstChannels;

			auto temp = srcdstData[srcPos + 2]; srcdstData[srcPos];

			srcdstData[srcPos + 2] = srcdstData[srcPos];
			srcdstData[srcPos] = temp;
		}
	}
	return 1;
}
#pragma endregion

//!
#pragma region RGB to BIN
int RGB2BIN(IVC *srcImg, IVC *dstImgt)
{
	auto srcData = static_cast<unsigned char *>(srcImg->data);
	auto srcBytesperline = srcImg->width * srcImg->channels;
	auto srcChannels = srcImg->channels;
	auto *dstData = static_cast<unsigned char *>(dstImgt->data);
	auto dstBytesperline = dstImgt->width * dstImgt->channels;
	auto dstChannels = dstImgt->channels;
	auto width = srcImg->width;
	auto height = srcImg->height;

	// verificação de erros
	if ((srcImg->width <= 0) || (srcImg->height <= 0) || (srcImg->data == nullptr))
	{
		return 0;
	}
	if ((srcImg->width != dstImgt->width) || (srcImg->height != dstImgt->height))
	{
		return 0;
	}
	if ((srcImg->channels != 3) || (dstImgt->channels != 1))
	{
		return 0;
	}

	for (auto y = 0; y < height; y++)
	{
		for (auto x = 0; x < width; x++)
		{
			long int srcPos = y * srcBytesperline + x * srcChannels;
			long int dstPos = y * dstBytesperline + x * dstChannels;

			auto rf = static_cast<float>(srcData[srcPos]);
			auto gf = static_cast<float>(srcData[srcPos + 1]);
			auto bf = static_cast<float>(srcData[srcPos + 2]);

			if (rf == 0 && gf == 0 && bf == 0)
			{
				dstData[dstPos] = 0;
			}
			else
			{
				dstData[dstPos] = 255;
			}
		}
	}
	return 1;
}
#pragma endregion

//! Ok
#pragma region BIN to RGB
int BIN2RGB(IVC *src, IVC *dst)
{
	auto srcData = static_cast<unsigned char *>(src->data);
	auto dstData = static_cast<unsigned char *>(dst->data);
	auto srcBytesperline = src->width * src->channels;
	auto dstBytesperline = dst->width * dst->channels;
	auto srcChannels = src->channels;
	auto dstChannels = dst->channels;
	auto width = src->width;
	auto height = src->height;

	// verificação de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == nullptr))
	{
		return 0;
	}
	if ((src->width != dst->width) || (src->height != dst->height))
	{
		return 0;
	}
	if ((src->channels != 1) || (dst->channels != 3))
	{
		return 0;
	}

	for (auto y = 0; y < height; y++)
	{
		for (auto x = 0; x < width; x++)
		{
			long int srcPos = y * srcBytesperline + x * srcChannels;
			long int dstPos = y * dstBytesperline + x * dstChannels;

			if (static_cast<float>(srcData[srcPos]) == 255)
			{
				dstData[dstPos] = 255;
				dstData[dstPos + 1] = 255;
				dstData[dstPos + 2] = 255;
			}
			else
			{
				dstData[dstPos] = 0;
				dstData[dstPos + 1] = 0;
				dstData[dstPos + 2] = 0;
			}
		}
	}
	return 1;
}
#pragma endregion

//! OK
#pragma region Filtrar Frutas
int HSVFilter(IVC *srcImg, IVC *dstImg)
{
	auto srcData = static_cast<unsigned char *>(srcImg->data);
	auto *dstData = static_cast<unsigned char *>(dstImg->data);
	auto srcBytesperline = srcImg->bytesperline;
	auto dstBytesperline = dstImg->bytesperline;	
	auto srcChannels = srcImg->channels;
	auto dstChannels = dstImg->channels;
	auto srcWidth = srcImg->width;
	auto srcHeight = srcImg->height;

	for (auto y = 0; y < srcHeight; y++)
	{
		for (auto x = 0; x < srcWidth; x++)
		{
			auto srcPos = y * srcBytesperline + x * srcChannels;
			auto dstPos = y * dstBytesperline + x * dstChannels;

			//if (srcData[srcPos] == HSVWHITEHUE && srcData[srcPos + 1] <= HSVWHITESAT && srcData[srcPos + 3] == HSVWHITEVALUE)
			if (srcData[srcPos + 1] <= HSVWHITESAT)
			{
				dstData[dstPos] = 0;
			}
			else
			{
				dstData[dstPos] = 255;
			}
		}
	}
	return 1;
}
#pragma endregion

//! OK
#pragma region Linha ao meio do ecra
int SplitScreen(IVC *srcdst)
{
	auto srcData = static_cast<unsigned char *>(srcdst->data);
	auto srcBytesperline = srcdst->bytesperline;
	auto srcChannels = srcdst->channels;
	auto srcWidth = srcdst->width;
	auto srcHeight = srcdst->height;	
	
	// Verificação de erros da imagemOriginal
	if ((srcWidth <= 0) || (srcHeight <= 0) || (srcData == nullptr))
	{
		return 0;
	}
	if (srcChannels != 3)
	{
		return 0;
	}

	for (auto y = 0; y < srcWidth; y++)
	{
		for (auto x = ((static_cast<int>(srcHeight) / 2) - SPLITSCREENLINESIZE); x < (static_cast<int>(srcHeight) / 2) + SPLITSCREENLINESIZE; x++)
		{
			auto srcPos = (x * srcBytesperline) + (y * srcChannels);
			srcData[srcPos] = SPLITSCREEN_R;
			srcData[srcPos + 1] = SPLITSCREEN_G;
			srcData[srcPos + 2] = SPLITSCREEN_B;
		}
	}
	return 1;
}
#pragma endregion

//! OK
#pragma region Copiar conteudo BLOB
int CopyBlobPixels(IVC *srcImg, IVC *dstImg, int x, int y)
{
	auto srcData = static_cast<unsigned char *>(srcImg->data);
	auto dstData = static_cast<unsigned char *>(dstImg->data);
	auto srcBytesperline = srcImg->bytesperline;
	auto dstBytesperline = dstImg->bytesperline;
	auto srcChannels = srcImg->channels;
	auto dstChannels = dstImg->channels;
	auto dstWidth = dstImg->width;
	auto dstHeight = dstImg->height;

	// Verificação de erros da imagemOriginal
	//if ((srcWidth <= 0) || (srcHeight <= 0) || (srcData == nullptr))
	//{
	//	return 0;
	//}
	if (srcChannels != 3)
	{
		return 0;
	}

	for (auto i = x; i < x + dstWidth; i++)
	{
		for (auto j = y; j < y + dstHeight; j++)
		{
			auto srcPos = (j * srcBytesperline) + (i * srcChannels);
			auto dstPos = ((j - y) * dstBytesperline) + ((i - x) * dstChannels);

			dstData[dstPos] = srcData[srcPos];
			dstData[dstPos + 1] = srcData[srcPos + 1];
			dstData[dstPos + 2] = srcData[srcPos + 2];
		}
	}
	return 1;
}
#pragma endregion

//! OK
#pragma region CountColorPixels
int CountColorPixels(IVC *src)
{
	auto orangePix = 0, redApplePix = 0, greenApplePix = 0, output = 0;
	
	IVC *tempIVC = vc_image_new(src->width, src->height, src->channels, 255);
	memcpy(tempIVC->data, src->data, src->bytesperline * src->height);
	vc_rgb_to_hsv(tempIVC);

	auto size = tempIVC->width * tempIVC->height * tempIVC->channels;

	for (auto i = 0; i < size; i = i + tempIVC->channels)
	{
		auto hue = static_cast<float>(tempIVC->data[i]);
		auto saturation = static_cast<float>(tempIVC->data[i + 1]);
		auto value = static_cast<float>(tempIVC->data[i + 2]);			

		// Laranja
		if ((hue >= HSVORANGEHUESTART && hue <= HSVORANGEHUEEND) && ((saturation / 255.0f * 100.0f) >= HSVORANGESAT) && ((value / 255.0f * 100.0f) >= HSVORANGEVALUE))
		{
			orangePix++;
		}
		// Maça Vermelha
		else if ((hue >= HSVREDAPPHUESTART && hue <= HSVREDAPPHUEEND) && ((saturation / 255.0f * 100.0f) >= HSVREDAPPSAT) && ((value / 255.0f * 100.0f) >= HSVREDAPPVALUE))
		{
			redApplePix++;
		}
		// Maça verde
		else if ((hue >= HSVGREENAPPHUESTART && hue <= HSVGREENAPPHUEEND) && ((saturation / 255.0f * 100.0f) >= HSVGREENAPPSAT) && ((value / 255.0f * 100.0f) >= HSVGREENAPPVALUE))
		{
			greenApplePix++;
		}
	}
	
	if (MAX3(orangePix, redApplePix, greenApplePix) == orangePix)
	{
		output = 1;
	}
	else if (MAX3(orangePix, redApplePix, greenApplePix) == redApplePix)
	{
		output = 2;
	}
	else if (MAX3(orangePix, redApplePix, greenApplePix) == greenApplePix)
	{
		output = 3;
	}

	printf("Orange pixels: %d\n", orangePix);
	printf("Red Apple pixels: %d\n", redApplePix);
	printf("Green Apple Pixels: %d\n", greenApplePix);

	return output;	
}
#pragma endregion

//! Ok
#pragma region Erode
int Erode(IVC * srcDst, int erodeSize)
{
	auto srcData = static_cast<unsigned char *>(srcDst->data);
	auto srcBytesperline = srcDst->width* srcDst->channels;
	auto srcChannels = srcDst->channels;
	auto srcWidth = srcDst->width;
	auto srcHeight = srcDst->height;
	auto minimo = 0, maximo = 0;
	long int pos = 0, posk = 0;

	IVC *temp1 = vc_image_new(srcDst->width, srcDst->height, srcDst->channels, srcDst->levels);
	memcpy(temp1->data, srcDst->data, srcDst->bytesperline * srcDst->height);
	auto dstData1 = static_cast<unsigned char *>(temp1->data);

	if ((srcDst->width <= 0) || (srcDst->height <= 0) || (srcDst->data == nullptr))
	{
		return 0;
	}
	if ((srcDst->width != temp1->width) || (srcDst->height != temp1->height))
	{
		return 0;
	}
	if ((srcDst->channels != 1) || (temp1->channels != 1))
	{
		return 0;
	}

	//! Erode
	//for (y = 0; y < srcHeight; y++)
	for (auto y = ((srcHeight / 100) * PERCENTIGNORE); y < (srcHeight - ((srcHeight / 100) * PERCENTIGNORE)); y++)
	{
		for (auto x = 0; x < srcWidth; x++)
		//for (x = ((srcHeight / 100) * PERCENTIGNORE); x < (srcHeight - ((srcHeight / 100) * PERCENTIGNORE)); x++)
		{
			pos = y * srcBytesperline + x * srcChannels;
			dstData1[pos] = 0;
			minimo = 255;
			auto contaKernel = erodeSize / 2;
			if ((((y - contaKernel) >= 0) && ((x - contaKernel) >= 0) && ((y + contaKernel) < srcHeight - 1) && ((x + contaKernel) < srcWidth - 1)))
			{
				for (auto yy = y - contaKernel; yy <= y + contaKernel; yy++)
				{
					for (auto xx = x - contaKernel; xx <= x + contaKernel; xx++)
					{
						posk = yy * srcBytesperline + xx * srcChannels;

						if (minimo > srcData[posk])
						{
							minimo = srcData[posk];
						}
					}
				}
				dstData1[pos] = minimo;
			}
		}
	}

	return 1;
}
#pragma endregion

//! 
#pragma region ErodeDilate
int ErodeDilate(IVC * srcDst, int erodeSize, int dilateSize)
{
	auto srcData = static_cast<unsigned char *>(srcDst->data);
	auto srcBytesperline = srcDst->width* srcDst->channels;
	auto srcChannels = srcDst->channels;
	auto srcWidth = srcDst->width;
	auto srcHeight = srcDst->height;
	int x, y, xx, yy;
	auto minimo = 0, maximo = 0;
	long int pos = 0, posk = 0;

	IVC *temp1 = vc_image_new(srcDst->width, srcDst->height, srcDst->channels, srcDst->levels);
	memcpy(temp1->data, srcDst->data, srcDst->bytesperline * srcDst->height);
	auto dstData1 = static_cast<unsigned char *>(temp1->data);

	if ((srcDst->width <= 0) || (srcDst->height <= 0) || (srcDst->data == nullptr))
	{
		return 0;
	}
	if ((srcDst->width != temp1->width) || (srcDst->height != temp1->height))
	{
		return 0;
	}
	if ((srcDst->channels != 1) || (temp1->channels != 1))
	{
		return 0;
	}

	//! Erode
	for (y = 0; y < srcHeight; y++)
	{
		for (x = 0; x < srcWidth; x++)
		{
			pos = y * srcBytesperline + x * srcChannels;
			dstData1[pos] = 0;
			minimo = 255;
			auto contaKernel = erodeSize / 2;
			if ((((y - contaKernel) >= 0) && ((x - contaKernel) >= 0) && ((y + contaKernel) < srcHeight - 1) && ((x + contaKernel) < srcWidth - 1)))
			{
				for (yy = y - contaKernel; yy <= y + contaKernel; yy++)
				{
					for (xx = x - contaKernel; xx <= x + contaKernel; xx++)
					{
						posk = yy * srcBytesperline + xx * srcChannels;

						if (minimo > srcData[posk])
						{
							minimo = srcData[posk];
						}
					}
				}
				dstData1[pos] = minimo;
			}
		}
	}

	minimo = 0, maximo = 0, pos = 0, posk = 0;

	//! Dilate
	memcpy(srcDst->data, temp1->data, temp1->bytesperline * temp1->height);
	auto dstData2 = static_cast<unsigned char *>(temp1->data);

	for (y = 0; y < srcHeight; y++)
	{
		for (x = 0; x < srcWidth; x++)
		{
			pos = y * srcBytesperline + x * srcChannels;
			srcData[pos] = 0;
			maximo = 0;
			if ((((y - dilateSize / 2) >= 0) && ((x - dilateSize / 2) >= 0) && ((y + dilateSize / 2) < srcHeight - 1) && ((x + dilateSize / 2) < srcWidth - 1)))
			{
				for (yy = y - dilateSize / 2; yy <= y + dilateSize / 2; yy++)
				{
					for (xx = x - dilateSize / 2; xx <= x + dilateSize / 2; xx++)
					{
						posk = yy * srcBytesperline + xx * srcChannels;

						if (maximo < dstData2[posk])
						{
							maximo = dstData2[posk];
						}
					}
				}
				srcData[pos] = maximo;
			}
		}
	}
	return 1;
}
#pragma endregion