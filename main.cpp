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
#include <unistd.h>

// library needed to read text files
#include <fstream>

using namespace std;

int main(void)
{
  srand(time(nullptr));

  // Image firstImage = Image(10, 10);
  // firstImage.setPixel(1, 2, Color::White);
  // firstImage.setPixel(2, 2, Color::Blue);
  // firstImage.DisplayImageInTerminal();

  // cout << firstImage.height() << endl
  //      << firstImage.width() << endl
  //      << firstImage.size() << endl;

  Image secondImage = Image::readAIP("img0");
  // secondImage.DisplayImageInTerminal();
  // secondImage.writeSVG("firstfiletest", 20);
  // // cout << "width of matrix is: " << secondImage.getWidthOfMatrix("amazonie_0.aip") << endl;
  // // cout << "height of matrix is: " << secondImage.getHeightOfMatrix("amazonie_0.aip") << endl;

  // int index = secondImage.toIndex(2, 5);
  // cout << "index is: " << index << endl;
  // std::pair<int, int> testingPair = secondImage.toCoordinate(index);
  // cout << "the pair is: (" << testingPair.first << ", " << testingPair.second << ")" << endl;
  // cout << "nb of pixels inside is: " << secondImage.getNumberOfPixels() << endl;

  string fileName = "amazonie1";
  secondImage.writeAIP(fileName);
  Image thirdImage = Image::readAIP(fileName);
  thirdImage.writeSVG(fileName, 20);

  // // bool testingbool = (secondImage==firstImage);
  // cout << "are these pixels consecutive pixels? " << secondImage.areConsecutivePixels(1, 49, 1, 50) << endl;

  // // Image fourthImage = Image(thirdImage);
  // firstImage.DisplayImageInTerminal();
  // cout << endl;
  // firstImage.fillRectangle(3, 3, 6, 9, Color::Red);
  // firstImage.fillRectangle(4, 5, 5, 8, Color::Black);
  // // firstImage.DisplayImageInTerminal();

  // Image fourthImage(firstImage);

  // //Image onethousand = makeRandomImage(2000,2000);
  // //onethousand.writeAIP("img10");
  Image test = Image(3,3);
  test.setPixel(1, Color::makeColor(1));
  test.setPixel(2, Color::makeColor(4));
  test.setPixel(3, Color::makeColor(4));
  test.setPixel(4, Color::makeColor(1));
  test.setPixel(5, Color::makeColor(2));
  test.setPixel(6, Color::makeColor(2));
  test.setPixel(7, Color::makeColor(2));
  test.setPixel(8, Color::makeColor(4));
  test.setPixel(9, Color::makeColor(2));
  Analyst firstAnalyst = Analyst(test);
  firstAnalyst.getImage().DisplayImageInTerminal();

  // firstAnalyst.getImage().DisplayImageInTerminal();
  // //firstAnalyst.getImage().DisplayImageInTerminal();
  // cout << endl;
  // // firstAnalyst.DisplayLL();
  // // cout << endl;
  // // cout << "parent of 1 is " << firstAnalyst.find(1) << endl;
  // // firstAnalyst.merge(1,2);
  // // cout << "parent of 1 is " << firstAnalyst.find(1) << endl;
  // // firstAnalyst.merge(6,5);
  // // firstAnalyst.merge(5,2);
  // firstAnalyst.DisplayLL();

  // cout << endl; 
  // cout << firstAnalyst.belongToTheSameZone(1, 2, 2, 2) << endl;
  // set<int> tmpset = firstAnalyst.zoneOfPixel(1,1);
  // int i = 1;
  // pair<int, int> coordinates = test.toCoordinate(i);
  // cout << "i is " << coordinates.first << " and j is " << coordinates.second << endl;


  // vector<pair<int, int>> test1 = {{1,2}, {3,4}, {5,6}};
  // test1.erase(test1.begin()+2);
  // for (int i = 0; i < (int)test1.size(); i++){
  //   cout << test1[i].first << " - " << test1[i].second << endl;
  // }
  // cout << endl;
  // cout << (int)test1.size() << endl;
  // test1.push_back({7,8});
  // test1.push_back({10,11});
  // for (int i = 0; i < (int)test1.size(); i++){
  //   cout << test1[i].first << " - " << test1[i].second << endl;
  // }
  FireSimulator firstSimulator = FireSimulator(Image::readAIP("img0"), 28,15);
  Image fullForest = Image(30,30);
  fullForest.fillRectangle(0,0,29,29, Color::Green);
  //FireSimulator firstSimulator = FireSimulator(fullForest, 1,1);
  //cout << "Start of the simulation, press 1 and enter to continue, 0 to stop" << endl;
  // firstSimulator.nextStep();
  // cout << "first potential burns: " << endl;
  //firstSimulator.nextStep();
  // firstSimulator.nextStep();


  // Image firstOne = firstSimulator.getImageOfStepN(35);
  // Image secondOne = firstSimulator.getImageOfStepN(36);
  // Image thirdOne = firstSimulator.getImageOfStepN(37);
  // Image fourthOne = firstSimulator.getImageOfStepN(77);
  // firstOne.writeSVG("firstOne", 10);
  // secondOne.writeSVG("secondOne", 10);
  // thirdOne.writeSVG("thirdOne", 10);
  // fourthOne.writeSVG("fourthOne", 10);
  string simulationInput;
  int simulationStep = 0;
  cout << "Start of the simulation." << endl << "ENTER key to continue, ANY OTHER key to stop" << endl;
  getline(cin, simulationInput);
  while (simulationInput.size() == 0){
    Image simulationImg = firstSimulator.getImageOfStepN(simulationStep);
    simulationImg.writeSVG("simulationImg", 10);
    simulationStep++;
    // cout <<  "ENTER key to continue, ANY OTHER key to stop" << endl;
    // getline(cin, simulationInput);
    sleep(1);
  }


  int rndtest = rand() % 100;
  cout << "rnd trest is " << rndtest << endl;



  

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
