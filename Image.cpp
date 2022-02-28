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

     for (int i = 1; i < height(); ++i){
          for (int j = 1; j < width(); ++j){
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
     //loop to instatiate every pixel of the matrix to be black
     //using the height here as pixelMatrix[i][j] would be the pixel of the ith row and in the jth column
     for (int i = 1; i <= h; i++)
     {
          //pixelMatrix being a vector of vectors, we first have to push_back the Black color in a temporary vector
          //and then push_back this vector to the pixelMatrix
          vector<Color> tmpVector;
          for (int j = 1; j <= w; j++)
          {
               tmpVector.push_back(Color::Black);
          }
          //pushing back the vector we just updated in the loop with j
          //each tmpVector corresponds to a row of the image
          pixelMatrix.push_back(tmpVector);
     }
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

     // Create a text string, which is used to manipulate the text file
     string myText;
     // Create and open a text file using a library
     ifstream MyReadFile("./images/" + filename);
     // Use a while loop together with the getline() function to read the file line by line
     getline(MyReadFile, myText);
     //getting the width of the file by converting a char to inv playing with ascii values
     int widthOfFile = std::stoi(to_string((int)myText[0] - 48) + to_string((int)myText[1] - 48));
     int heightOfFile = std::stoi(to_string((int)myText[3] - 48) + to_string((int)myText[4] - 48));
     cout << "width of file is: " << widthOfFile << endl;
     Image tmpImage = Image(widthOfFile, heightOfFile);
     int iIterator = 1;
     while (getline(MyReadFile, myText))
     {
          //myText.size() here gives us the length (number of columns) of the current line we are looking at
          for (int j = 1; j < (int)myText.size(); j++){
               //int test = (int)myText[j-1] - 48;
               //cout << "current color is: " << test << endl;
               tmpImage.setPixel(iIterator, j, Color::makeColor((int)myText[j-1] - 48));
          }
          iIterator++;
     }

     // Close the file
     MyReadFile.close();

     return tmpImage;
}

///
Color Image::getPixel(int i, int j) const
{
     //assert(1 <= i <= height() && 1 <= j <= width());
     return pixelMatrix.at(i-1).at(j-1);
}

void Image::setPixel(int i, int j, Color col)
{
     //add the assert later
     pixelMatrix.at(i-1).at(j-1) = col;
}

void Image::DisplayImage(){
     for (int i = 1; i <= (int)pixelMatrix.size(); i++){
          for (int j = 1; j <= (int)pixelMatrix.at(i-1).size(); j++){
               cout << pixelMatrix.at(i-1).at(j-1).toInt();
          }
          cout << endl;
     }
}

//function to return the height of the matrix of the file given as argument
int getHeightOfMatrix(std::string &filemame){
     int heightOfMatrix;
     //add the code for this function later
     return heightOfMatrix;
}