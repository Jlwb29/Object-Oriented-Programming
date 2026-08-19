#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{
}

DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    resampleSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr)
    {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader, true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());

        detectBPM();
        trackLoaded = true; 

    }
}

bool DJAudioPlayer::isTrackLoaded() const
{
    return trackLoaded;
}

void DJAudioPlayer::clearTrack()
{
    trackLoaded = false;
}

juce::int64 DJAudioPlayer::getTrackLength() const
{
    if (readerSource != nullptr)
    {
        return readerSource->getAudioFormatReader()->lengthInSamples;
    }
    return 0;
}



void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain should be between 0 and 1" << std::endl;
    }
    else
    {
        transportSource.setGain(gain);
    }
}

void DJAudioPlayer::setSpeed(double ratio)
{
    if (ratio < 1 || ratio > 5.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
    }
    else
    {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSec)
{
    transportSource.setPosition(posInSec);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
    if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else
    {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void DJAudioPlayer::start()
{
    transportSource.start();
}

void DJAudioPlayer::stop()
{
    transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

void DJAudioPlayer::detectOnsets(const juce::AudioBuffer<float>& buffer, std::vector<int>& onsetIndices)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    const float threshold = 0.7f; 

    for (int channel = 0; channel < numChannels; ++channel)
    {
        const float* channelData = buffer.getReadPointer(channel);

        for (int i = 1; i < numSamples; ++i)
        {
            if (std::abs(channelData[i]) > threshold && std::abs(channelData[i - 1]) < threshold)
            {
                onsetIndices.push_back(i);
            }
        }
    }
}

float DJAudioPlayer::calculateBPM(const std::vector<int>& onsetIndices, float sampleRate)
{
    if (onsetIndices.size() < 2)
        return 0.0f;

    float totalInterval = 0.0f;
    int count = 0;

    for (size_t i = 1; i < onsetIndices.size(); ++i)
    {
        float interval = (onsetIndices[i] - onsetIndices[i - 1]) / sampleRate;
        totalInterval += interval;
        ++count;
    }

    float averageInterval = totalInterval / count;
    float bpm = 60.0f / averageInterval;

    return bpm;
}

void DJAudioPlayer::detectBPM()
{
    if (readerSource != nullptr)
    {
        auto* reader = readerSource->getAudioFormatReader();

        if (reader != nullptr)
        {
            int numSamples = static_cast<int>(reader->lengthInSamples);
            juce::AudioBuffer<float> buffer(2, numSamples);

            reader->read(&buffer, 0, numSamples, 0, true, true);

            std::vector<int> onsetIndices;
            detectOnsets(buffer, onsetIndices);
            bpm = calculateBPM(onsetIndices, reader->sampleRate);

            juce::Logger::writeToLog("Detected BPM: " + juce::String(bpm));
        }
        else
        {
            juce::Logger::writeToLog("AudioFormatReader is null.");
        }
    }
    else
    {
        juce::Logger::writeToLog("ReaderSource is null.");
    }
}

float DJAudioPlayer::getBPM() const
{
    return bpm;
}

juce::URL DJAudioPlayer::getAudioURL() const
{
    return currentAudioURL; 
}
