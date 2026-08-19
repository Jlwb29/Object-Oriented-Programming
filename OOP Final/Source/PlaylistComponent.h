#pragma once
#include <JuceHeader.h>
#include "CustomTextEditor.h"
#include "EffectsComponent.h" 
#include <vector>
#include <string>

class PlaylistComponent  : public juce::Component, 
    public juce::TableListBoxModel, 
    public juce::Button::Listener,
    public juce::ChangeListener 

{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

    int getNumRows() override;  
    void paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;  
    void paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override; 
    juce::Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) override;  
    juce::File getTrackFileById(int id) const;
    void addTrack(const juce::File& file);
    void initializePlaylist(); 
    void performSearch();
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;

private:
    CustomTextEditor searchBox; 
    juce::TextButton searchButton;
    std::vector<juce::String> filteredTitles;
    std::vector<juce::String> trackTitles;
    std::vector<juce::File> trackFiles;
    juce::TabbedComponent tabbedComponent; 
    std::unique_ptr<juce::Component> playlistTab;  
    std::unique_ptr<juce::Component> effectsTab;   
    juce::TableListBox tableComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
