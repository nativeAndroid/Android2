#ifdef __ANDROID__
#ifndef PNG_H
	#include <jni/png.h>
#endif

struct ImageData {
	png_uint_32 img_width, img_height;
	int bit_depth, color_type;
	uint8_t* pixels;
};
static AAsset* ptrFile;

#elif  _WIN32
#ifndef __GLEW_H__
	#include <GL/GLEW.h>
#endif//__glew_h__
#ifndef _glfw3_h_
	#include <GLFW/glfw3.h>
#endif
 
#include <cstdio>
#include <cstdlib>
#include <jpeglib.h>
#include <jerror.h>
#endif

class Texture
{
private:
#ifdef __ANDROID__
	struct PNGImageData : public ImageData {
		PNGImageData(png_byte* pixels, int width, int height) {
			this->img_width = width;
			this->img_height = height;
			this->pixels = (uint8_t*)pixels;
		}
		virtual ~PNGImageData() {
			delete[] pixels;
		}
	};
	inline bool IsPowerOfTwo(int n)
	{
		// http://www.exploringbinary.com/ten-ways-to-check-if-an-integer-is-a-power-of-two-in-c/
		return (n & (n - 1)) == 0;
	}
	inline int NextPowerOfTwo(int n)
	{
		// Explanation : http://stackoverflow.com/questions/1322510/given-an-integer-how-do-i-find-the-next-largest-power-of-two-using-bit-twiddlin
		if (n < 0) {
			return 0;
		}
		n--;
		n |= n >> 1;   // Divide by 2^k for consecutive doublings of k up to 32,
		n |= n >> 2;   // and then or the results.
		n |= n >> 4;
		n |= n >> 8;
		n |= n >> 16;
		n++;           // The result is a number of 1 bits equal to the number
					   // of bits in the original number, plus 1. That's the
					   // next highest power of 2.
		return n;
	}

	struct ImageData* ptrImg;
#endif
private:
#ifdef __ANDROID__
	void LoadImage(AAssetManager* ptrAsset, const char * filename)
	{
		ptrFile = AAssetManager_open(ptrAsset, filename, AASSET_MODE_BUFFER);
		if (!ptrFile)
		{
			LOGW("WARNING: %s could be open ", filename);
			assert(1 > 2);
		}

		png_byte header[8];

		AAsset_read(ptrFile, header, 8);

		int is_png = !png_sig_cmp(header, 0, 8);

		if (!is_png)
		{
			AAsset_close(ptrFile);
			LOGW("ERROR: NOT A PNG FILE %s", filename);
			assert(1 > 2);
		}

		png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
			NULL, NULL);
		if (!png_ptr)
		{
			AAsset_close(ptrFile);
			LOGW("unable to create png struct");
			assert(1 > 2);
		}

		//create png info struct
		png_infop info_ptr = png_create_info_struct(png_ptr);
		if (!info_ptr)
		{
			png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
			LOGW("Unable to create png info : %s", filename);
			AAsset_close(ptrFile);
			assert(1 > 2);
		}

		png_infop end_info = png_create_info_struct(png_ptr);
		if (!end_info) {
			png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
			LOGW("Unable to create png end info : %s", filename);
			AAsset_close(ptrFile);
			assert(1 > 2);
		}

		if (setjmp(png_jmpbuf(png_ptr))) {
			AAsset_close(ptrFile);
			LOGW("Error during setjmp : %s", filename);
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			assert(1 > 2);
		}

		//init png reading
		//png_init_io(png_ptr, fp);
		png_set_read_fn(png_ptr, NULL, png_asset_read);

		//let libpng know you already read the first 8 bytes
		png_set_sig_bytes(png_ptr, 8);

		// read all the info up to the image data
		png_read_info(png_ptr, info_ptr);

		//variables to pass to get info
		int bit_depth, color_type;
		png_uint_32 width, height;

