#include "defs.h"

int main(int argc, char *argv[]){
	if(argc<2){
					printf("usage: imageflip <filename>\n");
					return 1;
	}


	image_t *image=NULL;
	image=loadImage(image,argv[1]);
	
	if(!image){
		fprintf(stderr, "Could not load image!");
		return 1;
	}

	int action = 0;

	while (action!=4){
		action = menu();
		switch(action) {
						case 1 :
										flipHorizontal(image);
						break;

						case 2:
										flipVertical(image);
						break;

						case 3: 
										purple(image);
						break;

						case 4: 
										writeImage(image, argv[1]);
						break;

						default:
										printf("Invalid input.\n");
						break;
		}
	}

	free(image->file);
	free(image->pixels);
	free(image);

	return 0;
}
