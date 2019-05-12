#ifndef BUFFER_2D_H_
#define BUFFER_2D_H_

#include "fstream"
#include <cstdlib>
#include <stdexcept>
#include <chrono>


namespace art
{
  template <typename T>
  class Buffer2D
  {
    private:
		uint32_t height;
		uint32_t width;
		T* buffer;

		Buffer2D(const Buffer2D<T>&);
		Buffer2D& operator = (const Buffer2D<T>&); //Para nao deixar copiar o buffer
    public:
		Buffer2D(){}
		Buffer2D(uint32_t _width, uint32_t _height)
		{
			width = _width;
			height = _height;
			
			if(width > 0 && height > 0)
				buffer = new T[width * height];
		}

		~Buffer2D()
		{
			delete[] buffer;
		}

		void initialize(uint32_t x,uint32_t y)
		{
			if(!(x > 0 && y > 0))
				throw std::invalid_argument("[BUFFER]: Inicializando objeto Buffer2D fora dos limites");

			height = y;
			width = x;
			buffer = new T[y * x];
		}

		const T& operator () (uint32_t x, uint32_t y) const
		{  
			if(x > width || y > height || x < 0 || y < 0)
				throw std::invalid_argument("[BUFFER]: Acessando objeto Buffer2D fora dos limites");
			return buffer[ (y*width) + x ];   
		}

		T& operator () (uint32_t x, uint32_t y)
		{
			if(x > width || y > height || x < 0 || y < 0)
				throw std::invalid_argument("[BUFFER]: Acessando objeto Buffer2D fora dos limites");
			return buffer[ (y*width) + x ];   
		}

		//Retorna tamanho do buffer em bytes
		uint64_t const getSizeInBytes() const
		{
			return width*height*sizeof(T);
		}

		// get dims
		uint32_t getLargura() const { return width; }
		uint32_t getAltura() const { return height; }
		uint32_t getHeight() const { return width; }
		uint32_t getWidth() const { return height; }

		void writeToDisk(std::ofstream & of) const
		{
			#ifdef USAR_BENCHMARK
                auto startBenchmark = std::chrono::steady_clock::now();
            #endif

			of.write((const char*)buffer, sizeof(T)*height*width);

			#ifdef USAR_BENCHMARK
                auto endBenchmark = std::chrono::steady_clock::now();
                std::cout << "[IMAGEM]: Imagem .ppm escrita em " << std::chrono::duration <double, std::milli> (endBenchmark - startBenchmark).count() << " ms" << std::endl;
            #endif
		}

		void writeToDiskBMP(std::ofstream & of) const
		{
			#ifdef USAR_BENCHMARK
                auto startBenchmark = std::chrono::steady_clock::now();
            #endif
			unsigned char file[14] = {
            'B','M', // magic
            0,0,0,0, // size in bytes
            0,0, // app data
            0,0, // app data
            40+14,0,0,0 // start of data offset
        };
        unsigned char info[40] = {
            40,0,0,0, // info hd size
            0,0,0,0, // width
            0,0,0,0, // heigth
            1,0, // number color planes
            24,0, // bits per pixel
            0,0,0,0, // compression is none
            0,0,0,0, // image bits size
            0x13,0x0B,0,0, // horz resoluition in pixel / m
            0x13,0x0B,0,0, // vert resolutions (0x03C3 = 96 dpi, 0x0B13 = 72 dpi)
            0,0,0,0, // #colors in pallete
            0,0,0,0, // #important colors
            };

        int w=getWidth();
        int h=getHeight();

        int padSize  = (4-(w*3)%4)%4;
        int sizeData = w*h*3 + h*padSize;
        int sizeAll  = sizeData + sizeof(file) + sizeof(info);

        file[ 2] = (unsigned char)( sizeAll    );
        file[ 3] = (unsigned char)( sizeAll>> 8);
        file[ 4] = (unsigned char)( sizeAll>>16);
        file[ 5] = (unsigned char)( sizeAll>>24);

        info[ 4] = (unsigned char)( w   );
        info[ 5] = (unsigned char)( w>> 8);
        info[ 6] = (unsigned char)( w>>16);
        info[ 7] = (unsigned char)( w>>24);

        info[ 8] = (unsigned char)( h    );
        info[ 9] = (unsigned char)( h>> 8);
        info[10] = (unsigned char)( h>>16);
        info[11] = (unsigned char)( h>>24);

        info[20] = (unsigned char)( sizeData    );
        info[21] = (unsigned char)( sizeData>> 8);
        info[22] = (unsigned char)( sizeData>>16);
        info[23] = (unsigned char)( sizeData>>24);

        of.write( (char*)file, sizeof(file) );
        of.write( (char*)info, sizeof(info) );

        unsigned char pad[3] = {0,0,0};

		//Imagens BMP nao utilizam o padrao RGB, e sim BGR, entao precisamos trocar a posicao
		//std::cout << buffer[(y*width) + x] << std::endl;
		for(int y = 0; y < h; y++)
		{
			for(int x = 0; x < w; x++)
			{
				buffer[(y*w) + x] = T(buffer[(y*w) + x].getZ(), buffer[(y*w) + x].getY(), buffer[(y*w) + x].getX());
			}
		}
			

			for ( int y=h; y!=0; y--)
			{
				T* p = buffer+(y*width);
							of.write((const char*)p, sizeof(T)*width);
							of.write( (char*)pad, padSize );
			}

		//Transforma para RGB de novo
		for(int y = 0; y < h; y++)
			for(int x = 0; x < w; x++)
				buffer[(y*w) + x] = T(buffer[(y*w) + x].getZ(), buffer[(y*w) + x].getY(), buffer[(y*w) + x].getX());

		#ifdef USAR_BENCHMARK
			auto endBenchmark = std::chrono::steady_clock::now();
			std::cout << "[IMAGEM]: Imagem .bmp escrita em " << std::chrono::duration <double, std::milli> (endBenchmark - startBenchmark).count() << " ms" << std::endl;
		#endif
		}
  };
}
#endif