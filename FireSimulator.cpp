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


//O(n) - n being the number of pixels in the image
FireSimulator::FireSimulator(const Image &givenImage, int given_i, int given_j) : fireAnalyst(givenImage), modifiedImage(givenImage) //calling the constructors of fireAnalyst and modifiedImage
{
    assert(given_i > 0 && given_i <= modifiedImage.height() && given_j > 0 && given_j <= modifiedImage.width() && modifiedImage.getPixel(given_i, given_j) == Color::Green);
    //converting the given coordinates to a single dimension coordinate
    startingPoint = (given_i - 1) * modifiedImage.width() + (given_j - 1) + 1;
    //initializing the stepCounter
    stepCounter = 0;
    
    //initializing the matrix of pair < Color, pair<bool,bool>> that will be used in other function to check if a certain pixel has already been added to the borders
    for (int i = 0; i < (int)modifiedImage.size(); i++)
    {
        //{false, false} : the first one is for the burning pixels, second one is for the ashes
        vectOfPairs.push_back({modifiedImage.getPixel(i + 1), {false, false}}); // O(1) call
    }
}

//O(1) as it is only using constant time functions
//function that adds the indexes of the pixels that can be burned during the step after the one we are at

void FireSimulator::addToPotentialBurningPixels(int givenIndex)
{
    //function called after we started to burn a pixel
    //assertion makes sure the pixel at coordinates givenIndex is indeed, burning
    assert(givenIndex >= 0 && givenIndex < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1) == Color::Red);

    //checking if the pixel on the left of the pixel at coordinates givenIndex is valid to add in the border of pixels that can be burned
    //for that: the index has to be valid; the pixel at this index has to be green; 
    //the pixel has to not have been added to the potentialBurningPixels (the pixel on the left of someone can be the pixel under another one), for that we check if the first boolean (the one corresponding to being added to burn border or not)
    //lastly, we have to make sure that the pixel is not on the other side of the image (the left pixel of one that is all the way to the left of the image cannot be valid as it is the other side of the "map")
    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Green && vectOfPairs[givenIndex - 1].second.first == false && (givenIndex-1)%modifiedImage.width() < givenIndex%modifiedImage.width())
    {
        // add this index to the potential burning pixels
        potentialBurningPixels.push_back(givenIndex - 1);
        // making the bool true to indicate that this pixel has already been added to the potential burning pixels, which will make him never be able to be added again
        vectOfPairs[givenIndex - 1].second.first = true;
    }

    //same idea but this time for the pixel on the right
    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Green && vectOfPairs[givenIndex + 1].second.first == false && (givenIndex+1)%modifiedImage.width() > givenIndex%modifiedImage.width())
    {
        potentialBurningPixels.push_back(givenIndex + 1);
        vectOfPairs[givenIndex + 1].second.first = true;
    }
    //same idea but this time for the pixel on the top
    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Green && vectOfPairs[givenIndex - modifiedImage.width()].second.first == false)
    {
        potentialBurningPixels.push_back(givenIndex - modifiedImage.width());
        vectOfPairs[givenIndex - modifiedImage.width()].second.first = true;
    }
    //same idea but this time for the pixel on the bottom
    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Green && vectOfPairs[givenIndex + modifiedImage.width()].second.first == false)
    {
        potentialBurningPixels.push_back(givenIndex + modifiedImage.width());
        vectOfPairs[givenIndex + modifiedImage.width()].second.first = true;
    }
}

