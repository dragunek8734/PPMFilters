#include "utils.h"
#include <algorithm>
#include <iostream>
#include <cmath>

void skipBorders(PPM original, PPM &copy, int margin)
{
    // left border
    for (int x = 0; x < margin; x++)
    {
        for (int y = 0; y < original.getHeight(); y++)
        {
            Pixel &pix = original.getPixel(x, y);
            Pixel &newpix = copy.getPixel(x, y);

            newpix.setPixel(pix.GetRed(), pix.GetGreen(), pix.GetBlue());
        }
    }

    // right border
    for (int x = original.getWidth() - 1 - margin; x < original.getWidth(); x++)
    {
        for (int y = 0; y < original.getHeight(); y++)
        {
            Pixel &pix = original.getPixel(x, y);
            Pixel &newpix = copy.getPixel(x, y);

            newpix.setPixel(pix.GetRed(), pix.GetGreen(), pix.GetBlue());
        }
    }

    // top border
    for (int y = 0; y < margin; y++)
    {
        for (int x = 0; x < original.getWidth(); x++)
        {
            Pixel &pix = original.getPixel(x, y);
            Pixel &newpix = copy.getPixel(x, y);

            newpix.setPixel(pix.GetRed(), pix.GetGreen(), pix.GetBlue());
        }
    }

    // bottom border
    for (int y = original.getHeight() - 1 - margin; y < original.getHeight(); y++)
    {
        for (int x = 0; x < original.getWidth(); x++)
        {
            Pixel &pix = original.getPixel(x, y);
            Pixel &newpix = copy.getPixel(x, y);

            newpix.setPixel(pix.GetRed(), pix.GetGreen(), pix.GetBlue());
        }
    }
}

PPM avgFilter(PPM picture, int window_size)
{
    if (window_size % 2 == 0 || window_size <= 2)
    {
        std::cout << "Wrong window size, use uneven value, 3 or greater" << std::endl;
        return picture;
    }

    int w = picture.getWidth();
    int h = picture.getHeight();
    int mval = picture.getMaxVal();

    int margin = static_cast<int>(floor(static_cast<float>(window_size) / 2));

    PPM *newpic = new PPM(w, h, mval);

    // Apply filter to all pixels using clamping for border handling
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            int sumRed = 0;
            int sumGreen = 0;
            int sumBlue = 0;

            for (int window_y = y - margin; window_y <= y + margin; window_y++)
            {
                for (int window_x = x - margin; window_x <= x + margin; window_x++)
                {
                    int clamped_x = std::clamp(window_x, 0, w - 1);
                    int clamped_y = std::clamp(window_y, 0, h - 1);
                    Pixel &pix = picture.getPixel(clamped_x, clamped_y);

                    sumRed += pix.GetRed();
                    sumGreen += pix.GetGreen();
                    sumBlue += pix.GetBlue();
                }
            }

            int windowArea = window_size * window_size;
            int newRed = sumRed / windowArea;
            int newGreen = sumGreen / windowArea;
            int newBlue = sumBlue / windowArea;

            Pixel &pix = newpic->getPixel(x, y);
            pix.setPixel(newRed, newGreen, newBlue);
        }
    }

    PPM result = *newpic;
    delete newpic;
    return result;
}

PPM medianFilter(PPM picture, int window_size)
{
    if (window_size % 2 == 0 || window_size <= 2)
    {
        std::cout << "Wrong window size, use uneven value, 3 or greater" << std::endl;
        return picture;
    }

    int w = picture.getWidth();
    int h = picture.getHeight();
    int mval = picture.getMaxVal();

    int margin = static_cast<int>(floor(static_cast<float>(window_size) / 2));

    PPM *newpic = new PPM(w, h, mval);

    // Apply filter to all pixels using clamping for border handling
    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            std::vector<int> Red;
            std::vector<int> Green;
            std::vector<int> Blue;

            for (int window_y = y - margin; window_y <= y + margin; window_y++)
            {
                for (int window_x = x - margin; window_x <= x + margin; window_x++)
                {
                    int clamped_x = std::clamp(window_x, 0, w - 1);
                    int clamped_y = std::clamp(window_y, 0, h - 1);
                    Pixel &pix = picture.getPixel(clamped_x, clamped_y);

                    Red.push_back(pix.GetRed());
                    Green.push_back(pix.GetGreen());
                    Blue.push_back(pix.GetBlue());
                }
            }

            sort(Red.begin(), Red.end());
            sort(Green.begin(), Green.end());
            sort(Blue.begin(), Blue.end());

            int medianIndex = Red.size() / 2;
            int newRed = Red[medianIndex];
            int newGreen = Green[medianIndex];
            int newBlue = Blue[medianIndex];

            Pixel &pix = newpic->getPixel(x, y);
            pix.setPixel(newRed, newGreen, newBlue);
        }
    }

    PPM result = *newpic;
    delete newpic;
    return result;
}

