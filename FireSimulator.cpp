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
    startingPoint = (given_i - 1) * modifiedImage.width() + (given_j - 1) + 1;
    stepCounter = 0;
    nbOfPotentialBurns = 0;

    modifiedImage.DisplayImageInTerminal();
    cout << endl;
}


// function to check if the pixel at index (starting from 0) givenIndex can be border (not surrounded by Red pixels)
bool FireSimulator::isValid(int givenIndex)
{
    assert(givenIndex >= 0 && givenIndex < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1) == Color::Red);

    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Green)
    {
        return true;
    }

    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Green)
    {
        return true;
    }

    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Green)
    {
        return true;
    }

    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Green)
    {
        return true;
    }
    return false;
}

void FireSimulator::updateNbOfPotentialBurns(int givenIndex)
{
    assert(givenIndex >= 0 && givenIndex < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1) == Color::Red);

    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Green)
    {
        nbOfPotentialBurns++;
    }

    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Green)
    {
        nbOfPotentialBurns++;
    }

    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Green)
    {
        nbOfPotentialBurns++;
    }

    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Green)
    {
        nbOfPotentialBurns++;
    }
}

int FireSimulator::getNbOfNeighbors(int givenIndex){
    assert(givenIndex >= 0 && givenIndex < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1) == Color::Red);
    int total = 0;
    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Green)
    {
        total++;
    }

    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Green)
    {
        total++;
    }

    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Green)
    {
        total++;
    }

    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Green)
    {
        total++;
    }

    return total;
}


void FireSimulator::displayBorder(){
    for (int i = 0; i < (int)border.size(); i++){
        cout << "|" << border[i].first << " and " << border[i].second << "|" << endl;
    }
}

void FireSimulator::updateBorder(int givenIndex){
    assert(givenIndex >= 0 && givenIndex < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1) == Color::Red);
    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Red)
    {
        border[givenIndex - 1].second--;
        if (border[givenIndex - 1].second == 0){
            border.erase(border.begin() + givenIndex - 1 - 1);
        }
    }

    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Red)
    {
        border[givenIndex + 1].second--;
        if (border[givenIndex + 1].second == 0){
            border.erase(border.begin() + givenIndex + 1 - 1);
        }
    }

    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Red)
    {
        border[givenIndex - modifiedImage.width()].second--;
        if (border[givenIndex - modifiedImage.width()].second == 0){
            border.erase(border.begin() + givenIndex - modifiedImage.width() - 1);
        }
    }

    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Red)
    {
        border[givenIndex + modifiedImage.width()].second--;
        if (border[givenIndex + modifiedImage.width()].second == 0){
            border.erase(border.begin() + givenIndex + modifiedImage.width() - 1);
        }
    }
}


