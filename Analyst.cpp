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
