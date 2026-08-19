#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "MergingWaveformDisplay.h"  
#include "DeckGUI3.h"

class MainComponent : public juce::AudioAppComponent, public juce::Button::Listener, public juce::Slider::Listener
{
public:
    
    MainComponent();
    ~MainComponent() override;

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void paint (juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button *) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void loadTrackIntoPlayer1(const juce::File& file); 
    void loadTrackIntoPlayer2(const juce::File& file); 
    void loadTracks(const juce::File& file1, const juce::File& file2);


private:
    PlaylistComponent playlistComponent;
    DJAudioPlayer player1{ formatManager };
    DJAudioPlayer player2{ formatManager };
    DJAudioPlayer player3{ formatManager };
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache };
    juce::Label titleLabel;
    DeckGUI3 deckGUI3{ formatManager, thumbCache, deckGUI1, deckGUI2 }; 
    juce::MixerAudioSource mixerSource;
    juce::AudioFormatManager formatManager;
    juce::AudioThumbnailCache thumbCache{ 100 }; 
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
