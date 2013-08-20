/**
 * @file imageIO.h
 *
 * @author take
 */

#ifndef IMAGEIO_H_
#define IMAGEIO_H_

#include "structDef.h"

namespace ImageIO {
void gammaCorrection(RawImage& img,float gamma);
int ccFtoI(float c);
int writePPM(const char* fileName,const RawImage& img);
int readHDR(const char* fileName,RawImage** img);

int readLine(const char* buff,int off,char* str,int len);
int readBRDF(const char* fileName,RawImage** img);
int readRawData(const char* fileName,RawImage** img);
}  // namespace ImageIO


#endif /* IMAGEIO_H_ */
