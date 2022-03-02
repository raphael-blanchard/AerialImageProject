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

//library needed to read text files
#include <fstream>

using namespace std;

int main(void)
{
  srand(time(nullptr));


  Image testingImage = Image(5, 6);
  testingImage.setPixel(1, 4, Color::White);
  testingImage.setPixel(2, 3, Color::Blue);
  testingImage.DisplayImageInTerminal();
  for (int i = 1; i <= 6; i++){
    for (int j = 1; j <= 5; j++){
      cout << "color of pixel is: " << testingImage.getPixel(i, j) << endl;
    }
  }

  cout << testingImage.height() << endl
       << testingImage.width() << endl
       << testingImage.size() << endl;


  Image testingImage2 = Image::readAIP("amazonie_0.aip");
  testingImage2.DisplayImageInTerminal();
  testingImage2.writeSVG("firstfiletest", 20);
  cout << "width of matrix is: " << testingImage2.getWidthOfMatrix("amazonie_0.aip") << endl;
  cout << "height of matrix is: " << testingImage2.getHeightOfMatrix("amazonie_0.aip") << endl;
  return 0;
}
