/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class dsp;
class MapUI;

class ICUSonificationAudioProcessor  : public juce::AudioProcessor, public juce::HighResolutionTimer
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
{
public:
    //==============================================================================
    ICUSonificationAudioProcessor();
    ~ICUSonificationAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void hiResTimerCallback();  // High Resolution Timer for sending data from JUCE to Faust engine

    int mapData(float ECGdata, float dataMin, float dataMax, int freqMin, int freqMax);  // maps data from .txt to freq in int
    int mapDataLog(float freqMin, float freqMax, int dataToSonify, int numberOfDataPoints);
    int freqToSonify;
    int freqToSonifyLog;
    int gainToSonify;
    int vowelToSonify;

    void setGate(bool gate);
    void setGateSound(bool gateSound);

    unsigned long timeMilliseconds = 0;  // counter for High Resolution Timer

    int ECGcounter = 0;  // counter for accessing datapoints from dataArray
    float samplingRate = 0.001; // was 0.004 from 250 Hz
    int modForSamplingRate = 1;
    
    bool isPlaying = false;    // turns audio on/off
    bool gateSoundChoice = false;    // choose the sound
    bool streamPicker = false; // true chooses first data, false the second
    bool dataRead = false;     // has data been read yet?
    bool dataReadTwo = false;  // has the second data sheet been read?
    float threshold = 0.2;  // threshold for sonification
    
    std::vector<float> dataVector; // Dynamic float array for reading in ECG data
    float dataMin1;
    float dataMax1;
    
    std::vector<float> dataVector2; // Dynamic float array for reading in ECG data
    float dataMin2;
    float dataMax2;

private:
    MapUI* fUI;
    dsp* fDSP;
    float** outputs;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICUSonificationAudioProcessor)
};
