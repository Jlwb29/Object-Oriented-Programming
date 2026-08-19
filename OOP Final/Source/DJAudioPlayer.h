#pragma once
#include <JuceHeader.h>  

class DJAudioPlayer : public juce::AudioSource {
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSec);
    void setPositionRelative(double pos);
    float getBPM() const;
    void detectBPM(); 
    void detectOnsets(const juce::AudioBuffer<float>& buffer, std::vector<int>& onsetIndices);
    float calculateBPM(const std::vector<int>& onsetIndices, float sampleRate);
    bool isTrackLoaded() const;
    juce::int64 getTrackLength() const; 
    void clearTrack(); 
    void start();
    void stop();
    double getPositionRelative();
    juce::URL getAudioURL() const; 

private:
    juce::AudioFormatManager& formatManager;
    float bpm{ 0.0f };
    bool trackLoaded; 
    juce::AudioFormatReader* audioReader; 
    juce::URL currentAudioURL; 

    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{ &transportSource, false, 2 };
};

