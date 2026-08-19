#pragma once
#include <JuceHeader.h>

class WaveformDisplay : public juce::Component, public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~WaveformDisplay() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

    void loadURL(juce::URL audioURL);
    void setPositionRelative(double pos);
    void setWaveformColor(juce::Colour color);

    juce::AudioThumbnail& getAudioThumbnail() { return audioThumb; } 

private:
    juce::AudioThumbnail audioThumb; 
    bool fileLoaded;
    double position;
    juce::Rectangle<int> visibleArea; 
    juce::Colour waveformColor = juce::Colours::white;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};
