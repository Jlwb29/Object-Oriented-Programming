#include "EffectsComponent.h"

EffectsComponent::EffectsComponent() : tabbedComponent(juce::TabbedButtonBar::TabsAtTop)
{
    tableComponent.getHeader().addColumn("Effect Title", 1, 400);
    tableComponent.getHeader().addColumn("Play", 2, 100);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    searchBox.setTextToShowWhenEmpty("Search...", juce::Colours::white);
    addAndMakeVisible(searchBox);
    searchBox.onTextChange = [this] { performSearch(); };

    searchButton.setButtonText("Search");
    searchButton.addListener(this);
    addAndMakeVisible(searchButton);

    stopButton.setButtonText("Stop");
    stopButton.addListener(this);
    addAndMakeVisible(stopButton);

    formatManager.registerBasicFormats();
    transportSource.addChangeListener(this);
    setupAudioDevice();
    initializeEffectTracks();
}

EffectsComponent::~EffectsComponent()
{
    transportSource.removeChangeListener(this);
}

int EffectsComponent::getNumRows()
{
    return filteredEffectTitles.size();
}

void EffectsComponent::resized()
{
    searchBox.setBounds(10, getHeight() - 40, getWidth() - 120, 30); 
    tableComponent.setBounds(0, 0, getWidth(), getHeight());
    searchButton.setBounds(getWidth() - 100, getHeight() - 40, 90, 30);
    stopButton.setBounds(getWidth() - 100, getHeight() - 80, 90, 30); 

}


void EffectsComponent::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::lightgrey);
}

void EffectsComponent::paintRowBackground(juce::Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    g.fillAll(rowIsSelected ? juce::Colours::orange : juce::Colours::darkgrey);
}

void EffectsComponent::performSearch() {
    juce::String searchText = searchBox.getText().trim(); 
    filteredEffectTitles.clear();

    if (searchText.isEmpty() || searchText == "Search in Effects...")
    {
        filteredEffectTitles = effectTitles; 
    }
    else
    {
        for (const auto& title : effectTitles)
        {
            if (title.toLowerCase().contains(searchText.toLowerCase()))
            {
                filteredEffectTitles.push_back(title);
            }
        }
    }

    std::cout << "Search Text: " << searchText << std::endl;
    std::cout << "Filtered Titles: " << filteredEffectTitles.size() << std::endl;
    for (const auto& title : filteredEffectTitles)
    {
        std::cout << "Filtered Title: " << title << std::endl;
    }

    tableComponent.updateContent(); 
}

void EffectsComponent::paintCell(juce::Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (rowNumber >= 0 && rowNumber < filteredEffectTitles.size()) {
        juce::String trackTitle = filteredEffectTitles[rowNumber];

        switch (columnId) {
        case 1: 
            g.drawText(trackTitle, 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        case 2: 
            g.drawText("Play", 2, 0, width - 4, height, juce::Justification::centredLeft, true);
            break;
        }
    }
}

juce::Component* EffectsComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, juce::Component* existingComponentToUpdate)
{
    if (existingComponentToUpdate == nullptr)
    {
        juce::TextButton* btn = new juce::TextButton{};
        btn->addListener(this);
        btn->setComponentID(juce::String(rowNumber));

        if (columnId == 2) 
        {
            btn->setButtonText("Play");
        }
        else
        {
            return existingComponentToUpdate;
        }

        existingComponentToUpdate = btn;
    }
    return existingComponentToUpdate;
}

void EffectsComponent::buttonClicked(juce::Button* button)
{
    if (button == &stopButton)
    {
        transportSource.stop();
        transportSource.setSource(nullptr);
        return;
    }

    int id = button->getComponentID().getIntValue();

    if (id >= 0 && id < effectFiles.size())
    {
        juce::File effectFile = getEffectTrackFileById(id);

        if (effectFile.existsAsFile())
        {
            if (button->getButtonText() == "Play")
            {
                transportSource.stop();
                transportSource.setSource(nullptr);

                auto* reader = formatManager.createReaderFor(effectFile);
                if (reader != nullptr)
                {
                    readerSource.reset(new juce::AudioFormatReaderSource(reader, true));
                    transportSource.setSource(readerSource.get(), 0, nullptr, reader->sampleRate);
                    transportSource.start();
                }

            }
        }
    }
}

void EffectsComponent::setupAudioDevice()
{
    audioDeviceManager.initialiseWithDefaultDevices(0, 2);
    audioDeviceManager.addAudioCallback(&audioSourcePlayer);
    audioSourcePlayer.setSource(this);
}

juce::File EffectsComponent::getEffectTrackFileById(int id) const
{
    if (id >= 0 && id < effectFiles.size())
    {
        return effectFiles[id];
    }
    return juce::File{};
}

void EffectsComponent::addEffectTrack(const juce::File& file)
{
    if (file.existsAsFile())
    {
        effectFiles.push_back(file);
        effectTitles.push_back(file.getFileNameWithoutExtension());

        juce::String searchText = searchBox.getText().toLowerCase();
        if (searchText.isEmpty() || file.getFileNameWithoutExtension().toLowerCase().contains(searchText))
        {
            filteredEffectTitles.push_back(file.getFileNameWithoutExtension());
        }

        tableComponent.updateContent();
    }
}

void EffectsComponent::initializeEffectTracks()
{
    juce::File effect1("C:/Users/user/Downloads/ZAYN - Dusk Till Dawn (Official Video) ft. Sia.mp3");
    juce::File effect2("C:/Users/user/Downloads/Jingle.mp3");
    addEffectTrack(effect1);
    addEffectTrack(effect2);
}

void EffectsComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void EffectsComponent::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    transportSource.getNextAudioBlock(bufferToFill);
}

void EffectsComponent::releaseResources()
{
    transportSource.releaseResources();
}

void EffectsComponent::changeListenerCallback(juce::ChangeBroadcaster* source)
{
}
