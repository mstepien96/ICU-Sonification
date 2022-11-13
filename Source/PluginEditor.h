/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class ICUSonificationAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::FilenameComponentListener
{
public:
    ICUSonificationAudioProcessorEditor (ICUSonificationAudioProcessor&);
    ~ICUSonificationAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void readFile (const juce::File& fileToRead) {
        if (!fileToRead.existsAsFile()) {
            return;
        }
        
        auto fileText = fileToRead.loadFileAsString();
        
        juce::FileInputStream inputStream(fileToRead);
        
        if (!inputStream.openedOk()) {
            return;
        }
        
        // Clearing after loading
        textContent->clear();
        
        // Counter used in ensuring it doesn't load the 12 million rows
        juce::int64 counter = 1;
        
        while(!inputStream.isExhausted()) {
            counter++;
            auto line = inputStream.readNextLine();
            
            textContent->insertTextAtCaret(line + juce::newLine);
            
            if (counter > 253) {
                break;
            }
        }
    }
    
    // Changing path for file and update the filename

    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override {
        if (fileComponentThatHasChanged == fileComp.get()) {
            readFile (fileComp->getCurrentFileText());
        }
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ICUSonificationAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ICUSonificationAudioProcessorEditor)
    
    /// File reader
    std::unique_ptr<juce::FilenameComponent> fileComp;
    /// Text field
    std::unique_ptr<juce::TextEditor> textContent;
};
