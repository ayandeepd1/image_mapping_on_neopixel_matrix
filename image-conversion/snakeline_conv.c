#pragma pack(1)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bmp.h"

PBITMAPFILEHEADER file_header;
PBITMAPINFOHEADER info_header;

int no_files=10;

char foldername[] = "bmpouts/bmp5/"; //
char extension[] = ".bmp";


int limit=2700;
uint8_t buffer[8214];
uint8_t pix_matrix[8100];
uint8_t file_buff[2700];

void file_read(char file_path[]) {
    FILE *fp;
    
    fp=fopen(file_path, "rb");
    printf("reading: %s\n", file_path);
    fread(buffer, 1, 8214, fp);
    fclose(fp);
    
    file_header = (PBITMAPFILEHEADER)(&buffer[0]);
    info_header = (PBITMAPINFOHEADER)(&buffer[0] + sizeof(BITMAPFILEHEADER));
    printf("pixel data offset: %d\n", file_header->bfOffBits);
    
}

void reverse_fill(){
    int i=54;
    for(int y=29; y>-1; --y){
        for(int x=0; x<90; ++x){
            pix_matrix[3*(y*90+x)+2]= buffer[i++];
            pix_matrix[3*(y*90+x)+1]= buffer[i++];
            pix_matrix[3*(y*90+x)+0]= buffer[i++];
            //printf(":%d,%d,%d\n", pix_matrix[3*(y*90+x)+0], pix_matrix[3*(y*90+x)+1], pix_matrix[3*(y*90+x)+2]);    
        }
        i+=2;
    }
    
}

void file_write(char f_no[]){
    int count;
    char dx[strlen(f_no)+2];
    FILE *file_ptr;

    count=0; 
    int i=1;
    for(int y=0; y<30; y++){
        if(y%2==0){
            for(int x=0; x<30; x++){
                //printf("%d:%d,%d,%d\n", i++, pix_matrix[3*(y*90+x)+0], pix_matrix[3*(y*90+x)+1], pix_matrix[3*(y*90+x)+2]);       
                file_buff[count++] = pix_matrix[3*(y*90+x)+0];
                file_buff[count++] = pix_matrix[3*(y*90+x)+1];
                file_buff[count++] = pix_matrix[3*(y*90+x)+2];
            }
        }
        else{
            for(int x=29; x>-1; x--){
                file_buff[count++] = pix_matrix[3*(y*90+x)+0];
                file_buff[count++] = pix_matrix[3*(y*90+x)+1];
                file_buff[count++] = pix_matrix[3*(y*90+x)+2];
            }
        }
    }
    
    strcpy(dx, "s1/");
    strcat(dx, f_no);
    
    file_ptr=fopen(dx, "wb");
   
    fwrite(file_buff, 1, limit, file_ptr);
    fclose(file_ptr);
    printf("wrote %s\t", dx);
    
    count=0;
    for(int y=0; y<30; y++){
        if(y%2==0){
            for(int x=30; x<60; x++){
                //printf("%d:%d,%d,%d\n", count++, pix_matrix[3*(y*90+x)+0], pix_matrix[3*(y*90+x)+1], pix_matrix[3*(y*90+x)+2]);       
                file_buff[count++] = pix_matrix[3*(y*90+x)+0];
                file_buff[count++] = pix_matrix[3*(y*90+x)+1];
                file_buff[count++] = pix_matrix[3*(y*90+x)+2];
            }
        }
        else{
            for(int x=59; x>29; x--){
                //printf("%d:%d,%d,%d\n", count++, pix_matrix[3*(y*90+x)+0], pix_matrix[3*(y*90+x)+1], pix_matrix[3*(y*90+x)+2]);       
                file_buff[count++] = pix_matrix[3*(y*90+x)+0];
                file_buff[count++] = pix_matrix[3*(y*90+x)+1];
                file_buff[count++] = pix_matrix[3*(y*90+x)+2];
            }
        }
    }
    
    dx[1]='2';
    file_ptr=fopen(dx, "wb");
    
    fwrite(file_buff, 1, limit, file_ptr);
    
    fclose(file_ptr);
    printf("%s\t", dx);
    
    count=0;
    for(int y=0; y<30; y++){
        if(y%2==0){
            for(int x=60; x<90; x++){
                //printf("%d:%d,%d,%d\n", i++, pix_matrix[3*(y*90+x)+0], pix_matrix[3*(y*90+x)+1], pix_matrix[3*(y*90+x)+2]);       
                file_buff[count++] = pix_matrix[3*(y*90+x)+0];
                file_buff[count++] = pix_matrix[3*(y*90+x)+1];
                file_buff[count++] = pix_matrix[3*(y*90+x)+2];
            }
        }
        else{
            for(int x=89; x>59; x--){
                file_buff[count++] = pix_matrix[3*(y*90+x)+0];
                file_buff[count++] = pix_matrix[3*(y*90+x)+1];
                file_buff[count++] = pix_matrix[3*(y*90+x)+2];
            }
        }
    }
    
    dx[1]='3';
    
    file_ptr=fopen(dx, "wb");
    fwrite(file_buff, 1, limit, file_ptr);
    fclose(file_ptr);
    printf("%s\n\n", dx);
}

int main() {
    
    for(int i=1; i<=no_files; i++){
        int ic=i;
        int index=1;

        char filename_base[] = "00000001";
        while (ic>0){
            filename_base[8-index++] = '0' + (ic%10);
            ic/=10;
        }

        char filename[strlen(foldername) + strlen(filename_base) + strlen(extension)];
        strcpy(filename, foldername);
        strcat(filename, filename_base);
        strcat(filename, extension);
        
        file_read(filename);
        reverse_fill();
        file_write(filename_base);
        
    }
    
    return 0;
}