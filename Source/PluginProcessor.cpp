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
    calculateBIQCoeff(1, 0.7);
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

int ICUSonificationAudioProcessor::mapDataToFreq(float ECGdata, float dataMin, float dataMax, int freqMin, int freqMax) {
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
            //    // int freqToSonify = abs(std::min(int(dataArray[ECGcounter][1] * 1000), 2000));
            //    // int freqToSonify2 = std::max(freqToSonify, 50);
            
            if (streamPicker) {
                float output = filterData(dataVector2[ECGcounter]);
                int freqToSonify = mapDataToFreq(output, -0.1, 0.5, 50, 2000);
                if (dataVector2[ECGcounter] > thresholdValue) {
                    fUI->setParamValue("freq", freqToSonify);
                } else {
                    fUI->setParamValue("freq", 0.0);
                }
            } else {
                float output = filterData(dataVector[ECGcounter]);
                int freqToSonify = mapDataToFreq(output, -0.1, 0.5, 50, 2000);
                if (dataVector[ECGcounter] > thresholdValue) {
                    fUI->setParamValue("freq", freqToSonify);
                } else {
                    fUI->setParamValue("freq", 0.0);
                }
            }
            
            ECGcounter++;
        }
        
        if (ECGcounter > (int(dataVector.size()) - 2 )) {
            ECGcounter = 0;
        }

        if (ECGcounter >= (int(dataVector.size()) - 1)) {
            isPlaying = false;
            setGate(isPlaying);
        }
    }
};

float ICUSonificationAudioProcessor::filterData(float input) {
    
    float y_n = a0 * input + a1 * input_z1 + a2 * input_z2 - b1 * output_z1 - b2 * output_z2;
    
    output_z2 = output_z1;
    output_z1 = y_n;
    
    input_z2 = input_z1;
    input_z1 = input;
    
    return y_n;
}

void ICUSonificationAudioProcessor::calculateBIQCoeff(float fCutoffFreq, float fQ) {
    // use same terms as in book:
    float theta_c = 2.0 * M_PI*fCutoffFreq / 1000;
    float d = 1.0 / fQ;
    
    // intermediate values
    float fBetaNumerator = 1.0 - ((d / 2.0) * (sin(theta_c)));
    float fBetaDenominator = 1.0 + ((d / 2.0) * (sin(theta_c)));

    // beta
    float fBeta = 0.5 * (fBetaNumerator / fBetaDenominator);

    // gamma
    float fGamma = (0.5 + fBeta) * (cos(theta_c));

    // alpha
    //float fAlpha = (0.5 + fBeta - fGamma) / 2.0;
    
    a0 = (0.5 + fBeta - fGamma) / 2.0;
    a1 = 0.5 + fBeta - fGamma;
    a2 = (0.5 + fBeta - fGamma) / 2.0;
    b1 = -2 * fGamma;
    b2 = 2 * fBeta;
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

