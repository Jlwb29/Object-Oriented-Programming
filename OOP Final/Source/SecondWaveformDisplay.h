#pragma once

#include <JuceHeader.h>

class SecondWaveformDisplay : public juce::Component
{
public:
    SecondWaveformDisplay();
    ~SecondWaveformDisplay() override;

    void loadAudioFile(const juce::URL& audioURL);
    void paint(juce::Graphics& g) override;
    void resized() override;

private:
    juce::Image waveformImage;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnail audioThumbnail;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SecondWaveformDisplay)
};
