#include <JuceHeader.h>
#include "MergingWaveformDisplay.h"

MergingWaveformDisplay::MergingWaveformDisplay()
{

}

MergingWaveformDisplay::~MergingWaveformDisplay()
{
}

void MergingWaveformDisplay::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (14.0f));
    g.drawText ("MergingWaveformDisplay", getLocalBounds(),
                juce::Justification::centred, true);  
}

void MergingWaveformDisplay::resized()
{

}
