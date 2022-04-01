////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "Analyst.h"

Analyst::Analyst(const Image &img) : analyzedImage(img)
{
    pair<Color, bool> tmpPair;
    for (int i = 0; i < analyzedImage.size(); i++)
    {
        tmpPair.first = analyzedImage.getPixel(i);
        tmpPair.second = false;
        matrixOfColorAndBool.push_back(tmpPair);
    }

    //O(w*h * z), z being the number of zones in the image. 
    //the worst case being when the image is a chess board, it's equivalent to O(w^2 * h^2) 
    floodFill();
}

Image Analyst::getImage() const
{
    return analyzedImage;
}

Image Analyst::fillZone(int i, int j, Color c)
{
    // creating a copy of the image
    Image tmpImage(analyzedImage);

    Color colorOfTheStartingPixel = tmpImage.getPixel(i, j);
    fillZoneDFS(tmpImage, i, j, colorOfTheStartingPixel, c);

    return tmpImage;
}

// O(w*h) function as in the worst case, the entire matrix contains the same color as the pixel at position (i, j), which will mean we travel through the entire matrix
// function that will get recursively called to execute what we call a flood fill
void Analyst::fillZoneDFS(Image &givenImage, int i, int j, Color previousColor, Color c)
{

    // checking if i and j are valid inputs for the image in a first place, that way we can't get any out of bounds errors while checking the conditions after

    // then checking if the color given is already the color of our starting pixel, which means we shouldn't do anything

    // then checking if the Color of the pixel we're looking at is different than the previousColor, which means we found a neighbor
    // that isn't in the same zone as the pixel at position (i,j)
    if (i < 0 || i >= givenImage.height() || j < 0 || j >= givenImage.width() || givenImage.getPixel(i, j) == c || givenImage.getPixel(i, j) != previousColor)
    {
        // if the condition is true, it means we either can't go on or don't want to go on, so we get out of the program
        return;
    }

    // setting the pixel we are looking at to the color given in argument
    givenImage.setPixel(i, j, c);
    // givenImage.DisplayImageInTerminal();

    // recursive call to the fillZoneDFS function
    // this time it'll apply the function to the pixel one row before (the row on top) the starting pixel (if it exists)
    fillZoneDFS(givenImage, i - 1, j, previousColor, c);
    // same idea but for the pixel one row after the starting pixel
    fillZoneDFS(givenImage, i + 1, j, previousColor, c);
    // same idea but for the pixel one column before (column on the left) the starting pixel
    fillZoneDFS(givenImage, i, j - 1, previousColor, c);
    // same idea but for the pixel one column after the starting pixel
    fillZoneDFS(givenImage, i, j + 1, previousColor, c);
}

// function that will take in parameters a set of integers (indexes of the coordinates transormed by the toIndex function)
// this algorithm will realize a DFS, by callingitself four times, once for every pixel around the pixel we're looking at (pixel on top, under, on the left and on the right)

