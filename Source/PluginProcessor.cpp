/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FaustEngine.h"
#include <algorithm>

//==============================================================================
ICUSonificationAudioProcessor::ICUSonificationAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    startTimer(1);
}

ICUSonificationAudioProcessor::~ICUSonificationAudioProcessor()
{
}

//==============================================================================
const juce::String ICUSonificationAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

int ICUSonificationAudioProcessor::mapData(float ECGdata, float dataMin, float dataMax, int freqMin, int freqMax) {
    float mappingFactor = (freqMax - freqMin) / (dataMax - dataMin);
    int ECGdataMapped = int((ECGdata - dataMin) * mappingFactor + dataMin);

    return ECGdataMapped;
}

void ICUSonificationAudioProcessor::setGate(bool gate)
{
    if (gate) {
        fUI->setParamValue("gate", 1);
    }
    else {
        fUI->setParamValue("gate", 0);
    }
}

void ICUSonificationAudioProcessor::hiResTimerCallback() {
    timeMilliseconds++;

    if (isPlaying) {
        if (dataRead && timeMilliseconds % modForSamplingRate == 0) {
            
            if (streamPicker) {
                int freqToSonify = mapData(dataVector2[ECGcounter], dataMin2 + threshold, dataMax2, 5000, 100000);
                int gainToSonify = mapData(dataVector2[ECGcounter], dataMin2 + threshold, dataMax2, 0, 100);
                int vowelToSonify = mapData(dataVector2[ECGcounter], dataMin2 + threshold, dataMax2, 0, 400);
                int vibratoFreqToSonify = mapData(dataVector2[ECGcounter], dataMin2 + threshold, dataMax2, 0, 1000);
                int vibratoGainToSonify = mapData(dataVector2[ECGcounter], dataMin2 + threshold, dataMax2, 0, 100);
                if (dataVector2[ECGcounter] > threshold) {
                    fUI->setParamValue("freq", (float)freqToSonify / 100);
                    fUI->setParamValue("gain", (float)gainToSonify / 100);
                    fUI->setParamValue("vowel", (float)vowelToSonify / 100);
                    fUI->setParamValue("vibratoFreq", (float)vibratoFreqToSonify / 100);
                    fUI->setParamValue("vibratoGain", (float)vibratoGainToSonify / 100);
                } else {
                    fUI->setParamValue("freq", 0.0);
                }
            } else {
                int freqToSonify = mapData(dataVector[ECGcounter], dataMin1 + threshold, dataMax1, 5000, 100000);
                int gainToSonify = mapData(dataVector[ECGcounter], dataMin1 + threshold, dataMax1, 0, 100);
                int vowelToSonify = mapData(dataVector[ECGcounter], dataMin2 + threshold, dataMax2, 0, 400);
                int vibratoFreqToSonify = mapData(dataVector[ECGcounter], dataMin2 + threshold, dataMax2, 0, 1000);
                int vibratoGainToSonify = mapData(dataVector[ECGcounter], dataMin2 + threshold, dataMax2, 0, 100);
                if (dataVector[ECGcounter] > threshold) {
                    fUI->setParamValue("freq", (float)freqToSonify / 100);
                    fUI->setParamValue("gain", (float)gainToSonify / 100);
                    fUI->setParamValue("vowel", (float)vowelToSonify / 100);
                    fUI->setParamValue("vibratoFreq", (float)vibratoFreqToSonify / 100);
                    fUI->setParamValue("vibratoGain", (float)vibratoGainToSonify / 100);
                } else {
                    fUI->setParamValue("freq", 0.0);
                }
            }
            
            ECGcounter++;
        }

        if (ECGcounter >= (int(dataVector.size()) - 1)) {
            isPlaying = false;
            setGate(isPlaying);
        }
    }
};

bool ICUSonificationAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ICUSonificationAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool ICUSonificationAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double ICUSonificationAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ICUSonificationAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ICUSonificationAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ICUSonificationAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String ICUSonificationAudioProcessor::getProgramName (int index)
{
    return {};
}

void ICUSonificationAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void ICUSonificationAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fDSP = new mydsp();
    fDSP->init(sampleRate);
    fUI = new MapUI();
    fDSP->buildUserInterface(fUI);
    outputs = new float* [2];
    for (int channel = 0; channel < 2; ++channel) {
        outputs[channel] = new float[samplesPerBlock];
    }
    fUI->setParamValue("gate", 0);
}

void ICUSonificationAudioProcessor::releaseResources()
{
 /*   delete fDSP;
    delete fUI;
    for (int channel = 0; channel < 2; ++channel) {
        delete[] outputs[channel];
    }
    delete[] outputs;*/
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ICUSonificationAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ICUSonificationAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    // for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    //     buffer.clear (i, 0, buffer.getNumSamples());

    fDSP->compute(buffer.getNumSamples(), NULL, outputs);

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            *buffer.getWritePointer(channel, i) = outputs[channel][i];
        }
    }
}

//==============================================================================
bool ICUSonificationAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* ICUSonificationAudioProcessor::createEditor()
{
    return new ICUSonificationAudioProcessorEditor (*this);
}

//==============================================================================
void ICUSonificationAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ICUSonificationAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ICUSonificationAudioProcessor();
}

