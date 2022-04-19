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
    // nbOfPotentialBurns = 0;
    for (int i = 0; i < (int)modifiedImage.size(); i++)
    {
        //{false, false} : the first one is for the burning pixels, second one is for the ashes
        vectOfPairs.push_back({modifiedImage.getPixel(i + 1), {false, false}});
    }

    modifiedImage.DisplayImageInTerminal();
    cout << endl;
}

// function that adds the indexes of the pixels that can be burned in the step we are at
void FireSimulator::addToPotentialBurningPixels(int givenIndex)
{
    assert(givenIndex >= 0 && givenIndex < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1) == Color::Red);
    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Green && vectOfPairs[givenIndex - 1].second.first == false && (givenIndex-1)%modifiedImage.width() < givenIndex%modifiedImage.width())
    {
        // add this index to the potential burning pixels
        potentialBurningPixels.push_back(givenIndex - 1);
        // making the bool true to indicate that this pixel has already been added to the potential burning pixels, which will make him never be able to be added again
        vectOfPairs[givenIndex - 1].second.first = true;
    }

    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Green && vectOfPairs[givenIndex + 1].second.first == false && (givenIndex+1)%modifiedImage.width() > givenIndex%modifiedImage.width())
    {
        potentialBurningPixels.push_back(givenIndex + 1);
        vectOfPairs[givenIndex + 1].second.first = true;
    }

    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Green && vectOfPairs[givenIndex - modifiedImage.width()].second.first == false)
    {
        potentialBurningPixels.push_back(givenIndex - modifiedImage.width());
        vectOfPairs[givenIndex - modifiedImage.width()].second.first = true;
    }

    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Green && vectOfPairs[givenIndex + modifiedImage.width()].second.first == false)
    {
        potentialBurningPixels.push_back(givenIndex + modifiedImage.width());
        vectOfPairs[givenIndex + modifiedImage.width()].second.first = true;
    }
}

// function that adds the indexes of the pixels that can be burned in the step we are at
void FireSimulator::addToPotentialAshes(int givenIndex)
{
    if (givenIndex < 0 || givenIndex > modifiedImage.size() || modifiedImage.getPixel(givenIndex + 1) != Color::Black)
    {
        return;
    }

    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Red && vectOfPairs[givenIndex - 1].second.second == false && (givenIndex-1)%modifiedImage.width() < givenIndex%modifiedImage.width())
    {
        // add this index to the potential burning pixels
        potentialAshes.push_back(givenIndex - 1);
        // making the bool true to indicate that this pixel has already been added to the potential burning pixels, which will make him never be able to be added again
        vectOfPairs[givenIndex - 1].second.second = true;
        // addToPotentialAshes(givenIndex - 1);
    }

    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Red && vectOfPairs[givenIndex + 1].second.second == false && (givenIndex+1)%modifiedImage.width() > givenIndex%modifiedImage.width())
    {
        potentialAshes.push_back(givenIndex + 1);
        vectOfPairs[givenIndex + 1].second.second = true;
        // addToPotentialAshes(givenIndex + 1);
    }

    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Red && vectOfPairs[givenIndex - modifiedImage.width()].second.second == false)
    {
        potentialAshes.push_back(givenIndex - modifiedImage.width());
        vectOfPairs[givenIndex - modifiedImage.width()].second.second = true;
        // addToPotentialAshes(givenIndex - modifiedImage.width());
    }

    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Red && vectOfPairs[givenIndex + modifiedImage.width()].second.second == false)
    {
        potentialAshes.push_back(givenIndex + modifiedImage.width());
        vectOfPairs[givenIndex + modifiedImage.width()].second.second = true;
        // addToPotentialAshes(givenIndex - modifiedImage.width());
    }
}

