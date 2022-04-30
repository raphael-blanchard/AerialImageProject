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
  //line needed to create a new seed each time we run the main. without, rand() function generates the same number as the first one it generates
  srand(time(nullptr));


  // // code to test our analyst on a 2000 by 2000 image
  // Image onethousand = makeRandomImage(2000,2000);
  // Analyst OneThousandAnalyst(onethousand);
  // cout << "number of zones in 2k*2k image: " << OneThousandAnalyst.nbZones() << endl;


  //CREATES A SIMULATOR WITH THE img0
  FireSimulator firstSimulator(Image::readAIP("img0"), 26,18);
  
  //UNCOMMENT LINE UNDER TO MAKE A SIMULATOR ON A WHOLE FOREST AND COMMENT LINE 112
  //Image fullForest = Image(30,30);
  //fullForest.fillRectangle(0,0,29,29, Color::Green);
  //FireSimulator firstSimulator(fullForest, 15,15);

  firstSimulator.Simulation();

  return 1;
}
