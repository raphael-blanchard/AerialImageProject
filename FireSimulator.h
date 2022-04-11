////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#ifndef FIRE_SIMULATOR_H
#define FIRE_SIMULATOR_H

#include <vector>
#include "Image.h"
#include "Analyst.h"
#include <queue>

////////////////////////////////////////////////////////////////////////////////
/// This is a fire simulator.
///
/// Given a pixel of forest in an image, it simulates a fire in the zone
/// of forest of the given pixel.
////////////////////////////////////////////////////////////////////////////////
class FireSimulator {
public:

FireSimulator(const Image& givenImage);

//lets us go to the next step (t = t+1)
void nextStep();

//uses the nextStep function n times to advance by n steps (t = t + n at the end)
void advanceByNSteps(int n);

//returns the image at step n from beginning (at t = n)
Image getImageOfStepN(int n);


private:
//will be used to store the index of all of the pixels that are part of forest zones (aka zones of green pixels)
set<int> setOfForestPixels;

//queue that will store vectors of pixels that have been burnt by time
queue<vector<int>> queueOfBurntPixels;

//used to get the forest zones and to keep track of the zones while it burns
Analyst fireAnalyst;

//storing the step we are currently at
int stepCounter;
int startingPoint;
Image modifiedImage;
};

#endif
