#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_write.h"


#include <math.h>
#include <stdio.h>
#include <float.h>


void mirror_transform(unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void grayScale_transform(unsigned char* in, int const height, int const width, int const channel, unsigned char* out);
void sobelFiltering_transform(unsigned char* in, int const height, int const width, int const channel, unsigned char* out);

int main()
{

	int height;
	int width;
	int channel;

	char command;/*
	printf("Take a picture? (y/n)\n");
	scanf("%c", &command);

	if (command == 'n')
		exit(1);


	printf("Cheeze !\r\n");
	system("raspistill -w 640 -h 480 -t 10 -o image.bmp");
	*/

	unsigned char* imgIn = stbi_load("test_image.bmp", &width, &height, &channel, 3);


	unsigned char* imgOut_mirror = (unsigned char*)malloc(sizeof(unsigned char) * 3 * 640 * 480);
	unsigned char* imgOut_grayScale = (unsigned char*)malloc(sizeof(unsigned char) * 3 * 640 * 480);
	unsigned char* imgOut_sobelFiltering = (unsigned char*)malloc(sizeof(unsigned char) * 3 * 640 * 480);

	mirror_transform(imgIn, height, width, channel, imgOut_mirror);
	grayScale_transform(imgIn, height, width, channel, imgOut_grayScale);
	sobelFiltering_transform(imgOut_grayScale, height, width, channel, imgOut_sobelFiltering);


	stbi_write_bmp("image_mirror.bmp", width, height, channel, imgOut_mirror);
	stbi_write_bmp("image_grayScale.bmp", width, height, channel, imgOut_grayScale);
	stbi_write_bmp("image_sobelFiltering.bmp", width, height, channel, imgOut_sobelFiltering);

	stbi_image_free(imgIn);
	free(imgOut_mirror);
	free(imgOut_grayScale);
	free(imgOut_sobelFiltering);


	return 0;
}

void mirror_transform(unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < channel; k++) {
				out[i * width * channel + (width - 1 - j) * channel + k] = in[i * width * channel + j * channel + k];
			}
		}
	}
}

void grayScale_transform(unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			for (int k = 0; k < channel; k++) {
				out[i * width * channel + j * channel + k] = (in[i * width * channel + j * channel] + in[i * width * channel + j * channel + 1] + in[i * width * channel + j * channel + 2]) / 3;
			}
		}
	}
}

void sobelFiltering_transform(unsigned char* in, int const height, int const width, int const channel, unsigned char* out) {


	for (int i = 1; i < (height - 1); i++) {
		for (int j = 1; j < (width - 1); j++) {
			for (int k = 0; k < channel; k++) {
				int x; int y;
				x = -in[(i - 1) * width * channel + (j - 1) * channel + k] + in[(i + 1) * width * channel + (j - 1) * channel + k]
					- 2 * in[(i - 1) * width * channel + j * channel + k] + 2 * in[(i + 1) * width * channel + j * channel + k]
					- in[(i - 1) * width * channel + (j + 1) * channel + k] + in[(i + 1) * width * channel + (j + 1) * channel + k];

				y = in[(i - 1) * width * channel + (j - 1) * channel + k] - in[(i - 1) * width * channel + (j + 1) * channel + k]
					+ 2 * in[i * width * channel + (j - 1) * channel + k] - 2 * in[i * width * channel + (j + 1) * channel + k]
					+ in[(i + 1) * width * channel + (j - 1) * channel + k] - in[(i + 1) * width * channel + (j + 1) * channel + k];

				out[i * width * channel + j * channel + k] = abs(x) + abs(y);
			}
		}
	}

	for (int j = 0; j < width; j++) {
		for (int k = 0; k < channel; k++) {
			out[j * channel + k] = 0; out[(height - 1) * width * channel + j * channel + k] = 0;
		}
	}

	for (int i = 0; i < width; i++) {
		for (int k = 0; k < channel; k++) {
			out[i * width * channel + k] = 0; out[i * width * channel + (width - 1) * channel + k] = 0;
		}
	}
}
