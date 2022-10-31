/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "FaustPolySynth.h"

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
}

ICUSonificationAudioProcessor::~ICUSonificationAudioProcessor()
{
}

//==============================================================================
const juce::String ICUSonificationAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

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
    driver = new dummyaudio(sampleRate, samplesPerBlock);
    faustObject = new FaustPolyEngine(NULL, driver, NULL);
    outputs = new float* [2];
    for (int channel = 0; channel < 2; ++channel) {
        outputs[channel] = new float[samplesPerBlock];
    }
}

void ICUSonificationAudioProcessor::releaseResources()
{
    //delete faustObject;
    delete driver;
    for (int channel = 0; channel < 2; ++channel) {
        delete[] outputs[channel];
    }
    delete[] outputs;
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

    faustObject->compute(buffer.getNumSamples(), NULL, outputs);

    for (int channel = 0; channel < totalNumOutputChannels; ++channel) {
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

void ICUSonificationAudioProcessor::keyOn(int pitch, int velocity)
{
    faustObject->keyOn(pitch, velocity);
}

void ICUSonificationAudioProcessor::keyOff(int pitch)
{
    faustObject->keyOff(pitch);
}

void ICUSonificationAudioProcessor::setCutoff(float cutoff)
{
    faustObject->setParamValue("cutoff", cutoff);
}