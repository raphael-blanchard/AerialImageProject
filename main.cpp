////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#include <cstdlib>
#include <ctime>
#include <sstream>
#include "FireSimulator.h"

// library needed to read text files
#include <fstream>

using namespace std;

int main(void)
{
  srand(time(nullptr));

  Image testingImage = Image(5, 10);
  testingImage.setPixel(1, 2, Color::White);
  testingImage.setPixel(2, 2, Color::Blue);
  testingImage.DisplayImageInTerminal();
  cout << "color that I got is: " << testingImage.getPixel(2, 2) << endl;
  for (int i = 1; i <= 6; i++)
  {
    for (int j = 1; j <= 5; j++)
    {
      cout << "color of pixel is: " << testingImage.getPixel(i, j) << endl;
    }
  }

  cout << testingImage.height() << endl
       << testingImage.width() << endl
       << testingImage.size() << endl;

  Image testingImage2 = Image::readAIP("amazonie_0.aip");
  testingImage2.DisplayImageInTerminal();
  testingImage2.writeSVG("firstfiletest", 20);
  //cout << "width of matrix is: " << testingImage2.getWidthOfMatrix("amazonie_0.aip") << endl;
  //cout << "height of matrix is: " << testingImage2.getHeightOfMatrix("amazonie_0.aip") << endl;

  int index = testingImage2.toIndex(2, 5);
  cout << "index is: " << index << endl;
  std::pair<int, int> testingPair = testingImage2.toCoordinate(index);
  cout << "the pair is: (" << testingPair.first << ", " << testingPair.second << ")" << endl;
  cout << "nb of pixels inside is: " << testingImage2.getNumberOfPixels() << endl;

  string fileName = "amazonie1";
  testingImage2.writeAIP(fileName);
  Image testingImage3 = Image::readAIP(fileName + ".aip");
  testingImage3.writeSVG(fileName + "svg", 20);

  return 0;
}
