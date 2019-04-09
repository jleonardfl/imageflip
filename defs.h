#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>

typedef struct pixel {
	int r;
	int g;
	int b;
}pixel_t;

typedef struct header {
	char *identifier;
	int  width;
	int  height;
	int  pixelMax;
}header_t;

typedef struct image {
	pixel_t *pixels;
	header_t header;
	FILE  *file;
}image_t;

void loadHeader (image_t *img, char *id) {
	int width;
	int height;
	int pixelMax;
	fscanf(img->file, "%s %d %d %d", id,
                                   &(width),
																   &(height),
																   &(pixelMax));
	img->header.identifier=id;
	img->header.width=width;
	img->header.height=height;
	img->header.pixelMax=pixelMax;
}

void getHeader (image_t *img) {
	fprintf(stdout, "%s\n",     img->header.identifier);
	fprintf(stdout, "%d " ,     img->header.width);
	fprintf(stdout, "%d\n",     img->header.height);
	fprintf(stdout, "%d\n",     img->header.pixelMax);
}

void loadPixels (image_t *img) {
	int bufferSize=3*img->header.width * img->header.height;
	fgetc(img->file); //reads in newline char

	for(int i = 0; i<bufferSize; i+=3){
		unsigned int r=fgetc(img->file);
		unsigned int g=fgetc(img->file);
		unsigned int b=fgetc(img->file);
		(*((img->pixels)+(i/3)))=(pixel_t){r, g, b};
	}
}

void getPixels (image_t *img){
	for(int y=0; y<img->header.height; y++){
		for(int x=0; x<img->header.width; x++){
			printf("%d %d %d\n", (*(img->pixels+(img->header.width *y)+x)).r,
		                       (*(img->pixels+(img->header.width *y)+x)).g,
											     (*(img->pixels+(img->header.width *y)+x)).b);
		}
	}
}

image_t* loadImage (image_t *img, char *fileName){
	img = (image_t*)malloc(sizeof(image_t)
	                       +sizeof(pixel_t*)
											   +sizeof(FILE*));
	img->file=fopen(fileName, "r");

	if(!img->file){
		fprintf(stderr, "Could not load image!");
		return NULL;
	}

	img->header.identifier=(char*)malloc(sizeof(char)*4);
	loadHeader(img, img->header.identifier);

	img->pixels=(pixel_t*)malloc(sizeof(pixel_t)*
                     img->header.width*
									   img->header.height);
	loadPixels(img);
	return img;
}

void writeImage (image_t *img, char *path){
	FILE *fp = fopen(path, "w");
				
	// Write Header
	fprintf(fp, "%s\n", img->header.identifier);
	fprintf(fp, "%d %d\n", img->header.width, img->header.height);
	fprintf(fp, "%d\n", img->header.pixelMax);

	// Write Pixel Data
	int y;
	int x;
	for(y = 0; y<img->header.height; y++){
		for(x = 0; x<img->header.width; x++){
			pixel_t pix = (*(img->pixels+(img->header.width*y)+x));
			fprintf(fp, "%c%c%c", pix.r, pix.g, pix.b);
		}
	}
	
	fclose(fp);
}


void flipHorizontal (image_t *img){
	for(int y = 0; y<img->header.height; y++){
		for(int x = 0; x<(img->header.width/2); x++){
			int pos = (img->header.width*y)+x;
			int opposite = (img->header.width *y)+(img->header.width -x);

			pixel_t temp = (*(img->pixels+pos));
			(*(img->pixels+pos))=(*(img->pixels+(opposite)));
			(*(img->pixels+(opposite))) = temp;
		}
	}
}

void flipVertical (image_t *img){
  for(int y = 0; y<img->header.height/2; y++){
		for(int x = 0; x<img->header.width; x++){
			int pos = (img->header.width * y)+x;
			int opposite = img->header.height * img->header.width - pos;
			pixel_t temp = (*(img->pixels+pos));
			(*(img->pixels+pos)) = (*(img->pixels+(opposite)));
			(*(img->pixels+(opposite)))=temp;
		}
	}
}

void mirrorVertical (image_t *img){
  for(int y = 0; y<img->header.height/2; y++){
		for(int x = 0; x<img->header.width; x++){
			int pos = (img->header.width * y)+x;
			pixel_t temp = (*(img->pixels+pos));
			(*(img->pixels+pos)) = (*(img->pixels+(img->header.height*(img->header.width - y))+x));
			(*(img->pixels+(img->header.height*(img->header.height - y-1)+x)))=temp;
		}
	}
}

void purple (image_t *img){
				for(int y = 0; y<img->header.height; y++){
								for(int x = 0; x<img->header.width; x++){
												int pos = (img->header.width*y)+x;
												pixel_t temp = (*(img->pixels+pos));
												temp.g = 0;
												(*(img->pixels+pos))=temp;
								}
				}
}

int menu (){
				printf("1. Flip horizontally\n");
				printf("2. Flip vertically\n");
				printf("3. Make purple\n");
				printf("4. Save and exit\n\n");

				int choice;
				scanf("%d", &choice);
				printf("Choice: %d\n", choice);

				return choice;
}
