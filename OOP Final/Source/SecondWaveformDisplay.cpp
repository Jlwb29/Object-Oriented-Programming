#include "SecondWaveformDisplay.h"

SecondWaveformDisplay::SecondWaveformDisplay()
    : audioThumbnail(512, formatManager, juce::AudioThumbnailCache(1000))
{
    formatManager.registerBasicFormats();
}

SecondWaveformDisplay::~SecondWaveformDisplay() = default;

void SecondWaveformDisplay::loadAudioFile(const juce::URL& audioURL)
{
    juce::File file(audioURL.getLocalFile());
    audioThumbnail.setSource(new juce::FileInputSource(file));
}

void SecondWaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.drawImageAt(waveformImage, 0, 0);
}

void SecondWaveformDisplay::resized()
{

}
