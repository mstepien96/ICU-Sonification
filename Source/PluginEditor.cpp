/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
    setSize (400, 300);
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
    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void ICUSonificationAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    
    fileComp->setBounds(10, 10, getWidth() - 20, 20);
    textContent->setBounds(10, 40, getWidth() - 20, getHeight() - 50);
}
