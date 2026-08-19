#include <JuceHeader.h>
#include "WaveformDisplay.h"

WaveformDisplay::WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : audioThumb(1000, formatManagerToUse, cacheToUse),
    fileLoaded(false),
    position(0.0)
{
    audioThumb.addChangeListener(this);

}

void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster* source) {
    std::cout << "wfd: change received!" << std::endl;
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
    if (fileLoaded)
    {
        position = juce::jlimit(0.0, 1.0, pos);
        repaint(); 
    }
}


WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentBlack);
    g.setColour(waveformColor); 
    g.setColour(juce::Colours::white);

    if (fileLoaded)
    {
        auto waveformWidth = getWidth();
        auto waveformHeight = getHeight();
        auto totalLength = audioThumb.getTotalLength();

        juce::Rectangle<int> waveformBounds(0, 0, waveformWidth, waveformHeight);

        g.setColour(juce::Colours::green);
        audioThumb.drawChannel(g, waveformBounds, 0.0, totalLength, 0, 1.0f);

        g.setColour(juce::Colours::red);
        double positionX = position * waveformWidth;  
        g.drawLine(positionX, 0, positionX, waveformHeight, 2.0f);
    }
    else
    {
        g.setFont(20.0f);
        g.drawText("File not loaded", getLocalBounds(), juce::Justification::centred, true);
    }
}



void WaveformDisplay::resized()
{

}

void WaveformDisplay::loadURL(juce::URL audioURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audioURL));

    if (fileLoaded) {
        std::cout << "wfd: loaded!" << std::endl;
        repaint();
    }
    else {
        std::cout << "wfd: not loaded!" << std::endl;
    }
}


void WaveformDisplay::setWaveformColor(juce::Colour color)
{
    waveformColor = color;
    repaint();
}
