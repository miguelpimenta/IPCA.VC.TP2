#define _CRT_SECURE_NO_WARNINGS

#include "shared.h"
#include "functions.h"
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include <opencv\highgui.h>
#include "opencv2/opencv.hpp"
#include <thread>

using namespace std;
using namespace cv;

/*
Objectivos
Os grupos deverão desenvolver um programa em linguagem C, ou C++, que processe o vídeo “video - tp2.avi”,
de modo a obter o seguinte conjunto de informação no final da sequência de vídeo :
//! Done • Número total de peças de fruta observadas ao longo do vídeo;
//! Done • Número total de laranjas observadas ao longo do vídeo;
//! Done • Número total de maçãs verdes observadas ao longo do vídeo;
//! Done • Número total de maçãs vermelhas observadas ao longo do vídeo;
//! Done • Área e perímetro(em pixéis) de todas as peças de fruta observadas ao longo do vídeo;
//! Done • Desenho, sobre a imagem a exibir em vídeo, da localização(área delimitadora) e centro de gravidade de cada peça de fruta.
Esta informação deverá ser exibida, sobre o vídeo fornecido.
*/

int main(void)
{
	#pragma region Variaveis
	auto pathImgDestination = static_cast<char *>(malloc(strlen(FILESDESTINATIONPATH) + strlen("test.pbm")));
	strcpy(pathImgDestination, FILESDESTINATIONPATH);
	strcat(pathImgDestination, "test.pbm");

	// Cmd abrir Imagem alterada
	char fileOutput[100];
	//strcpy(fileOutput, "cmd /c start FilterGear.exe ");
	strcpy(fileOutput, "FilterGear.exe ");
	strcat(fileOutput, pathImgDestination);

	struct
	{
		int width, height;
		int ntotalframes;
		int fps;
		int nframe;
	} video;

	FruitsInfo fruitsInfo;

	// Texto
	CvFont font, fontbkg;
	auto hScale = 0.5;
	auto vScale = 0.5;
	auto lineWidth = 1;
	char str[500] = { 0 };
	// Outros
	auto key = 0;
	auto processFrame = 1;
	auto skipFrames = 1;
	auto counter = skipFrames;
	auto blobCenterPos = 0;
	#pragma endregion

	#pragma region Video / Frames / Imagens
	// Vídeo
	char *videofile = FILETOPROCESS;
	CvCapture *capture = nullptr;
	// Imagens IPL (Frames)
	IplImage *frameOrig = nullptr;
	// Imagens IVC
	IVC *originalFrameIVC = nullptr;
	IVC *workedFrameIVC = nullptr;
	IVC *roiIVC = nullptr;
	IVC *binFrame1IVC = nullptr;
	IVC *binFrame2IVC = nullptr;
	// BLOBs
	OVC *blobs = nullptr;
	auto nblobs = 0;
	#pragma endregion

	#pragma region Captura Video
	/* Leitura de vídeo de um ficheiro */
	capture = cvCaptureFromFile(videofile);

	/* Verifica se foi possível abrir o ficheiro de vídeo */
	if (!capture)
	{
		fprintf(stderr, "Erro ao abrir o ficheiro de vídeo!\n");
		return 1;
	}
	#pragma endregion

	#pragma region Info Video (Total Frames / Framerate (FPS) / Resolution)
	video.ntotalframes = static_cast<int>(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT));
	video.fps = static_cast<int>(cvGetCaptureProperty(capture, CV_CAP_PROP_FPS));
	video.width = static_cast<int>(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH));
	video.height = static_cast<int>(cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT));
	#pragma endregion

	#pragma region Criar janela video / Inicializar fontes
	/* Cria uma janela para exibir o vídeo */
	cvNamedWindow(WINDOWSNAME, CV_WINDOW_AUTOSIZE);
	/* Inicializa a fonte */
	cvInitFont(&font, CV_FONT_HERSHEY_TRIPLEX, hScale, vScale, 0, lineWidth);
	cvInitFont(&fontbkg, CV_FONT_HERSHEY_TRIPLEX, hScale, vScale, 0, lineWidth);
	#pragma endregion

	// Save Video
	//auto fourcc = CV_FOURCC('H', '2', '6', '4');
	//CvVideoWriter *writer = cvCreateVideoWriter(".\\Output\\output.avi", fourcc, 15, cvSize(video.width, video.height));

	while (key != 'q')
	{
		#pragma region Ler frame
		/* Leitura de uma frame do vídeo */
		frameOrig = cvQueryFrame(capture);

		/* Verifica se conseguiu ler a frame */
		if (!frameOrig)
		{
			break;
		}

		/* Número da frame a processar */
		video.nframe = static_cast<int>(cvGetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES));
		#pragma endregion

		// Faça o seu código aqui...
		#pragma region Inicializar IVCs
		if (originalFrameIVC == nullptr)
		{
			originalFrameIVC = vc_image_new(frameOrig->width, frameOrig->height, frameOrig->nChannels, frameOrig->depth);
		}
		if (workedFrameIVC == nullptr)
		{
			workedFrameIVC = vc_image_new(frameOrig->width, frameOrig->height, frameOrig->nChannels, frameOrig->depth);
		}
		if (binFrame1IVC == nullptr)
		{
			binFrame1IVC = vc_image_new(frameOrig->width, frameOrig->height, 1, 255);
		}
		if (binFrame2IVC == nullptr)
		{
			binFrame2IVC = vc_image_new(frameOrig->width, frameOrig->height, 1, 255);
		}
		#pragma endregion

		originalFrameIVC->data = reinterpret_cast<unsigned char*>(frameOrig->imageData);
		originalFrameIVC->bytesperline = frameOrig->width * frameOrig->nChannels;

		BGR2RGB(originalFrameIVC);

		memcpy(workedFrameIVC->data, originalFrameIVC->data, originalFrameIVC->bytesperline * originalFrameIVC->height);

		vc_rgb_to_hsv(workedFrameIVC);
		HSVFilter(workedFrameIVC, binFrame1IVC);
		//Erode(binFrame1IVC, 3);

		//SplitScreen(originalFrameIVC);

		/*frameOrig->imageData = reinterpret_cast<char*>(originalFrameIVC->data);
		frameOrig->width = originalFrameIVC->width;
		frameOrig->height = originalFrameIVC->height;
		frameOrig->nChannels = originalFrameIVC->channels;	*/

		#pragma region Blobs
		// BLOB Info
		bool doBloobs = true;
		if (doBloobs == true)
		{
			blobs = vc_binary_blob_labelling(binFrame1IVC, binFrame2IVC, &nblobs);
			vc_binary_blob_info(binFrame2IVC, blobs, nblobs);
		}

		blobs = vc_binary_blob_labelling(binFrame1IVC, binFrame2IVC, &nblobs);
		vc_binary_blob_info(binFrame2IVC, blobs, nblobs);
		if (blobs != nullptr && doBloobs == true)
		{
			int raio;
			for (auto i = 0; i < nblobs; i++)
			{
				// Apenas blobs superiores a X nº de pixeis de area
				if (blobs[i].area > BLOBPIXELSIZE)
				{
					// Raio
					auto xr = static_cast<int>(blobs[i].xc - blobs[i].x);
					auto yr = static_cast<int>(blobs[i].yc - blobs[i].y);

					if (xr > yr)
					{
						raio = xr;
					}
					else
					{
						raio = yr;
					}

					if (blobs[i].yc >= ((originalFrameIVC->height - 10) / 2) && blobs[i].yc < ((originalFrameIVC->height + 10) / 2))
					{
						if (blobCenterPos == 0 || blobCenterPos - blobs[i].xc > 30 || blobCenterPos - blobs[i].xc < -30)
						{
							// Flash
							SplitScreen(originalFrameIVC);
							// Incrementa total de frutas
							fruitsInfo.numFruits++;
							printf("Fruta: %d\n", fruitsInfo.numFruits);
							printf("Area: %d\n", blobs[i].area);
							printf("Perimetro: %d\n", blobs[i].perimeter);
							// Criar imagem para copiar conteudo do blob
							roiIVC = vc_image_new(blobs[i].width, blobs[i].height, originalFrameIVC->channels, originalFrameIVC->levels);
							CopyBlobPixels(originalFrameIVC, roiIVC, blobs[i].x, blobs[i].y);
							// Contar pixeis
							auto fruitType = CountColorPixels(roiIVC);

							// Incrementar fruta detectada
							if (fruitType == 1)
							{
								printf("Orange\n");
								fruitsInfo.numOranges++;
							}
							else if (fruitType == 2)
							{
								printf("Red Apple\n");
								fruitsInfo.numRedApples++;
							}
							else if (fruitType == 3)
							{
								printf("Green Apple\n");
								fruitsInfo.numGreenApples++;
							}

							//vc_write_image(pathImgDestination, roiIVC);
							//system(fileOutput);

							/*printf("Inicio Meio: %d\n", ((originalFrameIVC->height - 10) / 2));
							printf("Fim Meio: %d\n", ((originalFrameIVC->height + 10) / 2));
							printf("blobCenterPos: %d\n", blobCenterPos);
							printf("Blob XC: %d\n", blobs[i].xc);
							printf("Blob XC: %d\n", blobs[i].xc);*/
							printf("\n");

							blobCenterPos = blobs[i].xc;

							vc_image_free(roiIVC);
						}
					}

					#pragma region Bounding Box + Centro Massa
					// Início e fim do blob
					if (((blobs[i].yc - raio) > 1) && ((blobs[i].yc + raio) < binFrame2IVC->height))
					{
						auto limiteInf = blobs[i].yc + raio + 20;

						cvRectangle(frameOrig, cvPoint(blobs[i].xc - 15, limiteInf - 15), cvPoint(blobs[i].xc + 200, limiteInf + 30), cvScalar(200, 255, 255), -1, 8);
						sprintf(str, "Area: %d pixels", blobs[i].area);
						cvPutText(frameOrig, str, cvPoint(blobs[i].xc - 10, limiteInf), &font, cvScalar(0, 0, 0, 0));
						sprintf(str, "Perimetro: %d pixels", blobs[i].perimeter);
						cvPutText(frameOrig, str, cvPoint(blobs[i].xc - 10, limiteInf + 20), &font, cvScalar(0, 0, 0, 0));
						//sprintf(str, "Raio: %d cm", raio);
						//cvPutText(frameOrig, str, cvPoint(blobs[i].xc - 10, margem + 40), &font, cvScalar(255, 0, 0, 0));
						// Centro de gravidade + Localização fruta
						cvCircle(frameOrig, cvPoint(blobs[i].xc, blobs[i].yc), 6, cvScalar(255, 50, 50, 0), 4, 4, 0);
						cvCircle(frameOrig, cvPoint(blobs[i].xc, blobs[i].yc), raio, cvScalar(0, 255, 0, 0), 4, 2, 0);
					}
					#pragma endregion

				}
			}
		}
		#pragma endregion

		/*SplitScreen(originalFrameIVC);*/
		RGB2BGR(originalFrameIVC);

		frameOrig->imageData = reinterpret_cast<char*>(originalFrameIVC->data);
		frameOrig->width = originalFrameIVC->width;
		frameOrig->height = originalFrameIVC->height;
		frameOrig->nChannels = originalFrameIVC->channels;

		/*BIN2RGB(binFrame1IVC, workedFrameIVC);
		frameOrig->imageData = reinterpret_cast<char*>(workedFrameIVC->data);
		frameOrig->width = workedFrameIVC->width;
		frameOrig->height = workedFrameIVC->height;
		frameOrig->nChannels = workedFrameIVC->channels;*/

		#pragma region Inserir texto na frame
		/* Exemplo de inserção texto na frame */
		cvRectangle(frameOrig, cvPoint(5, 5), cvPoint(250, 90), cvScalar(200, 255, 255), -1, 8);
		sprintf(str, "RESOLUCAO: %d x %d", video.width, video.height);
		cvPutText(frameOrig, str, cvPoint(10, 20), &font, cvScalar(0, 0, 0));
		sprintf(str, "TOTAL FRAMES: %d", video.ntotalframes);
		cvPutText(frameOrig, str, cvPoint(10, 40), &font, cvScalar(0, 0, 0));
		sprintf(str, "FRAME RATE: %d", video.fps);
		cvPutText(frameOrig, str, cvPoint(10, 60), &font, cvScalar(0, 0, 0));
		sprintf(str, "N. FRAME: %d", video.nframe);
		cvPutText(frameOrig, str, cvPoint(10, 80), &font, cvScalar(0, 0, 0));

		// Informação Frutas
		cvRectangle(frameOrig, cvPoint(5, video.height - 95), cvPoint(250, video.height - 10), cvScalar(200, 255, 255), -1, 8);
		sprintf(str, "N. FRUTAS: %d", fruitsInfo.numFruits);
		cvPutText(frameOrig, str, cvPoint(10, video.height - 80), &font, cvScalar(0, 200, 0));
		sprintf(str, "N. LARANJAS: %d", fruitsInfo.numOranges);
		cvPutText(frameOrig, str, cvPoint(10, video.height - 60), &font, cvScalar(0, 100, 0));
		sprintf(str, "N. MACAS VERMELHAS: %d", fruitsInfo.numRedApples);
		cvPutText(frameOrig, str, cvPoint(10, video.height - 40), &font, cvScalar(0, 100, 0));
		sprintf(str, "N. MACAS VERDES: %d", fruitsInfo.numGreenApples);
		cvPutText(frameOrig, str, cvPoint(10, video.height - 20), &font, cvScalar(0, 100, 0));

		if (video.nframe == video.ntotalframes)
		{
			cvRectangle(frameOrig, cvPoint(video.width - 180, 5), cvPoint(video.width - 10, 30), cvScalar(0, 0, 255), -1, 8);
			sprintf(str, "q para terminar");
			cvPutText(frameOrig, str, cvPoint(video.width - 170, 15), &font, cvScalar(0, 200, 0));
			sprintf(str, "[A12555] Miguel Pimenta");
			cvPutText(frameOrig, str, cvPoint(video.width - 250, video.height - 100), &font, cvScalar(0, 0, 0));
			sprintf(str, "[A4479] Ricardo Faria");
			cvPutText(frameOrig, str, cvPoint(video.width - 250, video.height - 75), &font, cvScalar(0, 0, 0));
			sprintf(str, "[A3503] Paulo Lima");
			cvPutText(frameOrig, str, cvPoint(video.width - 250, video.height - 50), &font, cvScalar(0, 0, 0));
			cvShowImage(WINDOWSNAME, frameOrig);
			key = cvWaitKey();
		}
		#pragma endregion

		/* Exibe a frame */
		cvShowImage(WINDOWSNAME, frameOrig);
		// Gravar frame como video
		//cvWriteFrame(writer, frameOrig);
		/* Sai da aplicação, se o utilizador premir a tecla 'q' */
		key = cvWaitKey(1);
	}
	#pragma region Fechar janela / Libertar recursos alocados
	/* Fecha a janela */
	cvDestroyWindow(WINDOWSNAME);
	/* Fecha o ficheiro de vídeo */
	//cvReleaseCapture(&capture);
	//vc_image_free(originalFrameIVC);
	vc_image_free(workedFrameIVC);
	vc_image_free(binFrame1IVC);
	vc_image_free(binFrame2IVC);
	free(frameOrig);
	free(blobs);
	#pragma endregion

	return EXIT_SUCCESS;
}