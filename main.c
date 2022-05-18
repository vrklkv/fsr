#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

int main(){
    // строка, путь к файлу
    char* inputPath="hamster.png";
    int iw, ih, n, i, j, k=0;

    // Загружаем изображение, чтобы получить информацию о ширине, высоте и цветовом канале
    unsigned char *idata=stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata==NULL){
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }

    //Переходим от трёхканального изображения к одноканальному
    unsigned char* od=(unsigned char*)malloc(iw*ih*sizeof(unsigned char));
    for (i=0; i<ih*iw*n; i=i+n){
        od[k]=0.34375*idata[i]+0.5*idata[i+1]+0.15625*idata[i+2];
        k++;  
    }


    for (i=2;i<=ih-2;i++){
        for (j=2;j<=iw-2;j++){
            if(od[iw*i+j]>155)od[iw*i+j]=255;
            if(od[iw*i+j]<92)od[iw*i+j]=0;
        }
    }


    //Гауссово размытие
    for (i=2;i<=ih-2;i++){
        for (j=2;j<iw-2;j++){
            od[iw*i+j]=0.0924*od[iw*(i-1)+(j-1)]+0.01192*od[iw*(i-1)+(j)]+0.0924*od[iw*(i-1)+(j+1)]+0.1192*od[iw*(i)+(j-1)]+0.1538*od[iw*(i)+(j)]+0.1192*od[iw*(i)+(j+1)]+0.0924*od[iw*(i+1)+(j-1)]+0.1192*od[iw*(i+1)+(j)]+0.0924*od[iw*(i+1)+(j+1)];
        }
    }


    char* outputPath = "output.png";
    // записываем картинку
    stbi_write_png(outputPath, iw, ih, 1, od, 0);
    stbi_image_free(idata);
    stbi_image_free(od);
    return 0;
}
