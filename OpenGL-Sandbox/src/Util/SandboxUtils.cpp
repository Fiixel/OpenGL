#include "SandboxUtils.h"

#include "../../vendor/stb/stb_image_write.h"
#include "../vendor/stb_image/stb_image.h"
#include "../../vendor/qrtopng/QrToPng.h"

#include <memory>

bool SandboxUtils::LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

void SandboxUtils::BasicQRDemo()
{
	const char* text = "Hello, world!";
	const qrcodegen::QrCode::Ecc errCorLv1 = qrcodegen::QrCode::Ecc::MEDIUM;

	const qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(text, errCorLv1);
	printQR(qr);
}

void SandboxUtils::ImageWriteTest(unsigned char w, unsigned char h, unsigned char channels_num)
{
	auto data = std::make_unique<unsigned char[]>(w * h * channels_num);

	int index = 0;
	for (int j = h - 1; j >= 0; --j)
	{
		for (int i = 0; i < w; ++i)
		{
			data[index++] = (unsigned char)(255.0 * i / w);
			data[index++] = (unsigned char)(255.0 * j / h);
			data[index++] = (unsigned char)(255.0 * 0.2);
		}
	}

	{
		unsigned char* tmpdata = data.get();
		stbi_write_jpg("jpg_test.jpg", w, h, channels_num, tmpdata, w * channels_num);
	}

	LOG_INFO("Wrote Image");
}

void SandboxUtils::QRImageWriteTest()
{
	std::string qrText = "Hello World!";
	std::string filename = "qrcode.png";

	int imgSize = 300;
	int minModulePixelSize = 3;
	QrToPng exampleQrPng1 = QrToPng(filename, imgSize, minModulePixelSize, qrText, true, qrcodegen::QrCode::Ecc::MEDIUM);

	std::cout << "Writing Example QR code 1 (normal) to " << filename << " with text: '" << qrText << "', size: " <<
		imgSize << "x" << imgSize << ", qr module pixel size: " << minModulePixelSize << ". " << std::endl;

	if (exampleQrPng1.writeToPNG())
		std::cout << "Success!" << std::endl;
	else
		std::cerr << "Failure..." << std::endl;
}

void SandboxUtils::printQR(const qrcodegen::QrCode& qr)
{
	int border = 4;
	for (int y = -border; y < qr.getSize() + border; y++) {
		for (int x = -border; x < qr.getSize() + border; x++) {
			std::cout << (qr.getModule(x, y) ? "##" : "  ");
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}