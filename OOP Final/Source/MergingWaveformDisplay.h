#pragma once
#include <JuceHeader.h>

class MergingWaveformDisplay  : public juce::Component
{
public:
    MergingWaveformDisplay();
    ~MergingWaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MergingWaveformDisplay)
};
