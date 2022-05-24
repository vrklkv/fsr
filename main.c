#define STBI_NO_SIMD
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>

int exists(int i,int j,int iw,int ih){
    if((i>=1)&&(i<=ih-1)&&(j>=1)&&(j<=iw-1))return 1;
    return 0;
}

void dfs(int i,int j,int iw,int ih,unsigned char* dip, int* v,int col_num){
    v[iw*i+j]=col_num;
    if(exists(i,j-2,iw,ih)==1){
        if((abs(dip[iw*i+j]-dip[iw*(i)+(j-2)])<=63)&&(v[iw*(i)+(j-2)]==0)){
            dfs(i,j-2,iw,ih,dip,v,col_num);
        }
    }
    if(exists(i-2,j+1,iw,ih)==1){
        if((abs(dip[iw*i+j]-dip[iw*(i-2)+(j+1)])<=63)&&(v[iw*(i-2)+(j+1)]==0)){
            dfs(i-2,j+1,iw,ih,dip,v,col_num);
        }
    }
    /*
    if(exists(i+1,j-1,iw,ih)==1){
        if((abs(dip[iw*i+j]-dip[iw*(i+1)+(j-1)])<=5)&&(v[iw*(i+1)+(j-1)]==0)){
            dfs(i+1,j-1,iw,ih,dip,v,col_num);
        }
    }
    */
    if(exists(i+2,j+1,iw,ih)==1){
        if((abs(dip[iw*i+j]-dip[iw*(i+2)+(j+1)])<=63)&&(v[iw*(i+2)+(j+1)]==0)){
            dfs(i+2,j+1,iw,ih,dip,v,col_num);
        }
    }
    return;
}


int main(){
    // строка, путь к файлу
    char* inputPath="hamster.png";
    int iw, ih, n, i, j, k=0,col_num=0,check=0;

    // Загружаем изображение, чтобы получить информацию о ширине, высоте и цветовом канале
    unsigned char *idata=stbi_load(inputPath, &iw, &ih, &n, 0);
    if (idata==NULL){
        printf("ERROR: can't read file %s\n", inputPath );
        return 1;
    }
    
    printf("iw=%d ih=%d n=%d\n",iw,ih,n);

    //Переходим от четырёхканального изображения к одноканальному
    unsigned char* dip=(unsigned char*)malloc(iw*ih*sizeof(unsigned char));       //dip - data in processing
    for (i=0; i<ih*iw*n; i=i+n){
        dip[k]=0.34375*idata[i]+0.5*idata[i+1]+0.15625*idata[i+2];
        k++;  
    }

    for (i=2;i<=ih-2;i++){  //Повышаем контрастность
        for (j=2;j<=iw-2;j++){
            if(dip[iw*i+j]>155)dip[iw*i+j]=255;
            if(dip[iw*i+j]<92)dip[iw*i+j]=0;
        }
    }
    
    printf("contrast increased\n");

    int* v=(int*)malloc((iw*ih)*sizeof(int));
    for(i=0;i<iw*ih;i++)v[i]=0;

    printf("v is created\n");

    for (i=1;i<=ih-1;i++){
        for (j=1;j<=iw-1;j++){
            if(v[iw*i+j]==0){
                col_num++;
                //printf("start DFS %d %d\n",i,j);
                dfs(i,j,iw,ih,dip,v,col_num);
            }
        }
    } 

    printf("OK DFS\n");

    unsigned char* odata=(unsigned char*)malloc(iw*ih*n*sizeof(unsigned char));
    k=0;int coef;
    for (i=0; i<ih*iw*n; i=i+n){
        coef=v[k]%50+v[k]%12;
        odata[i]=4*coef-70;
        odata[i+1]=3*coef+40;
        odata[i+2]=5*coef+3;
        odata[i+3]=255;
        k++;  
    }

    printf("OK odata\n");

    char* outputPath = "output.png";
    // записываем картинку
    stbi_write_png(outputPath, iw, ih, n, odata, 0);
    stbi_image_free(idata);
    stbi_image_free(dip);
    stbi_image_free(odata);
    return 0;
}
