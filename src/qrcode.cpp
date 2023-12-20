
/*********************************************************************************************************************
*                                    Include File Section                                                            *
*********************************************************************************************************************/

#include "qrencode.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "qrcode.h"

/*********************************************************************************************************************
*                                    Macro Definition Section                                                        *
*********************************************************************************************************************/
#define BI_RGB			0L

#define PIXEL_COLOR_R				0x00000008// Color of bmp pixels
#define PIXEL_COLOR_G				0x00000008
#define PIXEL_COLOR_B				0x00000008

/**
 * return value
 */
#define QR_SUCCESS  0
#define QR_FAIL     -1


/*********************************************************************************************************************
*                                    Type Definition Section                                                         *
*********************************************************************************************************************/

#pragma pack(push, 2)

typedef struct
{
    unsigned short  bfType;
    unsigned long   bfSize;
    unsigned short  bfReserved1;
    unsigned short  bfReserved2;
    unsigned long   bfOffBits;
} BITMAPFILEHEADER;

typedef struct
{
    unsigned long     biSize;
    signed long       biWidth;
    signed long       biHeight;
    unsigned short    biPlanes;
    unsigned short    biBitCount;
    unsigned long     biCompression;
    unsigned long     biSizeImage;
    signed long       biXPelsPerMeter;
    signed long       biYPelsPerMeter;
    unsigned long     biClrUsed;
    unsigned long     biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

/*********************************************************************************************************************
*                                    Global Variables Definition Section                                             *
*********************************************************************************************************************/

/*********************************************************************************************************************
*                                    Functions SourceCode Implement Section                                          *
*********************************************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

    int GenerateQRCodeImage(char* src,unsigned char **bmp,unsigned long &bmp_size,int version,int iPrescaler,int level,int encodeMode)
    {
        unsigned int	unWidth, unWidthAdjusted, unDataBytes;
        unsigned int	x, y, l, n;
        unsigned char	*pRGBData,*pSourceData, *pDestData;
        QRcode*			pQRC;

        // Compute QRCode
        if (pQRC = QRcode_encodeString(src, version, (QRecLevel)level, (QRencodeMode)encodeMode, 1))
        {
            unWidth = pQRC->width;
//            printf("QRcode:%s,Line:%d,pQRC->width[%d]\n",__FUNCTION__,__LINE__,unWidth);
            unWidthAdjusted = (unWidth) * iPrescaler * 3; //单行所占字节数
            if (unWidthAdjusted % 4)
            {
                unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
            }
            unDataBytes = unWidthAdjusted * (unWidth) * iPrescaler;

            // Allocate pixels buffer
            if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
            {
                QRcode_free(pQRC);
                printf("QRcode:%s,Line:%d,malloc pixels buffer is failed!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
                return QR_FAIL;
            }

            // Preset to white
            memset(pRGBData, 0xff, unDataBytes);

            // Prepare bmp headers
            BITMAPFILEHEADER kFileHeader;
            kFileHeader.bfType = 0x4d42;  // "BM"
            kFileHeader.bfSize =	sizeof(BITMAPFILEHEADER) +
                sizeof(BITMAPINFOHEADER) +
                unDataBytes;
            kFileHeader.bfReserved1 = 0;
            kFileHeader.bfReserved2 = 0;
            kFileHeader.bfOffBits =	sizeof(BITMAPFILEHEADER) +
                sizeof(BITMAPINFOHEADER);

            BITMAPINFOHEADER kInfoHeader;
            kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
            kInfoHeader.biWidth = (unWidth) * iPrescaler;
            kInfoHeader.biHeight = -((int)(unWidth) * iPrescaler);
            kInfoHeader.biPlanes = 1;
            kInfoHeader.biBitCount = 24;
            kInfoHeader.biCompression = BI_RGB;
            kInfoHeader.biSizeImage = 0;
            kInfoHeader.biXPelsPerMeter = 0;
            kInfoHeader.biYPelsPerMeter = 0;
            kInfoHeader.biClrUsed = 0;
            kInfoHeader.biClrImportant = 0;

            // Convert QrCode bits to bmp pixels
            pSourceData = pQRC->data;
            for(y = 0; y < unWidth; y++)
            {
                pDestData = pRGBData + unWidthAdjusted * y * iPrescaler;
                for(x = 0; x < unWidth; x++)
                {
                    if (*pSourceData & 1)
                    {
                        for(l = 0; l < iPrescaler; l++)
                        {
                            for(n = 0; n < iPrescaler; n++)
                            {
                                *(pDestData +		n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_R;
                                *(pDestData + 1 +	n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_G;
                                *(pDestData + 2 +	n * 3 + unWidthAdjusted * l) =	PIXEL_COLOR_B;
                            }
                        }
                    }
                    pDestData += 3 * iPrescaler;
                    pSourceData++;
                }
            }

            //entire bmp struct
            bmp_size = sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER)+unDataBytes;
            *bmp = (unsigned char*)malloc(bmp_size);
//            printf("QRcode:%s,Line:%d,bmp pointer[%p]\n",__FUNCTION__,__LINE__,*bmp);

            if(!*bmp)
            {
                QRcode_free(pQRC);
                free(pRGBData);
                printf("QRcode:%s,Line:%d,malloc bmp is failed222!!!!!!!!!!!!!\n",__FUNCTION__,__LINE__);
                return QR_FAIL;
            }

            memcpy(*bmp, &kFileHeader, sizeof(BITMAPFILEHEADER));
            memcpy(*bmp+sizeof(BITMAPFILEHEADER), &kInfoHeader, sizeof(BITMAPINFOHEADER));
            memcpy(*bmp+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER), pRGBData, unDataBytes);

            // Free data
            free(pRGBData);
            QRcode_free(pQRC);

            return QR_SUCCESS;
        }
        else
        {
            printf("QRcode:QRcode_encodeString failed!!!\n");
            return QR_FAIL;
        }
    }


    void DestroyQRCodeImage(unsigned char* dst)
    {
        if(NULL != dst)
        {
            free(dst);
        }
    }

#ifdef __cplusplus
}
#endif


bool GenerateQRCode::GenerateQRCodeImageFile(char *str_src,char *pFilePath,int version,int iPrescaler,QRecLevel level,QRencodeMode encodeMode)
{
    if(!str_src)
    {
        printf("QRcode:GenerateQRCode::%s,Line:%d,str_src is NULL!\n",__FUNCTION__,__LINE__);
        return false;
    }
    printf("QRcode:vesion[%d],iPrescaler[%d],QRecLevel[%d],QRencodeMode[%d]\n",version,iPrescaler,level,encodeMode);
    printf("QRcode:GenerateQRCode::%s,Line:%d,str_src length[%d]\n",__FUNCTION__,__LINE__,strlen(str_src));

    if(strlen(str_src)>GetQRCodeVesionSize(version))
    {
         printf("QRcode:%s,Line:%d,str length is beyond capacity!!!\n",__FUNCTION__,__LINE__);
         return false;
    }

    bool bret = false;
    unsigned char* bmp = NULL;
    unsigned long bmp_size= 0;
    int nRet=GenerateQRCodeImage(str_src,&bmp,bmp_size,version,iPrescaler,level,encodeMode);
    if(nRet== QR_SUCCESS)
    {
//        printf("QRcode:QR_SUCCESS,bmp_size[%d]\n",bmp_size);
//        printf("QRcode:%s,Line:%d,bmp pointer[%p]\n",__FUNCTION__,__LINE__,bmp);

        FILE *f;
        // Output the bmp file
        f = fopen(pFilePath, "wb+");
        if ( NULL == f )
        {
            printf("QRcode:open file error!!!\n");
        }
        else
        {
            int n=fwrite(bmp, sizeof(unsigned char), bmp_size, f);
            printf("QRcode:fwrite bmp_size[%d]\n",n);
            fclose(f);
            bret =true;
        }       
    }
    DestroyQRCodeImage(bmp);

    return bret;
}

bool GenerateQRCode::GenerateQRCodeImageData(char *str_src,unsigned char **pImageData,unsigned long &npImageLen,int version,int iPrescaler,QRecLevel level,QRencodeMode encodeMode)
{   
    if(!str_src)
    {
        printf("QRcode:GenerateQRCode::%s,Line:%d,str_src is NULL!\n",__FUNCTION__,__LINE__);
        return false;
    }
    printf("QRcode:vesion[%d],iPrescaler[%d],QRecLevel[%d],QRencodeMode[%d]\n",version,iPrescaler,level,encodeMode);
    printf("QRcode:GenerateQRCode::%s,Line:%d,str_src length[%d]\n",__FUNCTION__,__LINE__,strlen(str_src));

    if(strlen(str_src)>GetQRCodeVesionSize(version))
    {
         printf("QRcode:%s,Line:%d,str length is beyond capacity!!!\n",__FUNCTION__,__LINE__);
         return false;
    }

    bool bret = false;
    unsigned char* bmp = NULL;
    int nRet=GenerateQRCodeImage(str_src,&bmp,npImageLen,version,iPrescaler,level,encodeMode);
    if(nRet== QR_SUCCESS)
    {
        printf("QRcode:npImageLen[%d]\n",npImageLen);
        *pImageData = new unsigned char[npImageLen]();
        memcpy(*pImageData, bmp, npImageLen);
        bret=true;
//        printf("QRcode:%s,Line:%d,bmp pointer[%p]\n",__FUNCTION__,__LINE__,*pImageData);
    }
    DestroyQRCodeImage(bmp);

    return bret;
}

/**
 * QR_ECLEVEL_Q && QR_MODE_8
 */
int GenerateQRCode::GetQRCodeVesionSize(int verison)
{
    verison= verison< 1?1:verison;
    switch (verison)
    {
    case 1:
        return 11;
        break;
    case 2:
        return 20;
        break;
    case 3:
        return 32;
         break;
    case 4:
        return 46;
         break;
    case 5:
        return 60;
         break;
    case 6:
        return 74;
        break;
    case 7:
        return 86;
         break;
    case 8:
        return 108;
         break;
    case 9:
        return 130;
        break;
    case 10:
        return 151;
        break;
    case 11:
        return 177;
        break;
    case 12:
        return 203;
        break;
    case 13:
        return 241;
        break;
    case 14:
        return 258;
        break;
    case 15:
        return 290;
        break;

    defalut:
        return 300;
        break;
    }
}