//same idea as previous function, but this time for pixels that can become ashes in the next step
//O(1) as it is only using constant time functions
// function that adds the indexes of the pixels that can be burned in the step we are at
void FireSimulator::addToPotentialAshes(int givenIndex)
{
    //function called after the pixel at coordinates givenIndex became ashes
    //assertion makes sure the pixel at coordinates givenIndex is indeed, ashes
    assert(givenIndex >= 0 && givenIndex < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1) == Color::Black);

    //checking if the pixel on the left of the pixel at coordinates givenIndex is valid to add in the border of pixels that can be turned to ashes
    //for that: the index has to be valid; the pixel at this index has to be red (burning); 
    //the pixel has to not have been added to the potentialAshes border (the pixel on the left of someone can be the pixel under another one), for that we check if the second boolean (the one corresponding to being added to the ashes border or not)
    //lastly, we have to make sure that the pixel is not on the other side of the image (the left pixel of one that is all the way to the left of the image cannot be valid as it is the other side of the "map")
    if (givenIndex - 1 >= 0 && modifiedImage.getPixel(givenIndex + 1 - 1) == Color::Red && vectOfPairs[givenIndex - 1].second.second == false && (givenIndex-1)%modifiedImage.width() < givenIndex%modifiedImage.width())
    {
        // add this index to the potential burning pixels
        potentialAshes.push_back(givenIndex - 1);
        // making the bool true to indicate that this pixel has already been added to the potential ashes pixels, which will make him never be able to be added again
        vectOfPairs[givenIndex - 1].second.second = true;
    }
    //same idea but this time for the pixel on the right
    if (givenIndex + 1 < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + 1) == Color::Red && vectOfPairs[givenIndex + 1].second.second == false && (givenIndex+1)%modifiedImage.width() > givenIndex%modifiedImage.width())
    {
        potentialAshes.push_back(givenIndex + 1);
        vectOfPairs[givenIndex + 1].second.second = true;
    }
    //same idea but this time for the pixel on the top
    if (givenIndex - modifiedImage.width() >= 0 && modifiedImage.getPixel(givenIndex + 1 - modifiedImage.width()) == Color::Red && vectOfPairs[givenIndex - modifiedImage.width()].second.second == false)
    {
        potentialAshes.push_back(givenIndex - modifiedImage.width());
        vectOfPairs[givenIndex - modifiedImage.width()].second.second = true;
    }
    //same idea but this time for the pixel on the bottom
    if (givenIndex + modifiedImage.width() < modifiedImage.size() && modifiedImage.getPixel(givenIndex + 1 + modifiedImage.width()) == Color::Red && vectOfPairs[givenIndex + modifiedImage.width()].second.second == false)
    {
        potentialAshes.push_back(givenIndex + modifiedImage.width());
        vectOfPairs[givenIndex + modifiedImage.width()].second.second = true;
    }
}


