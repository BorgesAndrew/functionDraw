#ifndef IMAGE_H_
#define IMAGE_H_

//Inclui o <filesystem>
#ifdef USANDO_VERSAO_MODERNA

#include <filesystem>

    #ifdef _WIN32 //MSVC++ utiliza experimental
        namespace fs = std::experimental::filesystem;
    #else
        namespace fs = std::filesystem;
    #endif
#endif

#include <chrono>

#include "Buffer2D.h"
#include "Vec2.h"
#include "Function.h"

namespace art 
{
    template <typename T>
    class Image
    {
    private:
        Buffer2D<T> frameBuffer;
        int64_t width, height;
        std::vector<art::Vec2<int64_t>> DrawingVerticesBuffer;
        bool isUsingGlow;

    public:
        Image(const unsigned x, const unsigned y)
        {
            isUsingGlow = false;
            width = x;
            height = y;
            std::cout << "[IMAGE]: Construindo imagem..." << std::endl;
            #ifdef USAR_BENCHMARK
                auto startBenchmark = std::chrono::steady_clock::now();
            #endif

            frameBuffer.initialize(x,y);
            std::cout << "[IMAGE]: Imagem criada..." << std::endl;
            
            #ifdef USAR_BENCHMARK
                auto endBenchmark = std::chrono::steady_clock::now();
                std::cout << "[IMAGE]: Tempo para inicializacao da imagem: " << std::chrono::duration <double, std::milli> (endBenchmark - startBenchmark).count() << " ms" << std::endl;
            #endif
        }

        void setUsingGlow(bool _isUsingGlow)
        {
            isUsingGlow = _isUsingGlow;
        }

        void writeBinary(std::ofstream & outputFile, const T & dataBuffer, const std::ios_base::openmode io_mode = std::ios::binary | std::ios::app)
        {
            outputFile.write((const char*)&dataBuffer, sizeof(dataBuffer));
        }

        void setPixel(int64_t x, int64_t y, const T color = T(T().maxValue()))
        {
            y*=-1;
            x+=(width/2);
            y+=(height/2);
            if(!validBounds(x,y)) 
                return;

            frameBuffer(x,y) = color;
        }

        /* DESENHAR A PARTIR DE ARQUIVO */
        void drawFile(std::string fileName, const T color = T(T().maxValue()))
        {
            std::ifstream file(fileName);
            if(!file)
                return;
            double input;
            bool flipFlop = true;
            while(file >> input)
            {
                if(flipFlop)
                    DrawingVerticesBuffer.push_back(art::Vec2<int64_t>(input,0));
                else
                    DrawingVerticesBuffer.back() += art::Vec2<int64_t>(0,input);

                flipFlop = !flipFlop;
            }
            
            drawMultiLineFunction();
        }

        /* DRAW MULTI LINE */

        void drawMultiLineFunction(art::Vec2<int64_t> const arg, uint64_t & polygonCounter)
        {
            DrawingVerticesBuffer.push_back(arg);
        }

        void drawMultiLineFunction(int64_t const arg, uint64_t & polygonCounter)
        {
            if(polygonCounter%2 == 0)
                DrawingVerticesBuffer.push_back(art::Vec2<int64_t>(arg, 0));
            else
                DrawingVerticesBuffer.back() += art::Vec2<int64_t>(0, arg);
            
            polygonCounter++;
        }
        
        void drawMultiLineFunction(const T color = T(T().maxValue()))
        {
            art::Vec2<int64_t> verticeBuffer;
            while(DrawingVerticesBuffer.size()-1)
            {
                verticeBuffer = DrawingVerticesBuffer.back();
                DrawingVerticesBuffer.pop_back();
                drawLine(verticeBuffer, DrawingVerticesBuffer.back(), color);
            }
            DrawingVerticesBuffer.clear();
        }

        template<typename ... Args>
        void drawMultiLine(Args const... args) 
        {
            if(sizeof...(args) < 2)
                return;
            DrawingVerticesBuffer.reserve(sizeof...(args));
            uint64_t polygonCounter = 0;
            (void)std::initializer_list<int>
            {
                ((void)drawMultiLineFunction(args, polygonCounter),0)...
            };

            drawMultiLineFunction();
        }

        template<typename ... Args>
        void drawMultiLine(const T color = T(T().maxValue()), Args const... args) 
        {
            if(sizeof...(args) < 2)
                return;
            DrawingVerticesBuffer.reserve(sizeof...(args));
            uint64_t polygonCounter = 0;
            (void)std::initializer_list<int>
            {
                ((void)drawMultiLineFunction(args, polygonCounter),0)...
            };

            drawMultiLineFunction(color);
        }

        void drawMultiLine(std::vector<int64_t> & vertices, const T color = T(T().maxValue()))
        {
            if(vertices.size() < 4 || vertices.size()%2 != 0)
                return;
            
            uint64_t polygonCounter = 0;
            for(const auto & vertex : vertices)
                drawMultiLineFunction(vertex, polygonCounter);

            drawMultiLineFunction(color);
        }
        
