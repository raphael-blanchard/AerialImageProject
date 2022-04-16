////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "Analyst.h"
#include "FireSimulator.h"

FireSimulator::FireSimulator(const Image &givenImage, int given_i, int given_j) : fireAnalyst(givenImage), modifiedImage(givenImage)
{
    assert(given_i > 0 && given_i <= modifiedImage.height() && given_j > 0 && given_j <= modifiedImage.width() && modifiedImage.getPixel(given_i, given_j) == Color::Green);
    // converting to a single dimension coordinate
    startingPoint = given_i * modifiedImage.width() + given_j;
    stepCounter = 0;

    // if we want to randomize the start of the index
    //  for (int i = 1; i <= givenImage.size(); i++)
    //  {
    //      if (givenImage.getPixel(i) == Color::Green){
    //          pair<int, int> tmpCoordinates = givenImage.toCoordinate(i-1);
    //          set<int> tmpSet = fireAnalyst.zoneOfPixel(tmpCoordinates.first, tmpCoordinates.second);
    //          setOfForestPixels.insert(tmpSet.begin(), tmpSet.end());
    //      }
    //  }
    //  cout << endl;
    //  if (setOfForestPixels.size() > 0){
    //      int indexOfStartingPoint = rand() % setOfForestPixels.size();
    //      std::set<int>::iterator it = setOfForestPixels.begin();
    //      std::advance(it, indexOfStartingPoint);
    //      startingPoint = *it;
    //  }
    //  else{
    //      startingPoint = -1;
    //  }
    //  cout << "starting index is " << startingPoint << endl;
    //  cout << "size of the set is " << setOfForestPixels.size() << endl;
}



//Directions:
//top := 1
//right := 2
//bottom := 3
//left := 4
void FireSimulator::nextStep()
{
    //no assertion needed for the starting point as we previously checked it in the constructor
    if (stepCounter == 0)
    {
        modifiedImage.setPixel(startingPoint, Color::Red);
        stepCounter++;
        //when the simulation starts, only one pixel is burning, which means it can reach the pixel on top, left, bottom and right of himself on the next step.
        nbOfPossibleBurntPixels = 4;
    }
    else
    {
        int randNbOfPixels = rand() % nbOfPossibleBurntPixels;//getting a random number of pixels that will start to burn during this step
        
        stepCounter++;
    }
}

void FireSimulator::advanceByNSteps(int n)
{
    while (stepCounter < n)
    {
        nextStep();
    }
}

Image FireSimulator::getImageOfStepN(int n)
{
    while (stepCounter < n)
    {
        nextStep();
    }
    return modifiedImage;
}

// make porportion threshold with the number of burning pixels (that we put in a vector
// then, take a random number between 1 and sizeOfFOrest*proportion
// then

// burn them and add to a global array