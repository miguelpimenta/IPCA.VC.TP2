#ifndef VC_H
#define VC_H

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    VISÃO POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define VC_DEBUG

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct {
	unsigned char *data;
	int width, height;
	int channels;			// Binário/Cinzentos=1; RGB=3
	int levels;				// Binário=1; Cinzentos [1,255]; RGB [1,255]
	int bytesperline;		// width * channels
} IVC;

typedef struct {
	int x, y, width, height;	// Caixa Delimitadora (Bounding Box)
	int area;					// �rea
	int xc, yc;					// Centro-de-massa
	int perimeter;				// Per�metro
	int label;					// Etiqueta
} OVC;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROTÓTIPOS DE FUNÇÕES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// FUNÇÕES: ALOCAR E LIBERTAR UMA IMAGEM
IVC *vc_image_new(int width, int height, int channels, int levels);
IVC *vc_image_free(IVC *image);

// FUNÇÕES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC *vc_read_image(char *filename);
int vc_write_image(char *filename, IVC *image);

// BLOBS
OVC* vc_binary_blob_labelling(IVC *src, IVC *dst, int *nlabels);
int vc_binary_blob_info(IVC *src, OVC *blobs, int nblobs);

int vc_gray_negative(IVC *srcdst);
int vc_img_to_gray(IVC *srcImg, IVC *dstImg);
int vc_rgb_to_gray(IVC *srcImg, IVC *dstImg);
int vc_rgb_to_hsv(IVC * srcdst);
int vc_gray_to_binary(IVC * srcdst, int threshold);
int vc_scale_gray_to_rgb(IVC *src, IVC *dst);

int vc_binary_dilate(IVC *srcImg, IVC *dstImg, int kernelSize);
int vc_binary_erode(IVC *srcImg, IVC *dstImg, int kernelSize);
int vc_binary_open(IVC *src, IVC *dst, int sizeErode, int size);
int vc_binary_close(IVC *src, IVC *dst, int sizeErode, int size);

int vc_gray_dilate(IVC *src, IVC *dst, int kernel);
int vc_gray_erode(IVC *src, IVC *dst, int kernel);

int vc_gray_open(IVC *src, IVC *dst, int sizeerode, int sizedilate);
int vc_gray_close(IVC *src, IVC *dst, int sizedilate, int sizeerode);


#endif
