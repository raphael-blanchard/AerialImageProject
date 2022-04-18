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
#include <unordered_map>

////////////////////////////////////////////////////////////////////////////////
/// This is a fire simulator.
///
/// Given a pixel of forest in an image, it simulates a fire in the zone
/// of forest of the given pixel.
////////////////////////////////////////////////////////////////////////////////
class FireSimulator
{
public:
    FireSimulator(const Image &givenImage, int given_i, int given_j);

    // lets us go to the next step (t = t+1)
    void nextStep() throw (int);

    // uses the nextStep function n times to advance by n steps (t = t + n at the end)
    void advanceByNSteps(int n);

    // returns the image at step n from beginning (at t = n)
    Image getImageOfStepN(int n);

    //function that adds the pixels around the pixel at index givenIndex (starting at 1) to the vector indexOfPossibleBurningPixels if they are valid
    void addToPotentialBurningPixels(int givenIndex);

    void addToPotentialAshes(int givenIndex);
private:

    // used to get the forest zones and to keep track of the zones while it burns
    Analyst fireAnalyst;


    // storing the step we are currently at
    int stepCounter;
    int startingPoint;
    Image modifiedImage;

    // used to check if a pixel has already been added to the indexOfPossibleBurningPixels before adding them (first bool)
    //and check if they have been to the potentialAshes (second bool)
    vector< pair<Color, pair<bool, bool> > > vectOfPairs; 
    vector<int> potentialBurningPixels;
    vector<int> potentialAshes;
    


};

#endif
