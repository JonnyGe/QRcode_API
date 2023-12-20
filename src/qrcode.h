#ifndef __QRCODE_H__
#define __QRCODE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************************************************************************
*                                    Macro Definition Section                                                        *
*********************************************************************************************************************/
/**
 * version:1-40
 */
#define QRCODE_VERSION_1			1
#define QRCODE_VERSION_2			2
#define QRCODE_VERSION_3			3
#define QRCODE_VERSION_4			4
#define QRCODE_VERSION_5			5
#define QRCODE_VERSION_6			6
#define QRCODE_VERSION_7			7
#define QRCODE_VERSION_8			8
#define QRCODE_VERSION_9			9
#define QRCODE_VERSION_10			10
#define QRCODE_VERSION_11			11
#define QRCODE_VERSION_12			12
#define QRCODE_VERSION_13			13
#define QRCODE_VERSION_14			14
#define QRCODE_VERSION_15			15

/**
 * 二维码生成图片放大倍数,倍数越大图片空间越大
 */
#define PIXEL_PRESCALER_1			1
#define PIXEL_PRESCALER_2			2
#define	PIXEL_PRESCALER_3			3
#define	PIXEL_PRESCALER_4			4
#define	PIXEL_PRESCALER_5			5
#define	PIXEL_PRESCALER_6			6
#define	PIXEL_PRESCALER_7			7
#define	PIXEL_PRESCALER_8			8



/*********************************************************************************************************************
*                                    Type Definition Section                                                         *
*********************************************************************************************************************/
class GenerateQRCode
{
public:
    /**
     * Encoding mode.
     */
    enum QRencodeMode{
        QR_MODE_NUL = -1,  ///< Terminator (NUL character). Internal use only
        QR_MODE_NUM = 0,   ///< Numeric mode
        QR_MODE_AN,        ///< Alphabet-numeric mode
        QR_MODE_8,         ///< 8-bit data mode ,ofen use mode
        QR_MODE_KANJI,     ///< Kanji (shift-jis) mode  ,can use Chinese
        QR_MODE_STRUCTURE, ///< Internal use only
        QR_MODE_ECI,       ///< ECI mode
        QR_MODE_FNC1FIRST,  ///< FNC1, first position
        QR_MODE_FNC1SECOND, ///< FNC1, second position
    } ;

    /**
     * Level of error correction.
     */
    enum QRecLevel{
        QR_ECLEVEL_L = 0, ///< lowest
        QR_ECLEVEL_M,
        QR_ECLEVEL_Q,
        QR_ECLEVEL_H      ///< highest
    } ;

public:
    /**
     * Generate QRCode Image File.
     * @param str_src input str.
     * @param pFilePath out Generate QR Image file path.
     * @param version intput QR version.
     * @param iPrescaler intput image multiple.
     * @param level intput Level of error correction.
     * @param encodeMode intput Encoding mode.
     * @return true Generate success.
     * @return false Generate failed.
     */
    bool GenerateQRCodeImageFile(char *str_src,char *pFilePath,int version= QRCODE_VERSION_6,int iPrescaler=PIXEL_PRESCALER_5,QRecLevel level=QR_ECLEVEL_Q,QRencodeMode encodeMode=QR_MODE_8);

    /**
     * Generate QRCode Image Data.
     * @param str_src input str.
     * @param pImageData out Generate QR Image data.
     * @param npImageLen out QR Image length
     * @param version intput QR version.
     * @param iPrescaler intput image multiple.
     * @param level intput Level of error correction.
     * @param encodeMode intput Encoding mode.
     * @return true Generate success.
     * @return false Generate failed.
     */
    bool GenerateQRCodeImageData(char *str_src,unsigned char **pImageData,unsigned long &npImageLen,int version= QRCODE_VERSION_6,int iPrescaler=PIXEL_PRESCALER_5,QRecLevel level=QR_ECLEVEL_Q,QRencodeMode encodeMode=QR_MODE_8);

    //return version size
    int  GetQRCodeVesionSize(int verison);

private:


};


#endif /* __QRCODE_H__ */
