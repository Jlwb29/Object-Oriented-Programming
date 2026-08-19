#pragma once

#include <JuceHeader.h>
#include "WaveformDisplay.h"
#include "DeckGUI.h"

class DeckGUI; 

class DeckGUI3 : public juce::Component,
    private juce::Slider::Listener,
    private juce::Button::Listener 

{
public:
    DeckGUI3(juce::AudioFormatManager& formatManager, juce::AudioThumbnailCache& thumbCache,
        DeckGUI& deck1, DeckGUI& deck2);
    ~DeckGUI3() override;
    void duplicateWaveformFromDeckGUI2(); 

    void setWaveformFromURL(const juce::URL& audioURL);

    void paint(juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button*);
    void loadWaveformFromDeckGUIs();
    void setWaveformFromURL(const juce::URL& audioURL, bool isFromDeckGUI1);
    void updateWaveformPosition(double positionRatio); 
    void setWaveformPosition(double positionRatio); 
    void setWaveformDuplicateColor(juce::Colour color);
private:
    WaveformDisplay waveformDisplay1; 
    WaveformDisplay waveformDisplay2; 
    WaveformDisplay waveformDisplayDuplicate; 
    juce::FileChooser fChooser{ "Select a file..." };
    void drawWaveform(juce::Graphics& g, juce::AudioThumbnail& thumbnail);

    juce::TextButton loadButton{ "Load" };
    juce::Slider posSlider;
    DeckGUI& deckGUI1;  
    DeckGUI& deckGUI2; 
    void sliderValueChanged(juce::Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI3)
};
