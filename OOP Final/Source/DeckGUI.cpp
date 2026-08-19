#include <JuceHeader.h>
#include "DeckGUI.h"

DeckGUI::DeckGUI(DJAudioPlayer* _player,
    juce::AudioFormatManager& formatManagerToUse,
    juce::AudioThumbnailCache& cacheToUse)
    : player(_player), waveformDisplay(formatManagerToUse, cacheToUse), deckGUI3(nullptr) 
{
    addAndMakeVisible(playButton);
    addAndMakeVisible(volSlider);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    addAndMakeVisible(waveformDisplay);
    addAndMakeVisible(bpmLabel); 
    addAndMakeVisible(titleLabel);
    volSlider.setSliderStyle(juce::Slider::LinearVertical);
    speedSlider.setSliderStyle(juce::Slider::LinearVertical);
    posSlider.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);  
    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    addAndMakeVisible(volLabel);

    speedLabel.setText("Speed", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    addAndMakeVisible(speedLabel);

    posLabel.setText("", juce::dontSendNotification);
    posLabel.attachToComponent(&posSlider, false);
    addAndMakeVisible(posLabel);
    addAndMakeVisible(lengthLabel); 
    posSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    bpmLabel.setText("BPM: 0", juce::dontSendNotification);
    titleLabel.setText("No track loaded", juce::dontSendNotification);

    startTimer(500); 
    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(1.0, 5.0);
    posSlider.setRange(0.0, 1.0);
    double rowH = getHeight() / 8;

    addAndMakeVisible(currentTimeLabel); 

    currentTimeLabel.setText("--:--", juce::dontSendNotification);
    currentTimeLabel.setFont(juce::Font(14.0f));
    currentTimeLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(titleLabel);
    titleLabel.setJustificationType(juce::Justification::centred);
    titleLabel.setFont(juce::Font(16.0f));
    titleLabel.setText("No track loaded", juce::dontSendNotification); 

    startTimer(500);

}

void DeckGUI::timerCallback()
{
    waveformDisplay.setPositionRelative(player->getPositionRelative());

    float bpm = player->getBPM();
    bpmLabel.setText("BPM: " + juce::String(bpm), juce::dontSendNotification);

    if (player->isTrackLoaded())
    {
        juce::int64 lengthInSamples = player->getTrackLength();
        double sampleRate = 44100; 
        double lengthInSeconds = lengthInSamples / sampleRate;
        juce::String lengthStr;
        lengthStr << ((int)(lengthInSeconds / 60)) << ":" << juce::String(((int)(lengthInSeconds) % 60)).paddedLeft('0', 2);
        lengthLabel.setText(lengthStr, juce::dontSendNotification);

        double currentTimeInSeconds = player->getPositionRelative() * lengthInSeconds;
        juce::String currentTimeStr;
        if (currentTimeInSeconds >= 0 && lengthInSeconds > 0)
        {
            currentTimeStr << ((int)(currentTimeInSeconds / 60)) << ":" << juce::String(((int)(currentTimeInSeconds) % 60)).paddedLeft('0', 2);
        }
        else
        {
            currentTimeStr = "--:--";
        }
        currentTimeLabel.setText(currentTimeStr, juce::dontSendNotification);

        if (lengthInSeconds > 0)
        {
            double positionRatio = player->getPositionRelative();
            posSlider.setValue(positionRatio, juce::dontSendNotification);
        }
    }
    else
    {
        lengthLabel.setText("--:--", juce::dontSendNotification);
        currentTimeLabel.setText("--:--", juce::dontSendNotification);
    }


}



DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.drawRect(getLocalBounds(), 1);

    juce::Rectangle<int> groupedBounds = currentTimeLabel.getBounds().getUnion(posSlider.getBounds());

    g.setColour(juce::Colours::white);
    g.drawRect(groupedBounds, 1);

    g.setColour(juce::Colours::white);
    g.drawRect(titleLabel.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(bpmLabel.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(playButton.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(stopButton.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(loadButton.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(volSlider.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(speedSlider.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(waveformDisplay.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.drawRect(currentTimeLabel.getBounds(), 1);

    g.setColour(juce::Colours::white);
    g.setFont(juce::Font(14.0f));
    g.drawText("DeckGUI", getLocalBounds(), juce::Justification::centred, true);
}



void DeckGUI::resized()
{
    double rowH = getHeight() / 8;
    double waveformHeight = rowH * 3; 
    double sliderWidth = getWidth() / 5; 
    double buttonWidth = getWidth() / 6; 
    double buttonHeight = rowH / 2; 
    double waveformTop = rowH * 3; 

    if (waveformHeight > 0 && buttonWidth > 0 && buttonHeight > 0)
    {
        waveformDisplay.setBounds(0, rowH * 5.5, getWidth(), rowH);
        bpmLabel.setBounds(0, rowH, getWidth(), rowH / 2);
        lengthLabel.setBounds(getWidth() - sliderWidth, getHeight() - buttonHeight - rowH, (getWidth() - sliderWidth * 2) / 3, rowH);
        currentTimeLabel.setBounds(0, getHeight() - buttonHeight - rowH, (getWidth() - sliderWidth * 2) / 3, rowH);
        playButton.setBounds(0, rowH * 5, buttonWidth, buttonHeight);
        stopButton.setBounds(buttonWidth, rowH * 5, buttonWidth, buttonHeight);
        volSlider.setBounds(getWidth() - sliderWidth * 2, rowH * 2, sliderWidth, getHeight() - waveformTop - rowH - buttonHeight);
        speedSlider.setBounds(getWidth() - sliderWidth, rowH * 2, sliderWidth, getHeight() - waveformTop - rowH - buttonHeight);
        posSlider.setBounds(getWidth() / 5, getHeight() - buttonHeight - rowH, getWidth() - sliderWidth * 2, rowH);
        titleLabel.setBounds(0, rowH * 1.5, getWidth() - sliderWidth * 2, getHeight() - waveformTop - rowH - buttonHeight);
        loadButton.setBounds(0, getHeight() - buttonHeight, getWidth(), buttonHeight);
    }
}

void DeckGUI::buttonClicked(juce::Button* button)
{
    if (button == &playButton)
    {
        juce::Logger::writeToLog("Play button is pressed");
        player->start();
    }

    if (button == &stopButton)
    {
        player->stop();
    }

    if (button == &loadButton)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;

        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File chosenFile = chooser.getResult();

                if (chosenFile.existsAsFile())
                {
                    juce::URL audioURL = juce::URL{ chosenFile };
                    player->loadURL(audioURL);
                    updateTitleLabel(chosenFile.getFileNameWithoutExtension());
                    waveformDisplay.loadURL(audioURL);
                    juce::Logger::writeToLog("Loaded file: " + chosenFile.getFullPathName());

                    if (deckGUI3) 
                    {
                        deckGUI3->setWaveformFromURL(audioURL);
                        deckGUI3->duplicateWaveformFromDeckGUI2(); 

                    }
                    }
                    else
                    {
                        juce::Logger::writeToLog("No file was selected or the file does not exist.");
                    }
            });
    }
}

juce::URL DeckGUI::getCurrentAudioURL() const
{
    return player->getAudioURL(); 
}

void DeckGUI::setDeckGUI3(DeckGUI3* deckGUI3Component)
{
    deckGUI3 = deckGUI3Component;
}

void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        double positionRatio = slider->getValue();
        player->setPositionRelative(positionRatio);

        if (deckGUI3)
        {
            deckGUI3->updateWaveformPosition(positionRatio);
        }
    }
}


bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& file)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const juce::StringArray& files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        juce::File file(files[0]); 
        juce::URL url(file);       
        player->loadURL(url);      
    }
}


void DeckGUI::updateTitleLabel(const juce::String& newTitle)
{
    titleLabel.setText(newTitle, juce::dontSendNotification);
}

void DeckGUI::updateWaveformDisplay(const juce::URL& audioURL)
{
    waveformDisplay.loadURL(audioURL);  
}



