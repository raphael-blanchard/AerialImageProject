////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "Analyst.h"

//O(n) - n being the number of pixels in the image
Analyst::Analyst(const Image &img) : analyzedImage(img)
{
    zoneCount = analyzedImage.size();
    for (int i = 0; i < analyzedImage.size(); i++)
    {
        vectOfPointers.push_back(Head());
        vectOfPointers[i].size = new int(1);
        vectOfPointers[i].listOfNodes = new list<Node>();
        vectOfPointers[i].listOfNodes->push_back(Node({i, i}));
    }
    for (int i = 0; i < Color::nbColors(); i++)
    {
        vectOfNbOfZonesPerColor.push_back(0);
        vectOfNbOfPixelsPerColor.push_back(0);
    }

    for (int i = 1; i <= analyzedImage.size(); i++){
        //integer value of the pixel at coordinate i
        int indexOfColor = analyzedImage.getPixel(i).toInt();
        //as vectOfNbOfZonesPerColor.at(i) counts the numbers of zones of the color i (Color::makeColor(i))
        //this loop will set the number of the zone of color i equal to the number of pixels of color i in the image
        //(which we will decrease later on when merging zones)
        vectOfNbOfZonesPerColor.at(indexOfColor)++;

        //as vectOfNbOfPixelsPerColor.at(i) counts the number of pixels in the zone of color i
        //we can increment the value at the index indexOfColor
        vectOfNbOfPixelsPerColor.at(indexOfColor)++;
    }

    mergeAll();
}


//function for us to see how the linked list reacts, don't put in report
void Analyst::DisplayLL()
{

    // list<Node> tmpList;
    // for (int i = 0; i < 3; i++){
    //     tmpList.push_back(Node({i*10, i*10}));
    // }

    for (int i = 0; i < (int)vectOfPointers.size(); i++)
    {
        cout << "size of this list is: " << *(vectOfPointers[i].size) << " - ";
        for (auto it = vectOfPointers[i].listOfNodes->begin(); it != vectOfPointers[i].listOfNodes->end(); ++it)
        {
            cout << "{index: " << it->indexOfPixel << ",rep: " << find(i) << "}";
        }
        cout << endl;
    }
}

//O(n) : worst case is when all of the pixels are in the same zone, which means there will be one similar linked list for everyone.
//if we try to get the representant of the last added pixel of this linked list, it will call n times this function
//Omeg(1) when the the pixel at coordinate index is the representant
//function that returns the index of the representant of the zone in which the pixel at coordinate index is
int Analyst::find(int index)
{
    if (vectOfPointers[index].listOfNodes->front().representant == index)
    {
        return index;
    }
    else
    {
        return find(vectOfPointers[index].listOfNodes->front().representant);
    }
}

//NEED TO USE SHARED POINTERS HERE AND MAKE THE DESTRUCTOR

//O(k) - k being the size of the smallest zone between the zone in which index1 is in and the zone in which index2 is in
void Analyst::merge(int index1, int index2)
{
    assert(index1 < analyzedImage.size() && index1 >= 0 && index2 < analyzedImage.size() && index2 >= 0);
    int parentOfIndex1 = find(index1);
    int parentOfIndex2 = find(index2);
    if (parentOfIndex1 == parentOfIndex2)
    {
        return;
        //in the same LL so we don't need to concatenate them
    }

    //putting the LL of index1 after the LL at index2 as it is the smallest of the two, which makes us do less steps than doing the opposite
    if (*(vectOfPointers[index1].size) <= *(vectOfPointers[index2].size))
    {
        vector<int> tmpVectOfIndexes;
        for (auto it = vectOfPointers[index1].listOfNodes->begin(); it != vectOfPointers[index1].listOfNodes->end(); ++it)
        {
            tmpVectOfIndexes.push_back(it->indexOfPixel);
        }
        auto it = vectOfPointers[index2].listOfNodes->end();
        vectOfPointers[index2].listOfNodes->splice(it, *(vectOfPointers[index1].listOfNodes));
        *(vectOfPointers[index2].size) += *(vectOfPointers[index1].size);

        for (int index : tmpVectOfIndexes)
        {
            vectOfPointers[index].listOfNodes->front().representant = parentOfIndex2;
            vectOfPointers[index].listOfNodes = vectOfPointers[index2].listOfNodes;
            vectOfPointers[index].size = vectOfPointers[index2].size;
        }
    }
    //putting the LL of index2 after the LL at index1
    else
    {
        vector<int> tmpVectOfIndexes;
        for (auto it = vectOfPointers[index2].listOfNodes->begin(); it != vectOfPointers[index2].listOfNodes->end(); ++it)
        {
            tmpVectOfIndexes.push_back(it->indexOfPixel);
        }
        auto it = vectOfPointers[index1].listOfNodes->end();
        vectOfPointers[index1].listOfNodes->splice(it, *(vectOfPointers[index2].listOfNodes));
        *(vectOfPointers[index1].size) += *(vectOfPointers[index2].size);
        vectOfPointers[index2].listOfNodes->front().representant = parentOfIndex1;
        vectOfPointers[index2].listOfNodes = vectOfPointers[index1].listOfNodes;
        vectOfPointers[index2].size = vectOfPointers[index1].size;
        for (int index : tmpVectOfIndexes)
        {
            vectOfPointers[index].listOfNodes->front().representant = parentOfIndex1;
            vectOfPointers[index].listOfNodes = vectOfPointers[index1].listOfNodes;
            vectOfPointers[index].size = vectOfPointers[index1].size;
        }
    }
}

