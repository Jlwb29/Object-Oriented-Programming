#include "MainComponent.h"

MainComponent::MainComponent()
    : deckGUI1(&player1, formatManager, thumbCache),
    deckGUI2(&player2, formatManager, thumbCache),
    deckGUI3(formatManager, thumbCache, deckGUI1, deckGUI2) 
{
    setSize (800, 600);


    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        setAudioChannels (2, 2);
    }

    deckGUI1.setDeckGUI3(&deckGUI3);
    deckGUI2.setDeckGUI3(&deckGUI3);
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(deckGUI3);

    addAndMakeVisible(playlistComponent);
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

    //mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.setColour(juce::Colours::white);
    g.setFont(14.0f);
    g.drawText("PlaylistComponent", getLocalBounds(), juce::Justification::centred, true);
}

void MainComponent::resized()
{
    double deck3Height = getHeight() * 1 / 4; 
    double remainingHeight = getHeight() - deck3Height;
    double deckHeight = remainingHeight / 2; 

    deckGUI3.setBounds(0, 0, getWidth(), deck3Height);

    deckGUI1.setBounds(0, deck3Height, getWidth() / 2, deckHeight);
    deckGUI2.setBounds(getWidth() / 2, deck3Height, getWidth() / 2, deckHeight);

    double playlistHeight = getHeight() - deck3Height - deckHeight;
    playlistComponent.setBounds(0, deck3Height + deckHeight, getWidth(), playlistHeight);
}


void MainComponent::buttonClicked(juce::Button* button)
{

}



void MainComponent::sliderValueChanged(juce::Slider* slider)
{
   
}

void MainComponent::loadTrackIntoPlayer1(const juce::File& file)
{
    if (file.existsAsFile())
    {
        juce::URL audioURL(file);
        player1.loadURL(audioURL);
        deckGUI1.updateTitleLabel(file.getFileNameWithoutExtension());
        deckGUI1.updateWaveformDisplay(audioURL);  

        deckGUI3.setWaveformFromURL(audioURL);
    }
    else
    {
        std::cout << "File does not exist: " << file.getFullPathName().toStdString() << std::endl;
    }
}


void MainComponent::loadTrackIntoPlayer2(const juce::File& file)
{
    if (file.existsAsFile())
    {
        juce::URL audioURL(file);
        player2.loadURL(audioURL);
        deckGUI2.updateTitleLabel(file.getFileNameWithoutExtension());
        deckGUI2.updateWaveformDisplay(audioURL);  
    }
    else
    {
        std::cout << "File does not exist: " << file.getFullPathName().toStdString() << std::endl;
    }
}

void MainComponent::loadTracks(const juce::File& file1, const juce::File& file2)
{
    loadTrackIntoPlayer1(file1);
    loadTrackIntoPlayer2(file2);
}
