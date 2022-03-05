////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "Analyst.h"

Analyst::Analyst(const Image &img)
{
    // assert (img.size() == size());
    //if can't assert this, do an if condition and push_back what's needed

    analyzedImage.setWidth(img.width());
    analyzedImage.setHeight(img.height());

    for (int i = 0; i < analyzedImage.height() * analyzedImage.width(); i++)
    {
        analyzedImage.addPixel(img.getPixel(i));
    }
}

Image Analyst::getImage() const {
    return analyzedImage;
}

Image Analyst::fillZone(int i, int j, Color c){
    //creating a copy of the image
    Image tmpImage(analyzedImage);

    Color colorOfTheStartingPixel = tmpImage.getPixel(i,j);
    fillZoneDFS(tmpImage, i, j, colorOfTheStartingPixel, c);
    return tmpImage;
}

//O(w*h) function as in the worst case, the entire matrix contains the same color as the pixel at position (i, j), which will mean we travel through the entire matrix
//function that will get recursively called to execute what we call a flood fill
void Analyst::fillZoneDFS(Image &givenImage, int i, int j,Color previousColor, Color c){

    //checking if i and j are valid inputs for the image in a first place, that way we can't get any out of bounds errors while checking the conditions after

    //then checking if the color given is already the color of our starting pixel, which means we shouldn't do anything

    //then checking if the Color of the pixel we're looking at is different than the previousColor, which means we found a neighbor
    //that isn't in the same zone as the pixel at position (i,j)
    if (i<0 || i >= givenImage.height() || j<0 || j>=givenImage.width() || givenImage.getPixel(i,j) == c || givenImage.getPixel(i,j) != previousColor){
        //if the condition is true, it means we either can't go on or don't want to go on, so we get out of the program
        return;
    }

    //setting the pixel we are looking at to the color given in argument
    givenImage.setPixel(i,j, c);
    //givenImage.DisplayImageInTerminal();

    //recursive call to the fillZoneDFS function
    //this time it'll apply the function to the pixel one row before (the row on top) the starting pixel (if it exists)
    fillZoneDFS(givenImage, i-1, j, previousColor, c);
    //same idea but for the pixel one row after the starting pixel
    fillZoneDFS(givenImage, i+1, j, previousColor, c);
    //same idea but for the pixel one column before (column on the left) the starting pixel
    fillZoneDFS(givenImage, i, j-1, previousColor, c);
    //same idea but for the pixel one column after the starting pixel
    fillZoneDFS(givenImage, i, j+1, previousColor, c);
}