// The idea is to use the matrix of Colors and Bools to check whether we already visited a pixel or not, in the case that we did visit the pixel
// we're looking at, we get out of the function as there's nothing to be done, just like if the pixel we're looking at doesn't have the color c
//=> which means if we get out of the function, the pixel is either not from the same zone, or it is from the same zone but we already looked at it before, so there's nothing else to do.
void Analyst::floodFillRec(set<int> &givenSet, int i, int j, Color c)
{
    int widthOfMatrix = analyzedImage.width();
    int heightOfMatrix = analyzedImage.height();
    // if (i < 0 || i >= heightOfMatrix || j < 0 || j >= widthOfMatrix || matrixOfColorAndBool.at(i * widthOfMatrix + j).second == 1 || matrixOfColorAndBool.at(i * widthOfMatrix + j).first != c)
    // {
    //     // if the condition is true, it means we either can't go on or don't want to go on, so we get out of the program
    //     return;
    // }

    // recursive calls of the function that will realize what we mean by flood fill, it'll look at the pixels around the pixel we're starting from,
    // and then look at the pixel around these pixels etc etc... until we went through the entire image.
    if ((i-1>=0) && (i-1 < heightOfMatrix) && (j >=0 ) && ( j < widthOfMatrix) && (matrixOfColorAndBool.at((i-1)* widthOfMatrix + j).second==0) && (matrixOfColorAndBool.at((i-1)* widthOfMatrix + j).first== c)){
                // setting the bool of the pixel we're looking at to visited
        matrixOfColorAndBool.at(i * widthOfMatrix + j).second = 1;
        // adding the index of the pixel we're looking at into the given set
        givenSet.insert(analyzedImage.toIndex(i, j));
        floodFillRec(givenSet, i - 1, j, c);
    }
    
    if ((i+1>=0) && (i+1 < heightOfMatrix) && (j >=0 ) && ( j < widthOfMatrix) && (matrixOfColorAndBool.at((i+1)* widthOfMatrix + j).second==0) && (matrixOfColorAndBool.at((i+1)* widthOfMatrix + j).first==c)){
        // setting the bool of the pixel we're looking at to visited
        matrixOfColorAndBool.at(i * widthOfMatrix + j).second = 1;
        // adding the index of the pixel we're looking at into the given set
        givenSet.insert(analyzedImage.toIndex(i, j));
        floodFillRec(givenSet, i + 1, j, c);
    }

    if ((i>=0) && (i < heightOfMatrix) && (j-1 >=0 ) && ( j-1 < widthOfMatrix) && (matrixOfColorAndBool.at((i)* widthOfMatrix + j-1).second==0) && (matrixOfColorAndBool.at((i)* widthOfMatrix + j-1).first== c)){
        matrixOfColorAndBool.at(i * widthOfMatrix + j).second = 1;
        // adding the index of the pixel we're looking at into the given set
        givenSet.insert(analyzedImage.toIndex(i, j));
        floodFillRec(givenSet, i, j - 1, c);
    }

    if ((i>=0) && (i < heightOfMatrix) && (j+1 >=0 ) && ( j+1 < widthOfMatrix) && (matrixOfColorAndBool.at((i)* widthOfMatrix + j+1).second==0) && (matrixOfColorAndBool.at((i)* widthOfMatrix + j+1).first== c)){
        // setting the bool of the pixel we're looking at to visited
        matrixOfColorAndBool.at(i * widthOfMatrix + j).second = 1;
        // adding the index of the pixel we're looking at into the given set
        givenSet.insert(analyzedImage.toIndex(i, j));
        floodFillRec(givenSet, i, j + 1, c);
    }


    return;
    // this function will ultimately start from a pixel, and insert into the set the index of every pixel in the same zone as the starting pixel.
    // the goal is to run this function for every pixel of the image.

    // this function has a O(w*h) complexity as it goes through the entire matrix of pixel, which is w*h long.
}

// This function is called directly in the constructor, that way directly after the initialization of an Analyst object, we get a vector of sets containing the indexes
// of zones, for example all of the indexes in the first object of the vector, will be the indexes of pixels from the same zone
// That way, to get the number of zones, or in general, to play around will the indexes, I can directly use the size of my vector, do a search in my vector, etc etc...