// Direction index:
// top := 0
// right := 1
// bottom := 2
// left := 3
void FireSimulator::nextStep()
{
    // no assertion needed for the starting point as we previously checked it in the constructor
    if (stepCounter == 0)
    {
        cout << startingPoint << endl;
        //cout << border[0].first << " and " << border[0].second << endl;
        modifiedImage.setPixel(startingPoint, Color::Red);
        border.push_back({startingPoint - 1, getNbOfNeighbors(startingPoint-1)});
        modifiedImage.DisplayImageInTerminal();
        updateNbOfPotentialBurns(startingPoint - 1);
        stepCounter++;
    }
    else
    {
        // change this later on with gaussian probabilities to get a more realistic approach of the simulation
        int randNbOfPixels = (rand() % nbOfPotentialBurns) + 1; // getting a random number of pixels that will start to burn during this step
        cout << "maximum that we can burn is " << nbOfPotentialBurns << endl;
        cout << "number of pixel we will burn: " << randNbOfPixels << endl;

        // vector we use to store the indexes of pixels we will burn in the loop
        // which will be used later on to get the new borders after everything in this step that needs to be burning is burning
        vector<int> vectOfFutureBorders = {};
        for (int i = 0; i < randNbOfPixels; i++)
        {
            cout << "WHEN IS SEGFAULT 1" << endl;
            cout << "size of border is " << (int)border.size() << endl;
            displayBorder();
            // for (int i = 0; i < (int)border.size(); i++)
            // {
            //     cout << border[i].first << " - " << border[i].second << " ; ";
            // }
            cout << endl;
            int randomIndex = rand() % (int)border.size(); // rnd number from 0 to the size of the border
            cout << "WHEN IS SEGFAULT 1bis" << endl;
            cout << "the random index is " << randomIndex << endl;
            cout << "WHEN IS SEGFAULT 1bisbis" << endl;
            cout << "value at random index is: " << border[randomIndex].first << endl;

            cout << "WHEN IS SEGFAULT 2" << endl;
            // getting a vector of the pixels we can burn from the pixel at index border[randomIndex]
            //(eg. pixel at index 0 can only burn the one on its right and under itself (if they are forest pixels))
            // so tmpVector will be [1,50] in this specific example
            vector<int> tmpVector = getBurnablePixels(border[randomIndex].first);
            cout << "WHEN IS SEGFAULT 3" << endl;
            cout << "WHEN IS SEGFAULT 3bis" << endl;
            for (int item : tmpVector)
            {
                cout << item << endl;
            }
            cout << endl;
            cout << "WHEN IS SEGFAULT 4" << endl;
            // getting a random index of the tmpVector containing pixels we want to burn
            int randomPixelOfVector = rand() % (int)tmpVector.size();
            cout << "burning at index " << tmpVector[randomPixelOfVector] << endl;
            // burning the pixel we got
            cout << "WHEN IS SEGFAULT 5" << endl;
            modifiedImage.setPixel(tmpVector[randomPixelOfVector] + 1, Color::Red);
            //decrementing the number of pixels the pixel at index border[randomIndex] can burn
            updateBorder(tmpVector[randomPixelOfVector]);
            
            //updateBorder();

            cout << "WHEN IS SEGFAULT 6" << endl;
            vectOfFutureBorders.push_back(tmpVector[randomPixelOfVector]);

            // //storing the index of the pixel into the vectOfFutureBorders to later on define the new borders (+1 as addPossiblePixelsToBorder takes an input >= 1)
            // vectOfFuturePotentialBurns.push_back(tmpVector[randomPixelOfVector]+1);

            cout << "WHEN IS SEGFAULT 7" << endl;
            // delete from not burned border
            updateNbOfPotentialBurns(border[randomIndex].first);
            cout << "WHEN IS SEGFAULT 8" << endl;
            modifiedImage.DisplayImageInTerminal();
        }
        // for (int i = 0; i < (int)vectOfFuturePotentialBurns.size(); i++){
        //     //adding the new potential burning pixels to the vector
        //     addPossiblePixelsToBorder(vectOfFuturePotentialBurns[i]);
        // }

        cout << "invalid pointer under ??" << endl;
        cout << "size of the vect " << (int)vectOfFutureBorders.size() << endl;
        for (int i = 0; i < (int)vectOfFutureBorders.size(); i++)
        {
            cout  << "vectOfFutureBorders[i]=" << vectOfFutureBorders[i] << endl;
            // added the pixel at index tmpVector[randomPixelOfVector] to the border vector if is not surrounded by red pixels
            if (isValid(vectOfFutureBorders[i]))
            {
                int first = vectOfFutureBorders[i];
                int second = getNbOfNeighbors(vectOfFutureBorders[i]);
                pair<int, int> tmpPair = {first, second};
                border.push_back(tmpPair);
            }
            displayBorder();
        }
        
        stepCounter++;
    }
}

vector<int> FireSimulator::getBurnablePixels(int givenIndex)
{
    // cout << "start of burnable function" << endl;
    // cout << "given index is " << givenIndex << endl;
    assert(givenIndex >= 0);
    assert(givenIndex < modifiedImage.size());
    assert(modifiedImage.getPixel(givenIndex + 1) == Color::Red);
    vector<int> tmpVector;
    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Green)
    {
        tmpVector.push_back(givenIndex - 1);
    }

    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Green)
    {
        tmpVector.push_back(givenIndex + 1);
    }

    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Green)
    {
        tmpVector.push_back(givenIndex - modifiedImage.width());
    }

    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Green)
    {
        tmpVector.push_back(givenIndex + modifiedImage.width());
    }
    return tmpVector;
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
    advanceByNSteps(n);
    return modifiedImage;
}

// make porportion threshold with the number of burning pixels (that we put in a vector
// then, take a random number between 1 and sizeOfFOrest*proportion
// then

// burn them and add to a global array