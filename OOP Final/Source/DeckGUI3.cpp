#include "DeckGUI3.h"

DeckGUI3::DeckGUI3(juce::AudioFormatManager& formatManager,
    juce::AudioThumbnailCache& thumbCache,
    DeckGUI& deck1,
    DeckGUI& deck2)
    : waveformDisplay1(formatManager, thumbCache),
    waveformDisplay2(formatManager, thumbCache),
    waveformDisplayDuplicate(formatManager, thumbCache),
    deckGUI1(deck1),  
    deckGUI2(deck2)  
{
    addAndMakeVisible(waveformDisplay1);
    addAndMakeVisible(waveformDisplay2);
    addAndMakeVisible(waveformDisplayDuplicate); 
    addAndMakeVisible(loadButton);
    loadButton.addListener(this);
    setWaveformDuplicateColor(juce::Colours::red);
}

DeckGUI3::~DeckGUI3()
{
}

void DeckGUI3::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::transparentBlack); 
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1); 
    g.setColour(juce::Colours::white);
    g.setFont(juce::FontOptions(14.0f));
    g.drawText("DeckGUI3", getLocalBounds(), juce::Justification::centred, true); 

    setWaveformDuplicateColor(juce::Colours::red);
}

void DeckGUI3::drawWaveform(juce::Graphics& g, juce::AudioThumbnail& thumbnail)
{
    g.setColour(juce::Colours::red); 
    juce::Rectangle<int> bounds = waveformDisplayDuplicate.getBounds();
    thumbnail.drawChannel(g, bounds, 0.0, thumbnail.getTotalLength(), 0, 1.0f);
}


void DeckGUI3::resized()
{
    double rowH = getHeight() / 8;
    double waveformHeight = rowH * 3; 
    double waveformWidth = getWidth(); 
    double waveformTop = rowH * 4; 

    waveformDisplay1.setBounds(0, waveformTop, waveformWidth, waveformHeight); 
    waveformDisplayDuplicate.setBounds(0, waveformTop, waveformWidth, waveformHeight); 

    waveformDisplayDuplicate.toFront(true);

    double sliderWidth = getWidth() - 20;
    double buttonHeight = rowH / 2; 

    loadButton.setBounds(0, getHeight() - buttonHeight, getWidth(), buttonHeight);
    posSlider.setBounds(10, getHeight() - buttonHeight - 30, sliderWidth, 20);
}

void DeckGUI3::loadWaveformFromDeckGUIs()
{
    auto audioURL1 = deckGUI1.getCurrentAudioURL(); 
    auto audioURL2 = deckGUI2.getCurrentAudioURL(); 

    if (!audioURL1.isEmpty())
        waveformDisplay1.loadURL(audioURL1);

    if (!audioURL2.isEmpty())
        waveformDisplayDuplicate.loadURL(audioURL2);
}

void DeckGUI3::setWaveformPosition(double positionRatio)
{
    waveformDisplay1.setPositionRelative(positionRatio);
    waveformDisplay2.setPositionRelative(positionRatio); 
    repaint();
}

void DeckGUI3::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &posSlider)
    {
        double positionRatio = slider->getValue();
        updateWaveformPosition(positionRatio);
    }
}

void DeckGUI3::updateWaveformPosition(double positionRatio)
{
    waveformDisplay1.setPositionRelative(positionRatio);
    waveformDisplay2.setPositionRelative(positionRatio); 
    repaint(); 
}

void DeckGUI3::setWaveformFromURL(const juce::URL& audioURL)
{
    waveformDisplay1.loadURL(audioURL);
    waveformDisplay2.loadURL(audioURL);
}

void DeckGUI3::setWaveformFromURL(const juce::URL& audioURL, bool isFromDeckGUI1)
{
    if (isFromDeckGUI1)
    {
        waveformDisplay1.loadURL(audioURL);
    }
    else
    {
        waveformDisplay2.loadURL(audioURL);
    }
}

void DeckGUI3::duplicateWaveformFromDeckGUI2()
{
    auto audioURL2 = deckGUI2.getCurrentAudioURL(); 

    if (!audioURL2.isEmpty())
    {
        waveformDisplayDuplicate.loadURL(audioURL2);
    }
}

void DeckGUI3::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        auto fileChooserFlags = juce::FileBrowserComponent::canSelectFiles;

        fChooser.launchAsync(fileChooserFlags, [this](const juce::FileChooser& chooser)
            {
                juce::File chosenFile = chooser.getResult();

                if (chosenFile.existsAsFile())
                {
                    juce::URL audioURL = juce::URL{ chosenFile };
                    waveformDisplayDuplicate.loadURL(audioURL);
                    juce::Logger::writeToLog("Loaded file: " + chosenFile.getFullPathName());
                }
                else
                {
                    juce::Logger::writeToLog("No file was selected or the file does not exist.");
                }
            });
    }
}

void DeckGUI3::setWaveformDuplicateColor(juce::Colour color)
{
	waveformDisplayDuplicate.setWaveformColor(color);
}