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

FireSimulator::FireSimulator(const Image &givenImage) : fireAnalyst(givenImage), modifiedImage(givenImage)
{
    for (int i = 1; i <= givenImage.size(); i++)
    {
        if (givenImage.getPixel(i) == Color::Green){
            pair<int, int> tmpCoordinates = givenImage.toCoordinate(i-1);
            set<int> tmpSet = fireAnalyst.zoneOfPixel(tmpCoordinates.first, tmpCoordinates.second);
            setOfForestPixels.insert(tmpSet.begin(), tmpSet.end());
        }
    }
    cout << endl;
    if (setOfForestPixels.size() > 0){
        int indexOfStartingPoint = rand() % setOfForestPixels.size();
        std::set<int>::iterator it = setOfForestPixels.begin();
        std::advance(it, indexOfStartingPoint);
        startingPoint = *it;
    }
    else{
        startingPoint = -1;
    }
    cout << "starting index is " << startingPoint << endl;
    cout << "size of the set is " << setOfForestPixels.size() << endl;
    stepCounter = 0;
}


void FireSimulator::nextStep(){
    if (startingPoint >= 0){
        if (stepCounter == 0){
            modifiedImage.setPixel(startingPoint+1, Color::Red);
            stepCounter++;
        }
        else {
            stepCounter=0;
        }
    }
}


void FireSimulator::advanceByNSteps(int n){
    while (stepCounter < n){
        nextStep();
    }
}

Image FireSimulator::getImageOfStepN(int n){
    while (stepCounter < n){
        nextStep();
    }
    return modifiedImage;
}

// make porportion threshold with the number of burning pixels (that we put in a vector
// then, take a random number between 1 and sizeOfFOrest*proportion
// then 


// burn them and add to a global array