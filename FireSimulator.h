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
    void nextStep();

    // uses the nextStep function n times to advance by n steps (t = t + n at the end)
    void advanceByNSteps(int n);

    // returns the image at step n from beginning (at t = n)
    Image getImageOfStepN(int n);

    //function that adds the pixels around the pixel at index givenIndex (starting at 1) to the vector indexOfPossibleBurningPixels if they are valid
    //void addPossiblePixelsToBorder(int givenIndex);

    void updateNbOfPotentialBurns(int givenIndex);

    //set<int> getVectOfBurning();

    vector<int> getBurnablePixels(int givenIndex); //1 <= givenIndex <= sizeofImage

    bool isValid(int givenIndex);

    void updateBorder(int givenIndex);

    int getNbOfNeighbors(int givenIndex);

    void displayBorder();
private:
    // will be used to store the index of all of the pixels that are part of forest zones (aka zones of green pixels)
    // set<int> setOfForestPixels;

    // queue that will store vectors of pixels that have been burnt by time
    queue<vector<int>> queueOfBurntPixels;

    // used to get the forest zones and to keep track of the zones while it burns
    Analyst fireAnalyst;

    set<int> indexOfPossibleBurningPixels;
    vector<pair<int, int>> border;//vector containing indexes (starting from 0) of pixels on the border

    // storing the step we are currently at
    int stepCounter;
    int startingPoint;
    Image modifiedImage;
    vector< pair<Color, bool> > vectOfPairs; // used to check if a pixel has already been added to the indexOfPossibleBurningPixels before adding them;
    int nbOfPotentialBurns;
};

#endif
