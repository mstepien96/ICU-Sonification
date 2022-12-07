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
    
    float filterData(float input);
    
    void calculateBIQCoeff(float fCutoffFreq, float fQ);

    int mapDataToFreq(float ECGdata, float dataMin, float dataMax, int freqMin, int freqMax);  // maps data from .txt to freq in int

    void setGate(bool gate);

    unsigned long timeMilliseconds = 0;  // counter for High Resolution Timer

    int ECGcounter = 0;  // counter for accessing datapoints from dataArray
    float samplingRate = 0.001; // was 0.004 from 250 Hz
    int modForSamplingRate = 1;
    
    bool isPlaying = false;    // turns audio on/off
    bool streamPicker = false; // true chooses first data, false the second
    bool dataRead = false;     // has data been read yet?
    bool dataReadTwo = false;  // has the second data sheet been read?
    
    std::vector<float> dataVector; // Dynamic float array for reading in ECG data
    
    std::vector<float> dataVector2; // Dynamic float array for reading in ECG data
    
    float input_z1 = 0, input_z2 = 0;
    float output_z1 = 0, output_z2 = 0;
    
    float thresholdValue = 0.5; 
    
    double fs = 1000;
    
    double a0 = 1;
    double a1 = 0;
    double a2 = 0;
    double b1 = 0;
    double b2 = 0;

private:
    MapUI* fUI;
    dsp* fDSP;
    float** outputs;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICUSonificationAudioProcessor)
};
