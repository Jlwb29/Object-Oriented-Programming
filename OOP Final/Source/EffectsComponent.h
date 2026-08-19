#pragma once

#include <JuceHeader.h>
#include "CustomTextEditor.h"

class EffectsComponent : public juce::Component,
    public juce::TableListBoxModel,
    public juce::Button::Listener,
    public juce::ChangeListener,
    public juce::AudioSource
{
public:
    EffectsComponent();
    ~EffectsComponent() override;

    int getNumRows() override;
    void resized() override;
    void paintRowBackground(juce::Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(juce::Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;

    void buttonClicked(juce::Button* button) override;
    void addEffectTrack(const juce::File& file);
    juce::File getEffectTrackFileById(int id) const;
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    void performSearch();

    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    juce::TextEditor searchBox;
    void paint(juce::Graphics& g) override; 

    void initializeEffectTracks();
    void setupAudioDevice();

private:
    juce::TabbedComponent tabbedComponent;
    juce::TextButton searchButton;
    std::vector<juce::String> filteredEffectTitles;
    std::vector<juce::String> effectTitles;
    juce::TableListBox tableComponent;
    std::vector<juce::File> effectFiles;
    juce::AudioTransportSource transportSource;
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioDeviceManager audioDeviceManager;
    juce::AudioSourcePlayer audioSourcePlayer;
    juce::TextButton stopButton; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EffectsComponent)
};
