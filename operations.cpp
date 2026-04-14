#include "utils.h"
#include <vector>
#include <algorithm>


int proccessChannel(int value,float modifier, char operation)
{
    float result;

    switch(operation)
    {
        case '+':
            result = static_cast<float>(value) + modifier;
            break;
        case '-':
            result = static_cast<float>(value) - modifier;
            break;
        case '*':
            result = static_cast<float>(value) * modifier;
            break;
        case '/':
            if(modifier == 0.0f)
                result = value;
            else
                result = static_cast<float>(value)/modifier;
            break;
    }

    return std::clamp(static_cast<int>(std::round(result)),0,255);
}


void addPixel(PPM& picture,int modifier)
{
    for(int y = 0; y < picture.getHeight();y++)
    {
        for(int x = 0; x < picture.getWidth(); x++)
        {
            Pixel& pixel = picture.getPixel(x,y);

            pixel.setPixel(
                proccessChannel(pixel.GetRed(),modifier,'+'),
                proccessChannel(pixel.GetGreen(),modifier,'+'),
                proccessChannel(pixel.GetBlue(),modifier,'+')
            );
        }
    }
}

void subtractPixel(PPM& picture,int modifier)
{
    for(int y = 0; y < picture.getHeight();y++)
    {
        for(int x = 0; x < picture.getWidth(); x++)
        {
            Pixel& pixel = picture.getPixel(x,y);

            pixel.setPixel(
                proccessChannel(pixel.GetRed(),modifier,'-'),
                proccessChannel(pixel.GetGreen(),modifier,'-'),
                proccessChannel(pixel.GetBlue(),modifier,'-')
            );
        }
    }
}

void multiplyPixel(PPM& picture, float modifier)
{
    for(int y = 0; y < picture.getHeight(); y++)
    {
        for(int x = 0; x < picture.getWidth(); x++)
        {
            Pixel& pixel = picture.getPixel(x,y);

            pixel.setPixel(
                proccessChannel(pixel.GetRed(),modifier,'*'),
                proccessChannel(pixel.GetGreen(),modifier,'*'),
                proccessChannel(pixel.GetBlue(),modifier,'*')
            );
        }
    }
}

void dividePixel(PPM& picture, float modifier)
{

    if(modifier == 0.0f)
        return;


    for(int y = 0; y < picture.getHeight(); y++)
    {
        for(int x = 0; x < picture.getWidth(); x++)
        {
            Pixel& pixel = picture.getPixel(x,y);

            pixel.setPixel(
                proccessChannel(pixel.GetRed(),modifier,'/'),
                proccessChannel(pixel.GetGreen(),modifier,'/'),
                proccessChannel(pixel.GetBlue(),modifier,'/')
            );
        }
    }
}


void GrayScaleMean(PPM& picture)
{
    for(int y = 0; y < picture.getHeight(); y++)
    {
        for(int x = 0; x < picture.getWidth(); x++)
        {
            Pixel& pixel = picture.getPixel(x,y);

            float gray = (static_cast<float>(pixel.GetRed()) + static_cast<float>(pixel.GetGreen())+ static_cast<float>(pixel.GetBlue()))/3;
            int normalized_gray = static_cast<int>(std::round(gray));

            pixel.setPixel(normalized_gray, normalized_gray, normalized_gray);
        }
    }
}


void GrayScale(PPM& picture)
{
    for(int y = 0; y < picture.getHeight(); y++)
    {
        for(int x = 0; x < picture.getWidth(); x++)
        {
            Pixel& pixel = picture.getPixel(x,y);

            float gray = static_cast<float>(pixel.GetRed()) * 0.299 + static_cast<float>(pixel.GetGreen()) * 0.587 + static_cast<float>(pixel.GetBlue()) * 0.114;
            int normalized_gray = static_cast<int>(std::round(gray));

            pixel.setPixel(normalized_gray, normalized_gray, normalized_gray);
        }
    }
}