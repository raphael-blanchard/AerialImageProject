////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include "Color.h"

//used to represent the matrix of pixels of images
#include <vector>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
/// This a rectangular colored image.
////////////////////////////////////////////////////////////////////////////////
class Image {
public:

  /// Creates a black rectangular image with w*h pixels
  /// w is the width of this image, h is its height
  Image(int w, int h);

  Image();

  /// Destructor
  //not needed
  //~Image();

  /// Copy constructor
  Image(const Image& img);

  /// No assignment
  Image& operator=(const Image& img) = delete;

  /// Returns the width of this
  int width() const;

  /// Returns the height of this
  int height() const;

  void setWidth(int n) ;

  void setHeight(int n) ;

  /// Returns the number of pixels of this
  int size() const;

  //returns the vector of Colors
  vector<Color> getVectorOfColors();

  /// Gets the pixel in the i-th row and j-th column
  /// 1 <= i <= height(), 1 <= j <= width()
  Color getPixel(int i, int j) const;

  //override of the getPixel function but this time getting the pixel at index i in the vector of Colors
  // 0 <= i <= width()*height()
  Color getPixel(int i) const;

  /// Sets the pixel in the i-th row and j-th column
  /// 1 <= i <= height(), 1 <= j <= width()
  /// col is the new color
  void setPixel(int i, int j, Color col);

  //override of the setPixel function but this time setting the pixel at index i
  // 0 <= i <= width()*height()
  void setPixel(int i, Color c);

  /// Maps a coordinate (i, j) to a unique identifier
  int toIndex(int i, int j) const;
  // since we know that w > h and i < h and j < w
  //toIndex will return i x w + j
  //to find i in the toCoordinate function we will do the integer division of the int given
  //in argument by the width of the matrix; To find j, we'll do the argument given modulo the
  //width of the matrix
  //i = (i*w+j)/w
  //j = (i*w+j)%w

  /// Maps an identifier to the corresponding coordinates
  /// Given the result p, p.first is the row and p.second is the column 
  std::pair<int, int> toCoordinate(int k) const;


  /// Fills this with a given color
  void fill(Color c);

  /// Fills a rectangle in this with a given color
  /// (i1, j1) is the upper left corner
  /// (i2, j2) is the lower right corner
  void fillRectangle(int i1, int j1, int i2, int j2, Color c);
  
  /// Generates a SVG image given a filename without the extension
  /// The file name is of this image is filename.svg
  /// Each pixel is represented by a square of side pixelSize
  /// Throws an exception std::runtime_error if an error occurs
  void writeSVG(const std::string& filename, int pixelSize) const;

  /// Saves this in a text file using a specific format:
  ///   - width and height of this on the first line;
  ///   - followed by the lines of this such that each digit
  ///     corresponds to the color of the corresponding pixel.
  /// An example of output file follows:
  ///   5 3
  ///   10014
  ///   01233
  ///   23101
  /// The output file is named filename.aip
  /// Throws an exception std::runtime_error if an error occurs
  void writeAIP(const std::string& filename) const;

  /// Creates an image from an AIP file
  /// The file name must be given without the extension
  /// Throws an exception std::runtime_error if an error occurs
  static Image readAIP(const std::string& filename);

  /// Returns true if this and img are equal
  bool operator==(const Image& img) const;

  /// Returns true if this and img are different
  bool operator!=(const Image& img) const;

  /// Returns true if (i1, j1) and (i2, j2) are two consecutive pixels of this
  bool areConsecutivePixels(int i1, int j1, int i2, int j2);

  //function to display a member of the Image class in the terminal
  void DisplayImageInTerminal();

  //function to get the height of the matrix mentionned in the first line of an AIP file
  static int getHeightOfMatrix(const std::string &filename);
  //function to get the width of the matrix mentionned in the first line of an AIP file
  static int getWidthOfMatrix(const std::string &filename);

  int getNumberOfPixels();

  //function to add a pixel to the pixel matrix of an image
  void addPixel(Color c);

private:

  //start of my modifications:

  //corresponding to the height and width of the image
  int heightVal;
  int widthVal;
  
  //We initially went with the representation of the matrix in a vector of vectors of colors, but after some research on the differences
  //we decided to go with the representation of the matrix of pixels by a single vector of colors because of multiple reasons:
  // - vector of vectors would mean non contiguous memory on the part of the underlying vectors (the columns). Representing the matrix by a single vector
  // would give use contiguous memory. Using a vector of vectors shouldn't change much the time of completion by the computer thanks to how c++ is made but it will
  // require the cpu to work a little more because of less efficient memory caching. However, even if it doesn't change the performance by much, it doesn't mean
  // we should make the cpu work more if we can find a workaround.


  //representing the matrix of pixels by a vector of colors that will be of size w*h. The method of storage here is in row-major order.
  vector<Color> pixelMatrix;
  

  /// Tests if (i,j) is the coordinates of some pixel of this
  //bool isValidCoordinate(int i, int j) const;
};

/// Generates an image of width w and height h such that each pixel is
/// selected randomly.
Image makeRandomImage(int w, int h);

#endif