// function using the recursive call floodFillRec for every pixel of the matrix of pixels
// as said, the floodFillRec will insert into the set given in argument in the first call of the function, the indexes of the pixels from the same zone as the pixel we're starting from
// the goal of this function is to call the floodFillRec function for every pixel of the matrix.
void Analyst::floodFill()
{
    // initizaling a set that will then be passed as parameter of the floodFillRec function
    set<int> tmpSet;
    for (int i = 0; i < (int)analyzedImage.height(); i++)
    {
        for (int j = 0; j < (int)analyzedImage.width(); j++)
        {
            // initializing the set to an empty set for every iteration of the function
            //tmpSet = {};
            // calling the floodFillRec function on the pixel at position (i, j)
            floodFillRec(tmpSet, i, j, analyzedImage.getPixel(i, j));
            // at this point, tmpSet has been modified by the floodFillRec if it needed to be modified (aka if there was other pixels in the same zone as the pixel at position (i, j))
            // if the size of the set is not > 0, it means nothing has been added, which means we don't need to add it to our sets of indexes.
            if (tmpSet.size() > 1)
            {
                // adding the set of indexes of pixels to our vector of sets, that will later get used in other functions.
                vectOfSets.push_back(tmpSet);
            }
            tmpSet.clear();
        }
    }

    // This function run w*h times, using a function that has a complexity of O(w*h), so
    // the complexity of this function is O(w^2*h^2) in the worst case.
    // The worst case being whenever the image is a chess board for example, where there are w*h zones.
    // In reality, it isn't something in O(w^2*h^2) as the floodFillRec will run k times, k being the number of zones there are in the Image.
    // It is maybe more logical to express the time complexity of this function as O(w*h*k).
}

bool Analyst::belongToTheSameZone(int i1, int j1, int i2, int j2)
{
    int uniqueIndexOfFirstPixel = analyzedImage.toIndex(i1, j1);
    int uniqueIndexOfSecondPixel = analyzedImage.toIndex(i2, j2);
    // iterating through each set of the vector
    // O(k) operation, k being the number of zones, which is w*h zones in the worst case
    for (set<int> setOfIndexes : vectOfSets)
    {
        // returns true if there is the uniqueindexOfFirstPixel in the set we're looking at
        // O(log n) operation
        if (setOfIndexes.count(uniqueIndexOfFirstPixel))
        {
            // If we are at this point, we know the pixel at (i1, j1) is in the zone represented by the set we're currently looking at
            // which means that if the pixel (i1,j1) and (i2,j2) belong to the same zone, the unique index of the pixel at (i2,j2) is in the set we're looking at

            // returning the value obtaining when searching if the uniqueIndexOfSecondPixel is in this set
            return setOfIndexes.count(uniqueIndexOfSecondPixel);
        }
    }
    return false;
}
// O(log2(z)) ?? or do we count the one in the constructor

int Analyst::nbPixelsOfColor(Color c) const
{
    int totalNb = 0;
    for (int i = 0; i < analyzedImage.size(); i++)
    {
        if (analyzedImage.getPixel(i) == c)
        {
            totalNb++;
        }
    }
    return totalNb;

    // maybe check something in hashmap storing vectors of sets, and check size, do this with testeval
}

int Analyst::nbZonesOfColor(Color c) const
{
    int totalNbOfZones = 0;
    for (set<int> setOfPixel : vectOfSets)
    {
        int indexOfFirstPixelInZone = *(setOfPixel.begin());
        pair<int, int> coordinatesOfFirstPixelInZone = analyzedImage.toCoordinate(indexOfFirstPixelInZone);

        if (analyzedImage.getPixel(coordinatesOfFirstPixelInZone.first, coordinatesOfFirstPixelInZone.second) == c)
        {
            totalNbOfZones++;
        }
    }
    return totalNbOfZones;
}


int Analyst::nbZones() const
{
    // simply returning the size of the vect containing the sets, since every set is one zone
    return vectOfSets.size();
    //O(1) operation
}

set<int> Analyst::zoneOfPixel(int i, int j)
{
    
    int widthOfMatrix = analyzedImage.width();
    int heightOfMatrix = analyzedImage.height();
    assert(i > 0 && i <= heightOfMatrix && j > 0 && j <= widthOfMatrix);
    // getting the unique index of the pixel at position (i, j)
    int uniqueIndexOfPixel = analyzedImage.toIndex(i, j);
    for (set<int> setOfPixel : vectOfSets)
    {
        
        // O(log z) operation
        // if the unique index is in the set, we return the set
        if (setOfPixel.count(uniqueIndexOfPixel))
        {
            return setOfPixel;
        }
    }
    return set<int>();
}
