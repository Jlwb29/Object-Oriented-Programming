#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include "EffectsComponent.h"
#include "MainComponent.h"
#include "DJAudioPlayer.h"
#include "CustomTextEditor.h"

PlaylistComponent::PlaylistComponent()
    : tabbedComponent(juce::TabbedButtonBar::TabsAtTop),
    searchBox("Search...")  
{
    playlistTab = std::make_unique<juce::Component>();
    playlistTab->setName("Playlist Tab");

    tableComponent.getHeader().addColumn("Track Title", 1, 400);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Left Player", 3, 100);
    tableComponent.getHeader().addColumn("Right Player", 4, 100);
    tableComponent.getHeader().addColumn("Delete", 5, 100);
    tableComponent.setModel(this);

    playlistTab->addAndMakeVisible(tableComponent);

    tabbedComponent.addTab("Playlist", juce::Colours::lightgrey, playlistTab.get(), true);

    effectsTab = std::make_unique<EffectsComponent>();
    effectsTab->setName("Effects Tab");
    tabbedComponent.addTab("Effects", juce::Colours::lightgrey, effectsTab.get(), true);

    addAndMakeVisible(tabbedComponent);

    addAndMakeVisible(searchBox);
    searchBox.onTextChange = [this] { performSearch(); };

    searchButton.setButtonText("Search");
    searchButton.addListener(this);
    addAndMakeVisible(searchButton);

    tabbedComponent.getTabbedButtonBar().addChangeListener(this);

    initializePlaylist();
}

int PlaylistComponent::getNumRows() {
    return filteredTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) {
    g.fillAll(rowIsSelected ? juce::Colours::orange : juce::Colours::darkgrey);
}

void PlaylistComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (rowNumber >= 0 && rowNumber < filteredTitles.size()) {
        juce::String trackTitle = filteredTitles[rowNumber];

        switch (columnId) {
        case 1:
            g.drawText(trackTitle, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        case 2: 
            g.drawText("3:00", 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        }
    }
}

PlaylistComponent::~PlaylistComponent() {}

void PlaylistComponent::paint(juce::Graphics& g) {
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);
}

void PlaylistComponent::resized() {
    tabbedComponent.setBounds(0, 0, getWidth(), getHeight());

    if (tabbedComponent.getCurrentTabName() == "Playlist") {
        searchBox.setVisible(true);
        searchButton.setVisible(true);
        searchBox.setBounds(10, getHeight() - 40, getWidth() - 120, 30);
        searchButton.setBounds(getWidth() - 100, getHeight() - 40, 90, 30);
    }
    else {
        searchBox.setVisible(false);
        searchButton.setVisible(false);
    }

    tableComponent.setBounds(0, 0, getWidth(), getHeight()); 
}

void PlaylistComponent::changeListenerCallback(juce::ChangeBroadcaster* source) {
    if (source == &tabbedComponent.getTabbedButtonBar()) {
        resized();  
    }
}



void PlaylistComponent::performSearch() {
    juce::String searchText = searchBox.getText();
    filteredTitles.clear();

    if (searchText.isEmpty() || searchText == "Search...")
    {
        filteredTitles = trackTitles; 
    }
    else
    {
        for (const auto& title : trackTitles)
        {
            if (title.toLowerCase().contains(searchText.toLowerCase()))
            {
                filteredTitles.push_back(title);
            }
        }
    }

    std::cout << "Search Text: " << searchText << std::endl;
    std::cout << "Filtered Titles: " << filteredTitles.size() << std::endl;
    for (const auto& title : filteredTitles)
    {
        std::cout << "Filtered Title: " << title << std::endl;
    }

    tableComponent.updateContent(); 
}


juce::Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate) {
    if (existingComponentToUpdate == nullptr) {
        juce::TextButton* btn = new juce::TextButton{};
        btn->addListener(this);
        btn->setComponentID(std::to_string(rowNumber));

        switch (columnId) {
        case 3:
            btn->setButtonText("Left Load");
            break;
        case 4:
            btn->setButtonText("Right Load");
            break;
        case 5:
            btn->setButtonText("Delete");
            break;
        default:
            return existingComponentToUpdate;
        }

        existingComponentToUpdate = btn;
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button) {
    int id = std::stoi(button->getComponentID().toStdString());

    if (id >= 0 && id < trackFiles.size()) {
        juce::File trackFile = getTrackFileById(id);

        if (trackFile.existsAsFile()) {
            std::cout << "Button clicked for ID: " << id << ", Track file exists: " << trackFile.getFullPathName().toStdString() << std::endl;

            if (button->getButtonText() == "Left Load") {
                if (auto* mainComponent = findParentComponentOfClass<MainComponent>()) {
                    mainComponent->loadTrackIntoPlayer1(trackFile);
                }
                else {
                    std::cout << "MainComponent not found." << std::endl;
                }
            }
            else if (button->getButtonText() == "Right Load") {
                if (auto* mainComponent = findParentComponentOfClass<MainComponent>()) {
                    mainComponent->loadTrackIntoPlayer2(trackFile);
                }
                else {
                    std::cout << "MainComponent not found." << std::endl;
                }
            }
        }
        else {
            std::cout << "Track file does not exist." << std::endl;
        }
    }
    else {
        std::cout << "Invalid ID: " << id << std::endl;
    }
}

juce::File PlaylistComponent::getTrackFileById(int id) const {
    if (id >= 0 && id < trackFiles.size()) {
        return trackFiles[id];
    }
    else {
        std::cout << "Invalid ID: " << id << std::endl;
        return juce::File{};
    }
}

void PlaylistComponent::addTrack(const juce::File& file) {
    if (file.existsAsFile()) {
        trackFiles.push_back(file);
        trackTitles.push_back(file.getFileNameWithoutExtension().toStdString());
        filteredTitles = trackTitles; 
        tableComponent.updateContent(); 
    }
    else {
        std::cout << "Invalid file path: " << file.getFullPathName().toStdString() << std::endl;
    }
}

void PlaylistComponent::initializePlaylist() {
    juce::File file1("C:/Users/user/Downloads/ZAYN - Dusk Till Dawn (Official Video) ft. Sia.mp3");
    juce::File file2("C:/Users/user/Downloads/Evanescence - Bring Me To Life (Official HD Music Video).mp3");
    addTrack(file1);
    addTrack(file2);
}


