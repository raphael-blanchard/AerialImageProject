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

// to convert to a string
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
     int heightTimesWidth = w * h;
     // iterating w*h times as there are w*h pixels in this matrix
     for (int i = 0; i < heightTimesWidth; i++)
     {
          pixelMatrix.push_back(Color::Black);
     }

     // O(w*h)
}

Image::Image(){
     widthVal = 0;
     heightVal = 0;
}

Image::Image(const Image& img){
     // assert (img.size() == size());
     //if can't assert this, do an if condition and push_back what's needed
     widthVal = img.width();
     heightVal = img.height();

     for (int i = 0; i < widthVal*heightVal; i++){
          pixelMatrix.push_back(img.getPixel(i));
     }
}

//O(1)
int Image::width() const
{
     return widthVal;
}

//O(1)
int Image::height() const
{
     return heightVal;
}

void Image::setHeight(int n) {
     heightVal = n;
}

void Image::setWidth(int n){
     widthVal = n;
}

//O(1)
int Image::size() const
{
     return heightVal * widthVal;
}

Image Image::readAIP(const std::string &filename)
{
     // ask what precondition do we need for this method.

     // Create a text string, which is used to manipulate the AIP file
     string myText;
     // Create and open a AIP file via an object from the ifstream class
     ifstream MyReadFile("./images/" + filename);

     // re
     getline(MyReadFile, myText);

     // using integers to store the return values of 2 functions getHeightOfMatrix(...) and getWidthOfMatrix(...)
     int heightOfMatrix = getHeightOfMatrix(filename);
     int widthOfMatrix = getWidthOfMatrix(filename);

     // initializing an object of the Image class to a full black matrix, that will then be changed pixel by pixel
     Image tmpImage = Image(widthOfMatrix, heightOfMatrix);

     // iterator that will be used to iterate through each row of the matrix
     int iIterator = 0;
     // while loop that will iterate through each line of the AIP file
     while (getline(MyReadFile, myText))
     {
          // myText.size() here gives us the length (number of columns) of the current line we are looking at
          // starting the loop at j=1 later as the setPixel function modifies the pixel at position i-1 and j-1
          for (int j = 0; j < (int)myText.size(); j++)
          {
               // setting the pixel of the iIterator-th row and j-th column to the color read at iIterator row and j-1-th column
               tmpImage.setPixel(iIterator, j, Color::makeColor((int)myText[j] - 48));
          }
          iIterator++;
     }

     // Close the file
     MyReadFile.close();

     return tmpImage;
}

//O(1)
Color Image::getPixel(int i, int j) const
{
     assert((0 <= i && i <= height()) && (0 <= j && j <= width()));
     // i*widthVal + j will correspond to the pixel we are looking because:
     //  i*widthVal will take us to
     return pixelMatrix.at(i * widthVal + j);
}

//O(1)
Color Image::getPixel(int i) const
{
     assert(0 <= i && i <= height()*width());
     // i*widthVal + j will correspond to the pixel we are looking because:
     //  i*widthVal will take us to
     return pixelMatrix.at(i);
}

//O(1)
void Image::setPixel(int i, int j, Color col)
{
     assert((0 <= i && i < height()) && (0 <= j && j < width()));
     pixelMatrix.at(i * widthVal + j) = col;
}

//O(1)
void Image::setPixel(int i, Color col)
{
     assert(0 <= i && i < height()*width());
     pixelMatrix.at(i) = col;
}