//function that goes to the next step
//O( (rndNbOfPixelsBurned + rndNbOfAshes) * n ) 
//rndNbOfPixelsBurned being the number randomly generated of pixels that we will burn if we burn something at this step
//rndNbOfAshes being the number randomly generated of pixels that we will turn to ashes if we turn to ashes something at this step
//n being the number of pixels in the image
// the *n comes from the erase function that takes a worst time complexity of O(n) as it has to iterate through the vector and find the element we want to erase
void FireSimulator::nextStep() throw(int)
{
    //if we are not starting the simulation and that there are NO potential burning AND NO potential pixels that can be turned to ashes
    //that means that the simulation has to end
    //we throw an exception containing the step we stopped at, that will be caught by another function and that will display a message showing when did the simulation stop
    if ( stepCounter>0 && (int)potentialBurningPixels.size() == 0 && (int)potentialAshes.size() == 0)
    {
        throw(stepCounter);
        return;
    }

    // no assertion needed for the starting point as we previously checked it in the constructor
    if (stepCounter == 0)
    {
        //setting the pixel at index startingPoint to red, startingPoint being >=1
        modifiedImage.setPixel(startingPoint, Color::Red);
        //calling this function that will add the potential burning pixels to the border of potential burns
        addToPotentialBurningPixels(startingPoint - 1); // starting point can be 1 but the input to this function can be 0
        //incrementing the stepCounter
        stepCounter++;
    }
    else
    {
        //if we are here, that means we are after the first step
        //initializing two vectors that we will store the indexes of pixels that we burnt and/or turned to ashes, to later on call the addToPotentialBurningPixels
        //and addToPotentialAshes function. We cannot call them just after we burnt them or turned them to ashes as it would add them to the potential borders
        //and then be potentially picked in this step as we are iterating a certain amount of time (a random amount of times)
        //eg. pixel at index 1 burn pixel at index 2, if we call the function addToPotentialBurningPixels(2), it will add the pixel 3 and the one under if they are valid
        //but if it turns out, in this step we will burn 2 pixels of the border we had, it could pick the pixel 3 and burn it, which is absurd as one step cannot spread fire/ashes further than one pixel 
        vector<int> indexOfBurning;
        vector<int> indexOfBurnt;
        //checking if there are pixels that we can burn
        //it can happen that no, for example when all of the forest has been burned down and all is left to do is for the pixels to turn to ashes.
        if ((int)potentialBurningPixels.size() > 0)
        {
            //taking a random number of pixels we will burn
            int rndNbOfPixelsBurned = rand() % (int)potentialBurningPixels.size() + 1;
            for (int i = 0; i < rndNbOfPixelsBurned; i++)
            {
                // getting a random index of the vector potentialBurningPixels, which is storing the indexes (starting from 0) of pixels that can be burned
                int rndIndex = rand() % (int)potentialBurningPixels.size(); 
                //burning the pixel at the index potentialBurningPixels[rndIndex] (which is an index randomly took in the vector)
                modifiedImage.setPixel(potentialBurningPixels[rndIndex] + 1, Color::Red);

                // adding the index of this newly burnt pixel to a tmp vector, which we will iterate through at the end of the loop to update the border (as said on l143-147)
                indexOfBurning.push_back(potentialBurningPixels[rndIndex]);
                //erasing from the potential burning pixels the index of the pixel we just burnt
                potentialBurningPixels.erase(potentialBurningPixels.begin() + rndIndex);
            }
        }

        //when ashes start to appear (took 3 by looking at the simulation example we had)
        if (stepCounter > 3)
        {
            //if it is the first time we turn to ashes pixels, it HAS to start from
            //very similar approach to the one used to burn pixels.
            //We used the same approach as we wanted our simulation to be able to represent the fact that ashes can spread faster than the fire
            //if lets say in one call of this function, ndNbOfPixelsBurned < rndNbOfAshes, that means we will turn to ashes more pixels than what we are burning
            //and if this goes on, the ashes will take on the fire and stop it by itself
            //if we wanted to make ashes appear by an anteriority factor, we would have used a queue of vector of the indexes of pixel we burnt, and step
            //by step iterating through it and turning to ashes the pixels we burnt, which is actually a simpler way of simulating it, but not how we imagine a wildfire.
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

                    // adding the index to a tmp vector, which we will iterate through at the end of the loop to update the border
                    indexOfBurnt.push_back(potentialAshes[rndIndex]);
                    //erasing from the potential ashes the index of the pixel we just turned to ashes
                    potentialAshes.erase(potentialAshes.begin() + rndIndex);
                }
            }
        }

        // updating the borders by iterating through the vector we initialized at line 148 and 149
        for (int i = 0; i < (int)indexOfBurning.size(); i++)
        {
            addToPotentialBurningPixels(indexOfBurning[i]);
        }
        for (int i = 0; i < (int)indexOfBurnt.size(); i++)
        {
            addToPotentialAshes(indexOfBurnt[i]);
        }
        //incrementing the stepCounter
        stepCounter++;
    }
}

//O( (rndNbOfPixelsBurned + rndNbOfAshes) * n * nbSteps)
//rndNbOfPixelsBurned being the number randomly generated of pixels that we will burn if we burn something at a certain step
//rndNbOfAshes being the number randomly generated of pixels that we will turn to ashes if we turn to ashes something at a certain step
//n being the number of pixels in the image
//nbSteps being the number of steps the user want to advance by
//Calling advanceByNSteps(50) if we had called advanceByNSteps(30) before will go to the 50th step and will not call 50 times the nextStep function
void FireSimulator::advanceByNSteps(int nbSteps)
{
    //making sure the nbsteps given by the user is bigger than the step we are at as we cannot go back in time
    assert (nbSteps >= stepCounter);
    while (stepCounter < nbSteps)
    {
        //trying to call the next step function
        try
        {
            nextStep();
        }
        //if we catch an exception, it means the simulation has to stop, so we display that it is the end and when did the simulation stop
        //exiting the function after this is done as we no longer have to continue
        catch (int e)
        {
            cout << "END OF THE SIMULATION. YOU HAVE BURNED DOWN A FOREST." <<endl;
            cout << "IT TOOK " << e << " STEPS." << endl;
            exit(1);
        }
    }
    //
    //modifiedImage.writeSVG("simulationFile", 10);
}

//O( (rndNbOfPixelsBurned + rndNbOfAshes) * n * nbSteps)
//function that returns the image at step nbSteps or the maximum step the simulation for this image can go on 
Image FireSimulator::getImageOfStepN(int nbSteps)
{
    advanceByNSteps(nbSteps);
    return modifiedImage;
}

//explain the normal distribution here