		png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
			NULL, NULL, NULL);

		//if (!IsPowerOfTwo(width) || !IsPowerOfTwo(height)) {
		//	LOGW("Non-power of two ( %d, %d) texture are not supported : %s", width, height, filename);
		//	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		//	AAsset_close(ptrFile);
		//	assert(1 > 2);
		//}

		// Update the png info struct.
		png_read_update_info(png_ptr, info_ptr);

		// Row size in bytes.
		int rowbytes = png_get_rowbytes(png_ptr, info_ptr);

		// Allocate the image_data as a big block
		png_byte *image_data = new png_byte[rowbytes * height];
		if (!image_data) {
			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			LOGW("Unable to allocate image_data while loading %s", filename);
			AAsset_close(ptrFile);
			assert(1 > 2);
		}

		//row_pointers is for pointing to image_data for reading the png with libpng
		png_bytep *row_pointers = new png_bytep[height];
		if (!row_pointers) {
			//clean up memory and close stuff
			png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
			delete[] image_data;
			LOGW("Unable to allocate row_pointer while loading ", filename);
			AAsset_close(ptrFile);
		}
		// set the individual row_pointers to point at the correct offsets of image_data
		for (int i = 0; i < height; ++i) {
			//row_pointers[height - 1 - i] = image_data + i * rowbytes;
			row_pointers[i] = image_data + i * rowbytes;
		}

		//read the png into image_data through row_pointers
		png_read_image(png_ptr, row_pointers);

		ImageData* img = new PNGImageData(image_data, width, height);

		//clean up memory and close stuff
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		delete[] row_pointers;

		AAsset_close(ptrFile);

		ptrImg = img;
	}
	static void png_asset_read(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		AAsset_read(ptrFile, data, length);
	}
#endif
public:
#ifdef __ANDROID__
	GLuint createTexture(AAssetManager* ptrAsset, const char * filename)

	{
		assert(ptrAsset != NULL);
		LoadImage(ptrAsset, filename);

		GLuint texture1;
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ptrImg->img_width, ptrImg->img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, ptrImg->pixels);
		glGenerateMipmap(GL_TEXTURE_2D);

		return texture1;
	}

#elif _WIN32
	GLuint createTexture(char* FileName)
	{
		unsigned long x, y;
		unsigned int texture_id;
		unsigned long data_size;     // length of the file
		int channels;               //  3 =>RGB   4 =>RGBA 
		unsigned int type;
		unsigned char * rowptr[1];    // pointer to an array
		unsigned char * jdata;        // data for the image
		struct jpeg_decompress_struct info; //for our jpeg info
		struct jpeg_error_mgr err;          //the error handler

		FILE* file = fopen(FileName, "rb");  //open the file

		perror("fopen");
		char* path;
		_get_pgmptr(&path);

		info.err = jpeg_std_error(&err);
		jpeg_create_decompress(&info);   //fills info structure

										 //if the jpeg file doesn't load
		if (!file) {
			fprintf(stderr, "Error reading JPEG file %s!", FileName);
			return 0;
		}

		jpeg_stdio_src(&info, file);
		jpeg_read_header(&info, TRUE);   // read jpeg file header

		jpeg_start_decompress(&info);    // decompress the file

										 //set width and height
		x = info.output_width;
		y = info.output_height;
		channels = info.num_components;
		type = GL_RGB;
		if (channels == 4) type = GL_RGBA;

		data_size = x * y * 3;

		//--------------------------------------------
		// read scanlines one at a time & put bytes 
		//    in jdata[] array. Assumes an RGB image
		//--------------------------------------------
		jdata = (unsigned char *)malloc(data_size);
		while (info.output_scanline < info.output_height) // loop
		{
			// Enable jpeg_read_scanlines() to fill our jdata array
			rowptr[0] = (unsigned char *)jdata +  // secret to method
				3 * info.output_width * info.output_scanline;

			jpeg_read_scanlines(&info, rowptr, 1);
		}
		//---------------------------------------------------

		jpeg_finish_decompress(&info);   //finish decompressing

										 //----- create OpenGL tex map (omit if not needed) --------
		glGenTextures(1, &texture_id);
		glBindTexture(GL_TEXTURE_2D, texture_id);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, GL_RGB, GL_UNSIGNED_BYTE, jdata);

		fclose(file);                    //close the file
		free(jdata);

		return texture_id;    // for OpenGL tex maps
	}

#endif

};

