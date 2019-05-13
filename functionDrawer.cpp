#include "Image.h"
#include "Colors.h"
#include "Vec2.h"
#include "Vec3.h"

#include <string>
#include <iostream>


int main()
{

    std::string expression;

    int16_t height = 512;
    int16_t width = 512;
    int64_t horizontalStretching = 8;

    std::cout << "image height (default is 512) > ";
    std::cin >> height;
    std::cin.ignore();

    std::cout << "image width (default is 512) > ";
    std::cin >> width;
    std::cin.ignore();

    std::cout << "horizontal Stretching (default is 8, none is 1, the higher the number, the more stretched the output will be) > ";
    std::cin >> horizontalStretching;
    std::cin.ignore();

    while(true)
    {
        art::Image<art::Vec3<uint8_t>> imagem(height, width);

        std::cout << "\nWrite 'exit' to close" << std::endl;
        std::cout << "Expression > ";

        getline(std::cin, expression);
        if(expression == "exit")
            break;

        std::cout << "Calculating..." << std::endl;
        imagem.drawFunction(expression, BRANCO, horizontalStretching);
        std::cout << "Rendered successfully..." << std::endl;
        imagem.writeImageBMP("functionOutput.bmp");
        std::cout << "Image saved successfully..." << std::endl;
    }


    return 0;
}