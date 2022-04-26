////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#ifndef ANALYST_H
#define ANALYST_H
#include "Image.h"
#include <set>
#include <list>

#include <unordered_map>

struct Node {
  int indexOfPixel;
  int representant;
};

struct Head {
  list<Node>* listOfNodes;
  int* size;
};

////////////////////////////////////////////////////////////////////////////////
/// This is an analyst of images.
///
/// An analyst is able to see that an image is composed of different zones.
/// A zone is a connected set of pixels having the same color.
////////////////////////////////////////////////////////////////////////////////
class Analyst
{
public:
  /// Creates an analyst of a given image
  Analyst(const Image& img);

  /// No copy
  Analyst(const Analyst &) = delete;

  /// No assignment
  Analyst &operator=(const Analyst &) = delete;

  Image getImage() const;

  /// Destructor
  //~Analyst();

  /// Tests if the pixels (i1, j1) and (i2, j2) of the input image
  /// are in the same zone
  bool belongToTheSameZone(int i1, int j1, int i2, int j2);

  /// Returns the number of pixels of a given color in the input image
  int nbPixelsOfColor(Color c) const;

  /// Returns the number of zones of a given color in the input image
  int nbZonesOfColor(Color c) const;

  /// Returns the number of zones in the input image
  int nbZones() const;

  /// Creates a new image by filling the zone of pixel (i, j) in the input
  /// image with a given color
  Image fillZone(int i, int j, Color c);
  //function that will call the fillZoneDFS


  //function that will do the fill zone by doing a DFS across the image
  //created this function as the signature of the simple fillZone wouldn't let me use the argument previousColor nor tmpImage
  //which is essential to make the DFS work
  //It doesn't change the completion of the algorithm as I'll call the fillZoneDFS in the fillZone function though.
  //making the function static to be able to use it in the fillZone function
  void fillZoneDFS(Image &tmpImage, int i, int j,Color previousColor, Color c);

  /// Returns the indexes of the pixels that belong to the zone of (i, j)
  //ask if pixel at ith row and jth column
  std::set<int> zoneOfPixel(int i, int j);

  void DisplayLL();

  int find(int index);

  //function that merges two zones
  void merge(int index1, int index2);
  
  //function called in the constructor of the analyst, calling the merge function on 
  void mergeAll();

private:
  //creating a HashMap using Colors as keys and sets of integers as values
  Image analyzedImage;
  vector<Head> vectOfPointers; //vector of linked lists
  vector<int> vectOfNbOfPixelsPerColor;
  vector<int> vectOfNbOfZonesPerColor;
  int zoneCount;
};

#endif
 