PPM sobelFilter(PPM picture)
{
    int Sobel_X[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Sobel_Y[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    int w = picture.getWidth();
    int h = picture.getHeight();
    int maxval = picture.getMaxVal();

    PPM gray = picture;
    GrayScale(gray);

    PPM *newpic = new PPM(w, h, maxval);

    for (int y = 1; y < picture.getHeight() - 1; y++) // 300 szerokosc ->  < 299 czyli [298] ostatni pixel
    {
        for (int x = 1; x < picture.getWidth() - 1; x++)
        {
            /*  x = columns, y = rows
                a b c
                d e f  in e we are
                g h i
            */

            int a = gray.getPixel(x - 1, y - 1).GetGreen();
            int b = gray.getPixel(x, y - 1).GetGreen();
            int c = gray.getPixel(x + 1, y - 1).GetGreen();
            int d = gray.getPixel(x - 1, y).GetGreen();
            int e = gray.getPixel(x, y).GetGreen(); // CENTER
            int f = gray.getPixel(x + 1, y).GetGreen();
            int g = gray.getPixel(x - 1, y + 1).GetGreen();
            int h = gray.getPixel(x, y + 1).GetGreen();
            int i = gray.getPixel(x + 1, y + 1).GetGreen();

            int Gx = Sobel_X[0] * a + Sobel_X[1] * b + Sobel_X[2] * c + Sobel_X[3] * d + Sobel_X[4] * e + Sobel_X[5] * f + Sobel_X[6] * g + Sobel_X[7] * h + Sobel_X[8] * i;

            int Gy = Sobel_Y[0] * a + Sobel_Y[1] * b + Sobel_Y[2] * c + Sobel_Y[3] * d + Sobel_Y[4] * e + Sobel_Y[5] * f + Sobel_Y[6] * g + Sobel_Y[7] * h + Sobel_Y[8] * i;

            float magnitude = std::sqrt(Gx * Gx + Gy * Gy);
            int gradient = std::clamp((int)magnitude, 0, maxval);

            Pixel &pix = newpic->getPixel(x, y);
            pix.setPixel(gradient, gradient, gradient);
        }
    }

    PPM result = *newpic;
    delete newpic;
    return result;
}

PPM unsharp(PPM picture, float amount)
{

    if (amount < 0.5f || amount > 2.0f)
    {
        std::cout << "Wrong sharpening degree, use value from <0.5,2> range" << std::endl;
        return picture;
    }

    int w = picture.getWidth();
    int h = picture.getHeight();
    int maxval = picture.getMaxVal();

    PPM blurry = avgFilter(picture, 3);

    PPM *newpic = new PPM(w, h, maxval);

    for (int y = 0; y < picture.getHeight(); y++)
    {
        for (int x = 0; x < picture.getWidth(); x++)
        {
            Pixel &pix = picture.getPixel(x, y);
            Pixel &blur = blurry.getPixel(x, y);

            Pixel &newpix = newpic->getPixel(x, y);

            int newRed = std::clamp(static_cast<int>(pix.GetRed() + amount * (pix.GetRed() - blur.GetRed())), 0, maxval);
            int newGreen = std::clamp(static_cast<int>(pix.GetGreen() + amount * (pix.GetGreen() - blur.GetGreen())), 0, maxval);
            int newBlue = std::clamp(static_cast<int>(pix.GetBlue() + amount * (pix.GetBlue() - blur.GetBlue())), 0, maxval);

            newpix.setPixel(newRed, newGreen, newBlue);
        }
    }

    PPM result = *newpic;
    delete newpic;
    return result;
}

// dodac gaussian filter i dowolna maska

// G(x,y) = G(x) * G(y), o(n) = 2K not K^2, optimal
PPM gaussianFilter(PPM picture, float sigma)
{

    int radius = std::ceil(3 * sigma);
    int size = 2 * radius + 1;

    std::vector<float> kernel(size);

    float sum = 0.0f;

    for (int i = -radius; i <= radius; i++) // building 1D gaussian mask
    {
        float val = std::exp(-(i * i) / (2 * sigma * sigma));
        kernel[i + radius] = val;
        sum += val;
    }

    for (float &v : kernel) // mask normalization
    {
        v /= sum;
    }

    int w = picture.getWidth();
    int h = picture.getHeight();
    int maxval = picture.getMaxVal();
    PPM *newpic = new PPM(w, h, maxval); // result
    PPM *temp = new PPM(w, h, maxval);   // after the horizontal blur, result is built ON that horizontally blurred image

    // horizontal filtering

    for (int y = 0; y < picture.getHeight(); y++)
    {
        for (int x = 0; x < picture.getWidth(); x++)
        {
            float r = 0, g = 0, b = 0;
            Pixel &temppix = temp->getPixel(x, y);

            for (int k = -radius; k <= radius; k++)
            {
                int xx = std::clamp(x + k, 0, w - 1);
                Pixel &p = picture.getPixel(xx, y);
                float weight = kernel[k + radius];

                r += static_cast<float>(p.GetRed()) * weight;
                g += static_cast<float>(p.GetGreen()) * weight;
                b += static_cast<float>(p.GetBlue()) * weight;
            }

            temppix.setPixel(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        }
    }

    for (int y = 0; y < picture.getHeight(); y++)
    {
        for (int x = 0; x < picture.getWidth(); x++)
        {
            float r = 0, g = 0, b = 0;
            Pixel &newpix = newpic->getPixel(x, y);

            for (int k = -radius; k <= radius; k++)
            {
                int yy = std::clamp(y + k, 0, h - 1);
                Pixel &p = temp->getPixel(x, yy);
                float weight = kernel[k + radius];

                r += static_cast<float>(p.GetRed()) * weight;
                g += static_cast<float>(p.GetGreen()) * weight;
                b += static_cast<float>(p.GetBlue()) * weight;
            }

            newpix.setPixel(static_cast<int>(r), static_cast<int>(g), static_cast<int>(b));
        }
    }

    PPM result = *newpic;
    delete newpic;
    delete temp;
    return result;
}

PPM userFilter(PPM picture, int kernel_size)
{
    if (kernel_size % 2 == 0 || kernel_size <= 2)
    {
        std::cout << "Wrong window size, use uneven value, 3 or greater" << std::endl;
        return picture;
    }

    std::vector<std::vector<float>> kernel(kernel_size, std::vector<float>(kernel_size, 0));
    int sum = 0;
    for (int y = 0; y < kernel_size; y++)
    {
        for (int x = 0; x < kernel_size; x++)
        {
            int val;
            std::cout << "\nProvide value for [" << x << "]" << "[" << y << "]" << " kernel value: ";
            std::cin >> val;

            sum += val;
            kernel[x][y] = static_cast<float>(val);
        }
    }

    // normalization

    if (sum > 0)
    {
        for (int y = 0; y < kernel_size; y++)
        {
            for (int x = 0; x < kernel_size; x++)
            {
                kernel[x][y] /= sum;
            }
        }
    }
    else
    {
        char c;
        std::cout << "Kernel sum is " << sum << " (0 or negative). "
                  << "It will NOT be normalized. Continue anyway? (y/n): ";
        std::cin >> c;

        
        if (std::tolower(static_cast<unsigned char>(c)) != 'y')
        {
            std::cout << "Operation cancelled.\n";
            return picture;
        }
        
        std::cout << "Continuing without normalization.\n";
    }

    int w = picture.getWidth();
    int h = picture.getHeight();
    int maxval = picture.getMaxVal();

    int radius = kernel_size / 2;

    PPM *newpic = new PPM(w, h, maxval);

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;

            Pixel &newpix = newpic->getPixel(x, y);

            for (int i = -radius; i <= radius; i++)
            {
                for (int j = -radius; j <= radius; j++)
                {
                    int xx = std::clamp(x + i, 0, w - 1);
                    int yy = std::clamp(y + j, 0, h - 1);

                    Pixel &p = picture.getPixel(xx, yy);

                    float k = kernel[i + radius][j + radius];

                    sum_r += static_cast<float>(p.GetRed()) * k;
                    sum_g += static_cast<float>(p.GetGreen()) * k;
                    sum_b += static_cast<float>(p.GetBlue()) * k;
                }
            }

            newpix.setPixel(
                std::clamp(static_cast<int>(sum_r), 0, maxval),
                std::clamp(static_cast<int>(sum_g), 0, maxval),
                std::clamp(static_cast<int>(sum_b), 0, maxval));
        }
    }

    PPM result = *newpic;
    delete newpic;
    return result;
}