        void drawMultiLine(std::vector<art::Vec2<int64_t>> vertices, const T color = T(T().maxValue()))
        {
            if(vertices.size() < 2)
                return;
            
            uint64_t polygonCounter = 0;
            for(const auto & vertex : vertices)
                drawMultiLineFunction(vertex, polygonCounter);

            drawMultiLineFunction(color);
        }

        /* DRAW POLYGON */

        void drawPolygonFunction(art::Vec2<int64_t> const arg, uint64_t & polygonCounter)
        {
            DrawingVerticesBuffer.push_back(arg);
        }

        void drawPolygonFunction(int64_t const arg, uint64_t & polygonCounter)
        {
            if(polygonCounter%2 == 0)
                DrawingVerticesBuffer.push_back(art::Vec2<int64_t>(arg, 0));
            else
                DrawingVerticesBuffer.back() += art::Vec2<int64_t>(0, arg);
            
            polygonCounter++;
        }
        
        void drawPolygonFunction(const T color = T(T().maxValue()))
        {
            drawLine(DrawingVerticesBuffer.front(), DrawingVerticesBuffer.back(), color);

            art::Vec2<int64_t> verticeBuffer;
            while(DrawingVerticesBuffer.size()-1)
            {
                verticeBuffer = DrawingVerticesBuffer.back();
                DrawingVerticesBuffer.pop_back();
                drawLine(verticeBuffer, DrawingVerticesBuffer.back(), color);
            }
            DrawingVerticesBuffer.clear();
        }

        template<typename ... Args>
        void drawPolygon(Args const... args) 
        {
            if(sizeof...(args) < 3)
                return;
            DrawingVerticesBuffer.reserve(sizeof...(args));
            uint64_t polygonCounter = 0;
            (void)std::initializer_list<int>
            {
                ((void)drawPolygonFunction(args, polygonCounter),0)...
            };

            drawPolygonFunction();
        }

        template<typename ... Args>
        void drawPolygon(const T color = T(T().maxValue()), Args const... args) 
        {
            if(sizeof...(args) < 3)
                return;
            DrawingVerticesBuffer.reserve(sizeof...(args));
            uint64_t polygonCounter = 0;
            (void)std::initializer_list<int>
            {
                ((void)drawPolygonFunction(args, polygonCounter),0)...
            };

            drawPolygonFunction(color);
        }

        void drawPolygon(std::vector<int64_t> & vertices, const T color = T(T().maxValue()))
        {
            if(vertices.size() < 4 || vertices.size()%2 != 0)
                return;
            
            uint64_t polygonCounter = 0;
            for(const auto & vertex : vertices)
                drawPolygonFunction(vertex, polygonCounter);

            drawPolygonFunction(color);
        }
        
        void drawPolygon(std::vector<art::Vec2<int64_t>> vertices, const T color = T(T().maxValue()))
        {
            if(vertices.size() < 2)
                return;
            
            uint64_t polygonCounter = 0;
            for(const auto & vertex : vertices)
                drawPolygonFunction(vertex, polygonCounter);

            drawPolygonFunction(color);
        }

        void drawRectangle(art::Vec2<int64_t> p1, art::Vec2<int64_t> p2, const T color = T(T().maxValue()))
        {
            drawRectangle(p1.getX(), p1.getY(), p2.getX(), p2.getY(), color);
        }
        void drawRectangle(int64_t x1, int64_t y1, int64_t x2, int64_t y2, const T color = T(T().maxValue()))
        {
            drawLine(x1,y1,x2,y1, color); //desenha topo
            drawLine(x1,y1,x1,y2, color); //desenha lado esquerdo
            drawLine(x2,y1,x2,y2, color); //desenha lado direito
            drawLine(x1,y2,x2,y2, color); //desenha em baixo
        }

        void drawTriangle(art::Vec2<int64_t> p1, art::Vec2<int64_t> p2, art::Vec2<int64_t> p3, const T color = T(T().maxValue()))
        {
            drawTriangle(p1.getX(), p1.getY(), p2.getX(), p2.getY(), p3.getX(), p3.getY(), color);
        }

        void drawTriangle(int64_t x1, int64_t y1, int64_t x2, int64_t y2, int64_t x3, int64_t y3, const T color = T(T().maxValue()))
        {
            drawLine(x1,y1,x2,y2);
            drawLine(x2,y2,x3,y3);
            drawLine(x3,y3,x1,y1);
        }

        void drawCircle(art::Vec2<int64_t> p, int64_t raio, const T color = T(T().maxValue()))
        {
            drawCircle(p.getX(), p.getY(), raio, color);
        }

