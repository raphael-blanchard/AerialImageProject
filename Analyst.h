////////////////////////////////////////////////////////////////////////////////
/// This file is part of the Aerial Image Project (AIP).
///
/// Copyright (c) ...
///
/// The sources of AIP are distributed WITHOUT ANY WARRANTY.
////////////////////////////////////////////////////////////////////////////////

#ifndef ANALYST_H
#define ANALYST_H

#include <set>
#include "Image.h"

////////////////////////////////////////////////////////////////////////////////
/// This is an analyst of images.
///
/// An analyst is able to see that an image is composed of different zones.
/// A zone is a connected set of pixels having the same color.
////////////////////////////////////////////////////////////////////////////////
class Analyst {
public:
  /// Creates an analyst of a given image
  //Analyst(const Image& img);

  /// No copy
  Analyst(const Analyst&) = delete;

  /// No assignment
  Analyst& operator=(const Analyst&) = delete;

  /// Destructor
  //~Analyst();

  /// Tests if the pixels (i1, j1) and (i2, j2) of the input image
  /// are in the same zone
  //bool belongToTheSameZone(int i1, int j1, int i2, int j2);

  /// Returns the number of pixels of a given color in the input image
  //int nbPixelsOfColor(Color c) const;

  /// Returns the number of zones of a given color in the input image
  //int nbZonesOfColor(Color c) const;

  /// Returns the number of zones in the input image
  //int nbZones() const;

  /// Creates a new image by filling the zone of pixel (i, j) in the input
  /// image with a given color
  //Image fillZone(int i, int j, Color c);

  /// Returns the indexes of the pixels that belong to the zone of (i, j)
  //std::set<int> zoneOfPixel(int i, int j);

private:



};

#endif
