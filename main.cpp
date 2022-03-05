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

  Image firstImage = Image(10, 10);
  firstImage.setPixel(1, 2, Color::White);
  firstImage.setPixel(2, 2, Color::Blue);
  firstImage.DisplayImageInTerminal();

  cout << firstImage.height() << endl
       << firstImage.width() << endl
       << firstImage.size() << endl;

  Image secondImage = Image::readAIP("amazonie_0.aip");
  secondImage.DisplayImageInTerminal();
  secondImage.writeSVG("firstfiletest", 20);
  //cout << "width of matrix is: " << secondImage.getWidthOfMatrix("amazonie_0.aip") << endl;
  //cout << "height of matrix is: " << secondImage.getHeightOfMatrix("amazonie_0.aip") << endl;

  int index = secondImage.toIndex(2, 5);
  cout << "index is: " << index << endl;
  std::pair<int, int> testingPair = secondImage.toCoordinate(index);
  cout << "the pair is: (" << testingPair.first << ", " << testingPair.second << ")" << endl;
  cout << "nb of pixels inside is: " << secondImage.getNumberOfPixels() << endl;

  string fileName = "amazonie1";
  secondImage.writeAIP(fileName);
  Image thirdImage = Image::readAIP(fileName + ".aip");
  thirdImage.writeSVG(fileName + "svg", 20);

  //bool testingbool = (secondImage==firstImage);
  cout << "are these pixels consecutive pixels? " << secondImage.areConsecutivePixels(1,49,1,50) << endl;

  //Image fourthImage = Image(thirdImage);
  firstImage.DisplayImageInTerminal();
  cout << endl;
  firstImage.fillRectangle(3,3,6,9,Color::Red);
  //firstImage.DisplayImageInTerminal();

  Image fourthImage(firstImage);

  Analyst firstAnalyst = Analyst(firstImage);
  firstAnalyst.getImage().DisplayImageInTerminal();
  cout <<endl;

  firstAnalyst.fillZone(3,3, Color::Green).DisplayImageInTerminal();
  
  return 0;
}