        void drawCircle(int64_t xCenter, int64_t yCenter, int64_t raio, const T color = T(T().maxValue()))
        {
            int64_t x = raio;
            int64_t y = 0;
            int64_t d = 1 - raio;

            while(x > y)
            {
                setPixel(x + xCenter, y + yCenter, color);
                setPixel(x + xCenter, -y + yCenter, color);
                setPixel(-x + xCenter, y + yCenter, color);
                setPixel(-x + xCenter, -y + yCenter, color);
                setPixel(y + xCenter, x + yCenter, color);
                setPixel(y + xCenter, -x + yCenter, color);
                setPixel(-y + xCenter, x + yCenter, color);
                setPixel(-y + xCenter, -x + yCenter, color);
                y = y + 1;
                if(d < 0)
                    d = d + 2*y + 1;
                else
                {
                    x = x - 1;
                    d = d + 2*(y-x) + 1;
                }
            }

            // fonte(melhor): https://www.cs.helsinki.fi/group/goa/mallinnus/ympyrat/ymp1.html
            // fonde: http://groups.csail.mit.edu/graphics/classes/6.837/F98/Lecture6/circle.html
        }

        bool const validBounds(int64_t x, int64_t y) const
        {
            if(x < width && x > 0 && y < height && y > 0)
                return true;
            return false;
        }

        void drawLine(art::Vec2<int64_t> p1, art::Vec2<int64_t> p2, const T color = T(T().maxValue()))
        {
            drawLine(p1.getX(), p1.getY(), p2.getX(), p2.getY(), color);
        }

        inline void drawLine(const int64_t x, const int64_t y, const int64_t x2, const int64_t y2, const T color = T(T().maxValue()))
        {
            bool yLonger=false;
            int64_t incrementVal;

            int64_t shortLen=y2-y;
            int64_t longLen=x2-x;
            if (abs(shortLen)>abs(longLen)) {
                shortLen ^= longLen; 
                longLen  ^= shortLen; 
                shortLen ^= longLen;
                yLonger=true;
            }

            if (longLen<0) incrementVal=-1;
            else incrementVal=1;

            double divDiff;
            if (shortLen==0) divDiff=longLen;
            else divDiff=(double)longLen/(double)shortLen;
            if (yLonger) {
                for (int64_t i=0;i!=longLen;i+=incrementVal) {
                    setPixel(x+(int64_t)((double)i/divDiff),y+i, color);
                }
            } else {
                for (int64_t i=0;i!=longLen;i+=incrementVal) {
                    setPixel(x+i,y+(int64_t)((double)i/divDiff), color);
                }
            }
        }

        void drawFunction(std::string expression, const T color = T(T().maxValue()), const int64_t esticamentoHorizontal = 1)
        {
            DrawingVerticesBuffer.clear();
            art::Function function(expression);
            const int64_t rangeEnd = (frameBuffer.getWidth()/2);
            const int64_t rangeStart = rangeEnd*-1;

            for(int64_t x = rangeStart; x < rangeEnd; x++)
            {
                DrawingVerticesBuffer.push_back(art::Vec2<int64_t>(x*esticamentoHorizontal,function.getResult(x)));
                
            }

            drawMultiLineFunction(color);
        }

    #ifdef USANDO_VERSAO_MODERNA
        void const writeImagePPM(fs::path filePath, const std::ios_base::openmode io_mode = std::ios::binary | std::ios::app)
        {
            if(fs::exists(filePath))
                fs::remove(filePath);

            //ESCREVE HEADER
            std::ofstream file(filePath.string().c_str());
            file << "P6\n" << frameBuffer.getLargura() << ' ' << frameBuffer.getAltura() << "\n" << static_cast<unsigned>(T().maxValue()) << "\n";
            file.close();
            
            file.open(filePath.string().c_str(), io_mode);
            //ESCREVE IMAGEM
            frameBuffer.writeToDisk(file);
            file.close();
        }
    #endif

        void const writeImagePPM(std::string fileName, const std::ios_base::openmode io_mode = std::ios::binary | std::ios::app) const
        {
            std::cout << "[IMAGEM]: Escrevendo imagem .ppm..." << std::endl;
            //ESCREVE HEADER
            std::ofstream file(fileName.c_str(), std::ios::trunc);
            file << "P6\n" << frameBuffer.getLargura() << ' ' << frameBuffer.getAltura() << "\n" << static_cast<unsigned>(T().maxValue()) << "\n";
            file.close();

            file.open(fileName.c_str(), io_mode);
            //ESCREVE IMAGEM
            frameBuffer.writeToDisk(file);
            file.close();

            std::cout << "[IMAGEM]: Imagem .ppm escrita..." << std::endl;
        }

        void writeImageBMP(std::string fileName, const std::ios_base::openmode io_mode = std::ios::binary | std::ios::app)
        {
            std::cout << "[IMAGEM]: Escrevendo imagem .bmp..." << std::endl;
            std::ofstream fileOutput(fileName, std::ios::trunc);
            fileOutput.close();
            fileOutput.open(fileName, io_mode);
            frameBuffer.writeToDiskBMP(fileOutput);
            fileOutput.close();
            std::cout << "[IMAGEM]: Imagem .bmp escrita..." << std::endl;
        }

        uint64_t getSizeInBytes()
        {
            return frameBuffer.getSizeInBytes() + sizeof(this);
        }
    };
}
#endif