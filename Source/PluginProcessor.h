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

    int mapDataToFreq(float ECGdata, float dataMin, float dataMax, int freqMin, int freqMax);  // maps data from .txt to freq in int

    unsigned long timeMilliseconds = 0;  // counter for High Resolution Timer

    int ECGcounter = 0;  // counter for accessing datapoints from dataArray 
    
    bool isPlaying = false;  // turns audio on/off
    bool dataRead = false;  // has data been read yet?
    
    float dataArray[8000][2];  // float array for reading in ECG data

private:
    MapUI* fUI;
    dsp* fDSP;
    float** outputs;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICUSonificationAudioProcessor)
};
