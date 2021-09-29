#pragma once

#include<string>
#include <stdio.h>
#include <stdlib.h>
#include <pshpack2.h>
#include <glad/glad.h>
#include "../config/user_settings.h"


class ExportMedia
{

public:
    typedef long LONG;
    typedef unsigned char BYTE;
    typedef unsigned long DWORD;
    typedef unsigned short WORD;

    typedef struct {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
    } BMPFILEHEADER_T;

    struct BMPFILEHEADER_S {
        WORD    bfType;
        DWORD   bfSize;
        WORD    bfReserved1;
        WORD    bfReserved2;
        DWORD   bfOffBits;
    };

    typedef struct {
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
    } BMPINFOHEADER_T;


    static void Snapshot(BYTE* pData, int width, int height, const char* filename)
    {
        int size = width * height * 3; // 每个像素点3个字节

        // 位图第一部分，文件信息
        BMPFILEHEADER_T bfh;
        bfh.bfType = 0x4d42;  //bm
        bfh.bfSize = size  // data size
            + sizeof(BMPFILEHEADER_T) // first section size
            + sizeof(BMPINFOHEADER_T) // second section size
            ;
        bfh.bfReserved1 = 0; // reserved 
        bfh.bfReserved2 = 0; // reserved
        bfh.bfOffBits = bfh.bfSize - size;



        // 位图第二部分，数据信息
        BMPINFOHEADER_T bih;
        bih.biSize = sizeof(BMPINFOHEADER_T);
        bih.biWidth = width;
        bih.biHeight = height;
        bih.biPlanes = 1;
        bih.biBitCount = 24;
        bih.biCompression = 0;
        bih.biSizeImage = size;
        bih.biXPelsPerMeter = 0;
        bih.biYPelsPerMeter = 0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;

        FILE* fp;
        fopen_s(&fp, filename, "wb");
        if (!fp) return;

        fwrite(&bfh, 1, sizeof(BMPFILEHEADER_T), fp);

        fwrite(&bih, 1, sizeof(BMPINFOHEADER_T), fp);

        fwrite(pData, 1, size, fp);

        fclose(fp);

    }

    static void ExportImage(uint16_t width, uint16_t height) {  
        GLubyte* bits;//定义指向位图数据的指针
        bits = (unsigned char*)malloc(width * height * 3);//为位图分配内存空间并赋值给bits
        glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, bits);//从帧缓存中读取位图数据
        Snapshot((BYTE*)bits, width, height, "snapshot.bmp");// 生成24位BMP图片
    }

};

