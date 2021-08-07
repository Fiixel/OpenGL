#pragma once

#include "GLCore.h"

#include "../../vendor/QRCode/QrCode.hpp"

class SandboxUtils
{
public:
	static bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);

	static void BasicQRDemo();
	static void ImageWriteTest(unsigned char w, unsigned char h, unsigned char channels_num);
	static void QRImageWriteTest();
	static void printQR(const qrcodegen::QrCode& qr);
};