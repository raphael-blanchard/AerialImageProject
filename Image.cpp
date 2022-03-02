////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Image.h"

//to convert to a string
#include <string>

void Image::writeSVG(const std::string &filename, int pixelSize) const
{
     assert(pixelSize > 0);

     std::ofstream file;
     file.open(filename + ".svg");

     if (!file)
          throw std::runtime_error("error open file (write SVG)");

     file << "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
          << std::endl
          << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\""
          << pixelSize * width()
          << "\" height=\""
          << pixelSize * height()
          << "\">"
          << std::endl;

     for (int i = 1; i < height(); ++i)
     {
          for (int j = 1; j < width(); ++j)
          {
               file << "<rect width=\""
                    << pixelSize
                    << "\" height=\""
                    << pixelSize
                    << "\" x=\""
                    << pixelSize * (j - 1)
                    << "\" y=\""
                    << pixelSize * (i - 1)
                    << "\" fill=\""
                    << getPixel(i, j)
                    << "\" />"
                    << std::endl;
          }
     }

     file << "</svg>"
          << std::endl;

     file.close();
}

Image::Image(int w, int h)
{
     heightVal = h;
     widthVal = w;

     //pixelMatrix being a vector of vectors, we first have to push_back the Black color in a temporary vector
     //and then push_back this vector to the pixelMatrix
     vector<Color> tmpVector;
     //
     for (int j = 1; j <= w; j++)
     {
          tmpVector.push_back(Color::Black);
     }

     //loop to instatiate every pixel of the matrix to be black
     //using the height here as pixelMatrix[i][j] would be the pixel of the ith row and in the jth column
     for (int i = 1; i <= h; i++)
     {
          //pushing back the vector we just updated in the loop with j
          //each tmpVector corresponds to a row of the image
          pixelMatrix.push_back(tmpVector);
     }

     //O(w+h) ? O(n^2)
}

int Image::width() const
{
     return widthVal;
}

int Image::height() const
{
     return heightVal;
}

int Image::size() const
{
     return heightVal * widthVal;
}

Image Image::readAIP(const std::string &filename)
{
     //ask what precondition do we need for this method.

     // Create a text string, which is used to manipulate the AIP file
     string myText;
     // Create and open a AIP file via an object from the ifstream class
     ifstream MyReadFile("./images/" + filename);

     //re
     getline(MyReadFile, myText);

     //using integers to store the return values of 2 functions getHeightOfMatrix(...) and getWidthOfMatrix(...)
     int heightOfMatrix = getHeightOfMatrix(filename);
     int widthOfMatrix = getWidthOfMatrix(filename);


     cout << "width of file is: " << widthOfMatrix << endl;

     //initializing an object of the Image class to a full black matrix, that will then be changed pixel by pixel
     Image tmpImage = Image(widthOfMatrix, heightOfMatrix);

     //iterator that will be used to iterate through each row of the matrix
     int iIterator = 1;
     //while loop that will iterate through each line of the AIP file
     while (getline(MyReadFile, myText))
     {
          //myText.size() here gives us the length (number of columns) of the current line we are looking at
          //starting the loop at j=1 later as the setPixel function modifies the pixel at position i-1 and j-1
          for (int j = 1; j < (int)myText.size(); j++)
          {
               //setting the pixel of the iIterator-th row and j-th column to the color read at iIterator row and j-1-th column
               tmpImage.setPixel(iIterator, j, Color::makeColor((int)myText[j - 1] - 48));
          }
          iIterator++;
     }

     // Close the file
     MyReadFile.close();

     return tmpImage;
}

//
Color Image::getPixel(int i, int j) const
{
     //assert(1 <= i <= height() && 1 <= j <= width());
     //getting the pixel at i-1 and j-1 as i and j have to be >= 1
     //which means if i or/and j are 1, we want the first pixel which is at position 1-1=0
     return pixelMatrix.at(i - 1).at(j - 1);
}

void Image::setPixel(int i, int j, Color col)
{
     //add the assert later
     pixelMatrix.at(i - 1).at(j - 1) = col;
}

void Image::DisplayImageInTerminal()
{
     for (int i = 1; i <= (int)pixelMatrix.size(); i++)
     {
          for (int j = 1; j <= (int)pixelMatrix.at(i - 1).size(); j++)
          {
               cout << pixelMatrix.at(i - 1).at(j - 1).toInt();
          }
          cout << endl;
     }
}

int Image::getHeightOfMatrix(const std::string &filename)
{
     string heightOfMatrixSTD;
     // Create a text string, which is used to manipulate the AIP file
     string myText;
     // Create and open a AIP file via an object from the ifstream class
     ifstream MyReadFile("./images/" + filename);

     getline(MyReadFile, myText);
     int i;
     for (i = 0; i < myText.length(); i++){
          if (myText[i] == ' '){
               break;
          }
     }
     for (int j = myText.length() - i; j < myText.length(); j++){
          heightOfMatrixSTD[j-i-1] = myText[j];
     }
     int heightOfMatrix = stoi(heightOfMatrixSTD);
     // Close the file
     MyReadFile.close();
     return heightOfMatrix;
}

int Image::getWidthOfMatrix(const std::string &filename)
{
     string widthOfMatrixSTD;
     // Create a text string, which is used to manipulate the AIP file
     string myText;
     // Create and open a AIP file via an object from the ifstream class
     ifstream MyReadFile("./images/" + filename);

     getline(MyReadFile, myText);
     int i;
     for (i = 0; i < myText.length(); i++){
          if (myText[i] == ' '){
               break;
          }
          else {
               widthOfMatrixSTD[i] = myText[i];
          }
     }
     int widthOfMatrix = stoi(widthOfMatrixSTD);
     // Close the file
     MyReadFile.close();
     return widthOfMatrix;
}