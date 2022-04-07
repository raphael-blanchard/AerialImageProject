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
    zoneCount = analyzedImage.height()* analyzedImage.width();
    for (int i = 0; i < analyzedImage.height()* analyzedImage.width(); i++){
        vectOfPointers.push_back(Head());
        vectOfPointers[i].size = new int(1);
        vectOfPointers[i].listOfNodes = new list<Node>();
        vectOfPointers[i].listOfNodes->push_back(Node({i, i}));
    }
    mergeAll();
    cout << "number of zones: " <<zoneCount << endl;
}

void Analyst::DisplayLL(){

    // list<Node> tmpList;
    // for (int i = 0; i < 3; i++){
    //     tmpList.push_back(Node({i*10, i*10}));
    // }

    for (int i = 0; i < (int)vectOfPointers.size(); i++){
        // auto it = vectOfPointers[i].listOfNodes.begin();
        // vectOfPointers[i].listOfNodes.splice(it, tmpList);
        cout << "size of this list is: " << *(vectOfPointers[i].size) << " - ";
        for (auto it=vectOfPointers[i].listOfNodes->begin(); it!=vectOfPointers[i].listOfNodes->end(); ++it){
            cout << "{index: " << it->indexOfPixel << ",rep: " << find(i) << "}";
        }
        cout << endl;
    }
}

int Analyst::find(int index){
    if (vectOfPointers[index].listOfNodes->front().representant == index){
        return index;
    }
    else {
        return find(vectOfPointers[index].listOfNodes->front().representant);
    }
}

void Analyst::merge(int index1, int index2){

    assert (index1<analyzedImage.width()*analyzedImage.height() && index1>=0 && index2<analyzedImage.width()*analyzedImage.height() && index2>=0);
    int parentOfIndex1 = find(index1);
    int parentOfIndex2 = find(index2);
    if (parentOfIndex1 == parentOfIndex2){
        return;
        //in the same LL so we don't need to concatenate them
    }

    //putting the LL of index1 after the LL at index2
    if (*(vectOfPointers[index1].size) <= *(vectOfPointers[index2].size)){
        vector<int> tmpVectOfIndexes;
        for (auto it=vectOfPointers[index1].listOfNodes->begin(); it!=vectOfPointers[index1].listOfNodes->end(); ++it){
            tmpVectOfIndexes.push_back(it->indexOfPixel);
        }
        auto it = vectOfPointers[index2].listOfNodes->end();
        vectOfPointers[index2].listOfNodes->splice(it, *(vectOfPointers[index1].listOfNodes));
        *(vectOfPointers[index2].size) += *(vectOfPointers[index1].size);
        
        for (int index : tmpVectOfIndexes){
            vectOfPointers[index].listOfNodes->front().representant = parentOfIndex2;
            vectOfPointers[index].listOfNodes = vectOfPointers[index2].listOfNodes;
            vectOfPointers[index].size = vectOfPointers[index2].size;
        }
    }
    else{
        vector<int> tmpVectOfIndexes;
        for (auto it=vectOfPointers[index2].listOfNodes->begin(); it!=vectOfPointers[index2].listOfNodes->end(); ++it){
            tmpVectOfIndexes.push_back(it->indexOfPixel);
        }
        auto it = vectOfPointers[index1].listOfNodes->end();
        vectOfPointers[index1].listOfNodes->splice(it, *(vectOfPointers[index2].listOfNodes));
        *(vectOfPointers[index1].size) += *(vectOfPointers[index2].size);
        vectOfPointers[index2].listOfNodes->front().representant = parentOfIndex1;
        vectOfPointers[index2].listOfNodes = vectOfPointers[index1].listOfNodes;
        vectOfPointers[index2].size = vectOfPointers[index1].size;
        for (int index : tmpVectOfIndexes){
            vectOfPointers[index].listOfNodes->front().representant = parentOfIndex1;
            vectOfPointers[index].listOfNodes = vectOfPointers[index1].listOfNodes;
            vectOfPointers[index].size = vectOfPointers[index1].size;
        }
    }
}

void Analyst::mergeAll(){
    int size = analyzedImage.width()*analyzedImage.height();
    for (int i = 1; i <= size; i++){
        if (i+1 <= size && ((i)%analyzedImage.width() > (i-1)%analyzedImage.width()) && analyzedImage.getPixel(i+1) == analyzedImage.getPixel(i) && find(i-1) != find(i)){
            merge(i-1, i);
            cout << "first if: " << i << " and " << i+1 <<endl;
            zoneCount--;
        }
        if (i+analyzedImage.width()<=size && analyzedImage.getPixel(i+analyzedImage.width()) == analyzedImage.getPixel(i) && find(i+analyzedImage.width()) != find(i)){
            merge(i-1, i+analyzedImage.width()-1);
            cout << "second if: " << i << " and " << i+analyzedImage.width() <<endl;
            zoneCount--;
        }
    }
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
