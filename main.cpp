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

  Image secondImage = Image::readAIP("img0");
  secondImage.DisplayImageInTerminal();
  secondImage.writeSVG("firstfiletest", 20);
  // cout << "width of matrix is: " << secondImage.getWidthOfMatrix("amazonie_0.aip") << endl;
  // cout << "height of matrix is: " << secondImage.getHeightOfMatrix("amazonie_0.aip") << endl;

  int index = secondImage.toIndex(2, 5);
  cout << "index is: " << index << endl;
  std::pair<int, int> testingPair = secondImage.toCoordinate(index);
  cout << "the pair is: (" << testingPair.first << ", " << testingPair.second << ")" << endl;
  cout << "nb of pixels inside is: " << secondImage.getNumberOfPixels() << endl;

  string fileName = "amazonie1";
  secondImage.writeAIP(fileName);
  Image thirdImage = Image::readAIP(fileName);
  thirdImage.writeSVG(fileName + "svg", 20);

  // bool testingbool = (secondImage==firstImage);
  cout << "are these pixels consecutive pixels? " << secondImage.areConsecutivePixels(1, 49, 1, 50) << endl;

  // Image fourthImage = Image(thirdImage);
  firstImage.DisplayImageInTerminal();
  cout << endl;
  firstImage.fillRectangle(3, 3, 6, 9, Color::Red);
  firstImage.fillRectangle(4, 5, 5, 8, Color::Black);
  // firstImage.DisplayImageInTerminal();

  Image fourthImage(firstImage);

  Image onethousand = Image(1000,1000);
  onethousand.writeAIP("img10");
  Analyst firstAnalyst = Analyst(makeRandomImage(3,3));
  firstAnalyst.getImage().DisplayImageInTerminal();
  //firstAnalyst.getImage().DisplayImageInTerminal();
  cout << endl;
  // firstAnalyst.DisplayLL();
  // cout << endl;
  // cout << "parent of 1 is " << firstAnalyst.find(1) << endl;
  // firstAnalyst.merge(1,2);
  // cout << "parent of 1 is " << firstAnalyst.find(1) << endl;
  // firstAnalyst.merge(6,5);
  // firstAnalyst.merge(5,2);
  firstAnalyst.DisplayLL();

  // // firstAnalyst.fillZone(1,1, Color::Red).DisplayImageInTerminal();
  // cout << "in the same zone? " << firstAnalyst.belongToTheSameZone(0, 0, 0, 23) << endl;
  // cout << "number of zones of color " << Color::Black << " " << firstAnalyst.nbZonesOfColor(Color::Black) << endl;

  // cout << endl;
  // set<int> tmpSet = firstAnalyst.zoneOfPixel(1, 1);
  // for (int item : tmpSet)
  // {
  //   cout << item << " ";
  // }

  // Image randomImage = makeRandomImage(10,10);
  // Image blackTest = Image(10,10);
  // randomImage.writeSVG("randomImage", 20);
  // randomImage.DisplayImageInTerminal();
  // FireSimulator tmpSimulator = FireSimulator(randomImage);


  // cout << endl;

  return 0;
}
