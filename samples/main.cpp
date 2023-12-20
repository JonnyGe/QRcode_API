
#include "qrcode.h"
#include <string>

char *str0 ="si";
char *str1 = "http://we.qq.com/d/AQCEfRa4gW1JPSw-RMYzdiHxPBNabMrqQ68k_dR1";
char *str2 ="http://www.baidu.com";
char *str3 = "http://www.baidu.com/s?wd=asdada&rsv_spt=1&issp=1&rsv_bp=0&ie=utf-8&tn=baiduhome_pg&f=3&inputT=2780";

int main(int argc, char **argv)
{
    GenerateQRCode a;
    char *InImagePathName= "./QRtest11.bmp";
    printf("GetQRCodeVesionSize Max is %d\n",a.GetQRCodeVesionSize(QRCODE_VERSION_6));

    bool bret= false;
    bret=a.GenerateQRCodeImageFile(str2,InImagePathName,QRCODE_VERSION_6);
    if(bret)
    {
        printf("Generate QRCodeImage is finish!\n");
    }
    else
    {
        printf("Generate QRCodeImage is failed!\n");
    }


    unsigned char *pImageData = NULL;
    unsigned long npImageLen= 0;
    bool bret2= false;
    bret2= a.GenerateQRCodeImageData(str2,&pImageData,npImageLen);
    if(bret2)
    {
        printf("Generate QRCodeImage22 is finish!\n");
        printf("%s,Line:%d,npImageLen[%d]\n",__FUNCTION__,__LINE__,npImageLen);
    }

    if(pImageData)  //if need delete
    {
        delete [] pImageData;
        pImageData =NULL;
    }

    printf("main finish!\n");
    return 0;
}


