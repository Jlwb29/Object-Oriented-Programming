#pragma once
#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "DeckGUI3.h" 
class DeckGUI3; 

class DeckGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::FileDragAndDropTarget,
    public juce::Timer
{
public:
    DeckGUI(DJAudioPlayer* player,
        juce::AudioFormatManager& formatManagerToUse,
        juce::AudioThumbnailCache& cacheToUse);
    ~DeckGUI() override;

    juce::URL getCurrentAudioURL() const;
    void setDeckGUI3(DeckGUI3* deckGUI3Component); 

    void paint(juce::Graphics&) override;
    void resized() override;

    void buttonClicked(juce::Button*) override;
    void sliderValueChanged(juce::Slider* slider) override;

    bool isInterestedInFileDrag(const juce::StringArray& file) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;
    void timerCallback() override;

    void updateTitleLabel(const juce::String& newTitle);
    void updateWaveformDisplay(const juce::URL& audioURL); 

private:
    juce::Label currentTimeLabel;
    DeckGUI3* deckGUI3 = nullptr;

    juce::TextButton playButton{ "Play" };
    juce::TextButton stopButton{ "Stop" };
    juce::Slider volSlider{ "Volume" };
    juce::TextButton loadButton{ "Load" };
    juce::Slider speedSlider{ "SpeedSlider" };
    juce::FileChooser fChooser{ "Select a file..." };
    juce::Slider posSlider{ "PosSlider" };
    juce::Label bpmLabel{ "BPM", "BPM: 0" };
    juce::Label volLabel;
    juce::Label speedLabel;
    juce::Label posLabel;
    juce::Label lengthLabel;
    juce::Label titleLabel;

    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
