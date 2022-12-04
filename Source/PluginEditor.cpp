/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <algorithm>

//==============================================================================
ICUSonificationAudioProcessorEditor::ICUSonificationAudioProcessorEditor (ICUSonificationAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    
    // File reader initialization
    fileComp.reset (new juce::FilenameComponent ("fileComp",
                                                 {},
                                                 false,
                                                 false,
                                                 false,
                                                 {},
                                                 {},
                                                 "Select file to open"));
    
    addAndMakeVisible(fileComp.get());
    // Listener in order to update after a file has been selected
    fileComp->addListener(this);
    
    // Text Field initialization
    textContent.reset(new juce::TextEditor());
    addAndMakeVisible(textContent.get());
    textContent->setMultiLine(true);
    textContent->setReadOnly(true);
    textContent->setCaretVisible(false);
    setSize (800, 600);
    
    // Play/Pause Button
    addAndMakeVisible(playPause);
    playPause.onClick = [this] {
        audioProcessor.isPlaying = playPause.getToggleState();
        audioProcessor.setGate(audioProcessor.isPlaying);
    };
    addAndMakeVisible(playPauseLabel);
    playPauseLabel.setText("Play/Pause", juce::dontSendNotification);
    playPauseLabel.attachToComponent(&playPause, true);

    /// Rewind and Fast Forward
    addAndMakeVisible(Rewind);
    Rewind.onClick = [this] {
        audioProcessor.ECGcounter = std::max(0, int(audioProcessor.ECGcounter - 1 / audioProcessor.samplingRate));
    };
    addAndMakeVisible(RewindLabel);
    RewindLabel.setText("Rewind", juce::dontSendNotification);
    RewindLabel.attachToComponent(&Rewind, true);

    addAndMakeVisible(FastForward);
    FastForward.onClick = [this] {
        audioProcessor.ECGcounter = std::min(int(audioProcessor.dataVector.size() - 1), int(audioProcessor.ECGcounter + 1 / audioProcessor.samplingRate));
    };
    addAndMakeVisible(FastForwardLabel);
    FastForwardLabel.setText("Fast Forward", juce::dontSendNotification);
    FastForwardLabel.attachToComponent(&FastForward, true);
    
    addAndMakeVisible(stateChange);
    stateChange.setButtonText("Normal data");
    stateChange.onClick = [this] {
        audioProcessor.streamPicker = !audioProcessor.streamPicker;
        if (audioProcessor.streamPicker) {
            stateChange.setButtonText("ST-elevation");
        } else {
            stateChange.setButtonText("Normal data");
        }
    };
    
    
}

ICUSonificationAudioProcessorEditor::~ICUSonificationAudioProcessorEditor()
{
}

//==============================================================================
void ICUSonificationAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    // g.drawText("Some Text", getLocalBounds(), juce::Justification::centred, false);
    if (audioProcessor.dataRead) {
        textContent->clear();
        currentTime = juce::String(audioProcessor.ECGcounter * audioProcessor.samplingRate);
        textContent->insertTextAtCaret("Current Time: " + currentTime + " s" + juce::newLine);
        recordingLength = juce::String(int(audioProcessor.dataVector.size() * audioProcessor.samplingRate));
        textContent->insertTextAtCaret("Recording Length: " + recordingLength + " s" + juce::newLine);
        ECGamplitude = juce::String(audioProcessor.dataVector[audioProcessor.ECGcounter]);
        textContent->insertTextAtCaret("ECG amplitude: " + ECGamplitude + " mV");
    }
}

void ICUSonificationAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    fileComp->setBounds(10, 10, getWidth() - 20, 20);
    textContent->setBounds(10, 40, getWidth() - 20, 100);
    playPause.setBounds(100, 150, getWidth() - 20, 20);
    Rewind.setBounds(100, 200, 20, 20);
    FastForward.setBounds(100, 250, 20, 20);
    stateChange.setBounds(100, 300, 60, 30);
}