//function that calls the merge function with the bottom and right pixel (if they exist and are valid) of all of the pixel of the image, 
//which will, in the end, put every pixel of a zone in the same zone and this, for every existing zone of the image
//O(n*k) - n being the number of pixels in the image and k being the size of the smallest zone that we're trying to merge for each pixel
//O(sum from 1 to size O(k)) = O(n*k)
void Analyst::mergeAll()
{
    for (int i = 1; i <= analyzedImage.size(); i++)
    {
        if (i + 1 <= analyzedImage.size() && ((i) % analyzedImage.width() > (i - 1) % analyzedImage.width()) && analyzedImage.getPixel(i + 1) == analyzedImage.getPixel(i) && find(i - 1) != find(i))
        {
            merge(i - 1, i);
            int valueOfColor = analyzedImage.getPixel(i).toInt();
            //as vectOfNbOfZonesPerColor.at(i) counts the numbers of zones of the color i (Color::makeColor(i))
            //decrementing the value at the index valueOfColor is logical as merging two zones of the same colors, decreases the number of
            //zones of this specific color
            vectOfNbOfZonesPerColor.at(valueOfColor)--;
            //we decrement the zoneCount as merging 2 zones means they are part of the same zone, which means their zone only has to count for 1 and not 2
            zoneCount--;
        }
        if (i + analyzedImage.width() <= analyzedImage.size() && analyzedImage.getPixel(i + analyzedImage.width()) == analyzedImage.getPixel(i) && find(i + analyzedImage.width() - 1) != find(i))
        {
            merge(i - 1, i + analyzedImage.width() - 1);
            int valueOfColor = analyzedImage.getPixel(i).toInt();
            //same idea as previous if condition
            vectOfNbOfZonesPerColor.at(valueOfColor)--;
            zoneCount--;
        }
    }
}

//O(1)
//function that returns the image.
Image Analyst::getImage() const
{
    return analyzedImage;
}

//function that fills the zone in which pixel at coordinates (i,j) is, with the color c
//O(n)
Image Analyst::fillZone(int i, int j, Color c)
{
    // creating a copy of the image
    Image tmpImage(analyzedImage);

    Color colorOfTheStartingPixel = tmpImage.getPixel(i, j);
    fillZoneDFS(tmpImage, i, j, colorOfTheStartingPixel, c);

    return tmpImage;
}

// O(n) - n being the number of pixels in the image
//in the worst case, the entire matrix contains the same color as the pixel at position (i, j), which will mean we travel through the entire matrix
//DFS inspired method called floodFill
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

//function that checks if two pixels are in the same zone
//O(n) in the worst case as find is O(n)
//Omeg(1) if pixel at coordinates (i1,j1) and pixel at coordinates (i2,j2) are both the representant of their zone and if they're not in the same zone(can't be in the same zone if each representant anyway)
bool Analyst::belongToTheSameZone(int i1, int j1, int i2, int j2)
{
    int widthOfImage = analyzedImage.width();
    //i1*widthOfImage + j1 will be the index of the pixel at coordinates (i1,j1) in a one dimension array
    //if the parent of these pixels aren't the same, then they are not in the same zone
    return find(i1 * widthOfImage + j1) == find(i2 * widthOfImage + j2);
}

//O(1) as we store a vector of the number of pixels of each color. the indexes of this vector are corresponding to the values of the color 
//eg. vectOfNbOfPixelsPerColor[0] corresponds to the number of pixels with the color Black
int Analyst::nbPixelsOfColor(Color c) const
{
    return vectOfNbOfPixelsPerColor.at(c.toInt());
}

//same idea but for the nb of zones of a certain color
//as we have initialized (in the constructor) and decremented these values each time we were merging zones, we can return this value in constant time
//O(1) function
//same eg. vectOfNbOfZonesPerColor[0] will correspond to the number of zones of color black in the image analyzed
int Analyst::nbZonesOfColor(Color c) const
{

    return vectOfNbOfZonesPerColor.at(c.toInt());
}

//O(1)
int Analyst::nbZones() const
{
    //as we set the number of zones to the number of pixels at the beginning
    //then decrement this value each time we merge zones
    //we can return this in constant time
    return zoneCount;
}


//function that returns a set of the indexes of the pixels that are in the zone in which pixel at coordinates (i,j) is (him included)
//O(n) - n being the number of pixels in the image. worst case is when there's only one zone, so we are iterating n times
//Theta(k) in general, k being the size of the zone in which pixel at coordinates (i, j) is
set<int> Analyst::zoneOfPixel(int i, int j)
{
    assert(i >= 0 && i <analyzedImage.height() && j >= 0 && j < analyzedImage.width());
    set<int> tmpSet;
    int indexOfPixel = analyzedImage.toIndex(i,j);
    for (auto it = vectOfPointers[indexOfPixel].listOfNodes->begin(); it != vectOfPointers[indexOfPixel].listOfNodes->end(); ++it){
        //constant operation
        tmpSet.insert(it->indexOfPixel);
    }
    return tmpSet;
}
