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


FireSimulator::FireSimulator(const Image& givenImage) : fireAnalyst(givenImage) {

    //make initialImage = givenImage
    initialImage.setWidth(givenAnalyst.getImage().width());
    initialImage.setHeight(givenAnalyst.getImage().height());

    for (int i = 0; i < initialImage.height() * initialImage.width(); i++)
    {
        initialImage.addPixel(givenAnalyst.getImage().getPixel(i));
    }

    fireAnalyst.
    fireAnalyst.getImage().DisplayImageInTerminal();



}