void FireSimulator::nextStep() throw(int)
{
    if ( stepCounter>0 && (int)potentialBurningPixels.size() == 0 && (int)potentialAshes.size() == 0)
    {
        throw(stepCounter);
        return;
    }
    // no assertion needed for the starting point as we previously checked it in the constructor
    if (stepCounter == 0)
    {
        cout << startingPoint << endl;
        // cout << border[0].first << " and " << border[0].second << endl;
        modifiedImage.setPixel(startingPoint, Color::Red);
        addToPotentialBurningPixels(startingPoint - 1); // starting point can be 1 but the input to this function can be 0
        stepCounter++;
    }
    else
    {
        vector<int> indexOfBurning = {};
        vector<int> indexOfBurnt = {};
        if ((int)potentialBurningPixels.size() > 0)
        {
            int rndNbOfPixelsBurned = rand() % (int)potentialBurningPixels.size() + 1;
            for (int i = 0; i < rndNbOfPixelsBurned; i++)
            {
                int rndIndex = rand() % (int)potentialBurningPixels.size(); // getting a random index of the vector potentialBurningPixels

                modifiedImage.setPixel(potentialBurningPixels[rndIndex] + 1, Color::Red);

                // adding the index to a tmp vector, which we will iterate through at the end of the loop to update the border
                indexOfBurning.push_back(potentialBurningPixels[rndIndex]);
                // addToPotentialBurningPixels(potentialBurningPixels[rndIndex]);
                potentialBurningPixels.erase(potentialBurningPixels.begin() + rndIndex);
            }
        }

        // when ashes start to appear
        if (stepCounter > 3)
        {
            if (stepCounter == 4)
            {
                modifiedImage.setPixel(startingPoint, Color::Black);
                addToPotentialAshes(startingPoint - 1);
            }
            else
            {
                int rndNbOfAshes = rand() % (int)potentialAshes.size() + 1;
                for (int i = 0; i < rndNbOfAshes; i++)
                {
                    int rndIndex = rand() % (int)potentialAshes.size(); // getting a random index of the vector potentialBurningPixels
                    modifiedImage.setPixel(potentialAshes[rndIndex] + 1, Color::Black);
                    cout << "index of ash is " << potentialAshes[rndIndex] << endl;

                    // adding the index to a tmp vector, which we will iterate through at the end of the loop to update the border
                    indexOfBurnt.push_back(potentialAshes[rndIndex]);
                    // updating the pixels that can be burned in this step
                    // addToPotentialAshes(potentialAshes[rndIndex]);
                    potentialAshes.erase(potentialAshes.begin() + rndIndex);
                }
            }
        }
        cout << "step counter is " << stepCounter << endl;
        for (int i = 0; i < (int)indexOfBurning.size(); i++)
        {
            cout << (indexOfBurning[i]) << " | ";
        }
        cout << endl;
        for (int i = 0; i < (int)indexOfBurnt.size(); i++)
        {
            cout << (indexOfBurnt[i]) << " - ";
        }
        cout << endl;

        // updating the borders
        for (int i = 0; i < (int)indexOfBurning.size(); i++)
        {
            addToPotentialBurningPixels(indexOfBurning[i]);
        }
        for (int i = 0; i < (int)indexOfBurnt.size(); i++)
        {
            addToPotentialAshes(indexOfBurnt[i]);
        }

        cout << "added to the burning pixels and ashes" << endl;
        for (int i = 0; i < (int)potentialBurningPixels.size(); i++)
        {
            cout << (potentialBurningPixels[i]) << " | ";
        }
        cout << endl;
        for (int i = 0; i < (int)potentialAshes.size(); i++)
        {
            cout << (potentialAshes[i]) << " - ";
        }
        cout << endl;
        stepCounter++;
    }
    // modifiedImage.DisplayImageInTerminal();
}

void FireSimulator::advanceByNSteps(int n)
{
    while (stepCounter < n)
    {
        try
        {
            nextStep();
        }
        catch (int e)
        {
            cout << "END OF THE SIMULATION. YOU HAVE BURNED DOWN A FOREST." <<endl;
            cout << "IT TOOK " << e << " STEPS." << endl;
            exit(1);
        }
    }
    modifiedImage.writeSVG("fireSimulator1", 10);
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