//O(w*h)
void Image::DisplayImageInTerminal()
{
     for (int i = 0; i < heightVal * widthVal; i++)
     {
          if (i % widthVal == 0 && i != 0)
          {
               cout << endl;
          }
          cout << pixelMatrix.at(i).toInt();
     }
     cout << endl;
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
     for (i = 0; i < (int)myText.length(); i++)
     {
          if (myText[i] == ' ')
          {
               break;
          }
     }
     for (int j = myText.length() - i; j < (int)myText.length(); j++)
     {
          heightOfMatrixSTD[j - i - 1] = myText[j];
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
     cout << "width of matrix is " << myText << endl;
     int i;
     for (i = 0; i < (int)myText.length(); i++)
     {
          if (myText[i] == ' ')
          {
               break;
          }
          else
          {
               widthOfMatrixSTD[i] = myText[i];
          }
     }
     
     int widthOfMatrix = stoi(widthOfMatrixSTD);
     // Close the file
     MyReadFile.close();
     return widthOfMatrix;
}

int Image::toIndex(int i, int j) const{
     return i*width() + j;
}

std::pair<int, int> Image::toCoordinate(int k) const{
     //i = k / widthOfMatrix
     //j = k % widthOfMatrix
     std::pair<int, int> tmpPair;
     tmpPair.first = k / width();
     tmpPair.second = k % width();
     return tmpPair;
}

void Image::fill(Color c)
{
     int heightTimesWidth = heightVal * widthVal;
     for (int i = 0; i < heightTimesWidth; i++)
     {
          pixelMatrix.at(i) = c;
     }
}

int Image::getNumberOfPixels(){
     return pixelMatrix.size();
}

void Image::writeAIP(const std::string& filename) const {
     //creating an object of the ofstream class
     std::ofstream file;
     //creating a file in the images directory, which is a necessary step as we implemented our functions such as every aip files is in this directory.
     file.open("./images/" + filename + ".aip");

     //checking if the file hasn't been opened like it should have
     if (!file)
          throw std::runtime_error("error open file (writeAIP)");


     //writing in the first line of the aip file the width and height of the Image we're working with.
     file << to_string(width())
          << ' '
          << to_string(height());

     //writing on each line of the aip file the value of the Color of each pixel
     for (int i = 0; i < (int)pixelMatrix.size(); i++){
          //if condition needed as we represent our matrix of pixels by a single vector of Colors
          //if i%width=0 that means we are at the next line of the file. 
          //ex: if the width is 10, the color at pixelMatrix.at(10) is the first pixel of the second row because 10 = 10*1 + 0 
          if (i%width() == 0){
               file << endl;
          }
          file << to_string(pixelMatrix.at(i).toInt());
     }

     file.close();
}

//override of the == operator for the object of the class Image
//O(n) function in the worst case, Omeg(1) in best case when they don't have the same size
bool Image::operator==(const Image& img) const{
     //check in a first time if the size of the img given has a different size of our object or not
     //this is a O(1) function as size() uses the widthVal and heightVal of the Image class' objects, which is accessed in constant time
     if (img.size() != size()){
          return false;
     }
     for (int i = 0; i < size(); i++){
          if (pixelMatrix.at(i) != img.pixelMatrix.at(i)){
               return false;
          }
     }
     return true;
}

//override of the != operator for the object of the class Image
//O(n) function in the worst case, Omeg(1) in best case when they don't have the same size
bool Image::operator!=(const Image& img) const{
     //check in a first time if the size of the img given has a different size of our object or not
     //this is a O(1) function as size() uses the widthVal and heightVal of the Image class' objects, which is accessed in constant time
     if (img.size() != size()){
          return true;
     }
     for (int i = 0; i < size(); i++){
          if (pixelMatrix.at(i) == img.pixelMatrix.at(i)){
               return false;
          }
     }
     return true;
}

//O(1) function to check if pixels at position (i1,j1) and (i2,j2) are consecutive pixels, in one way or the other
bool Image::areConsecutivePixels(int i1, int j1, int i2, int j2){
     assert (i1 < size() && j1 < size() && i2 < size() && j2 < size());
     //as we are representing our matrix of pixels by a vector of Colors, we can just do i*widthVal + j and check the value if they are of by one
     //first part of the if condition corresponds to checking if the pixel in position i1,j1 is the pixel just before the pixel at i2,j2
     //second part of the condition corresponds to checking if the pixel in position i1,j1 is the pixel just after the pixel at i2,j2
     return ((i1*widthVal+j1+1 == i2*widthVal+j2) || (i1*widthVal+j1 == i2*widthVal+j2+1));
}

void Image::fillRectangle(int i1, int j1, int i2, int j2, Color c){
     assert(0<=i1 && i1<widthVal && 0<=i2 && i2<widthVal && 0<=j1 && j1<heightVal && 0<=j2 && j2<heightVal);
     for (int i = i1*widthVal+j1-1 ; i <= i2*widthVal+j2; i++){
          if (i%widthVal<=j2 && i%widthVal>=i1){
               pixelMatrix.at(i) = c;
          }
     }
}

void Image::addPixel(Color c){
     pixelMatrix.push_back(c);
}
// }