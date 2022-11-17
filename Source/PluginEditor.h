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
 should add this when it is used
 public juce::HighResolutionTimer
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
            juce::String trimmed = line.trim();
            juce::String trimmed2 = trimmed.trim();
        
            juce::StringArray tokens;
            
            tokens.addTokens(trimmed2, "  ");
            
            //textContent->insertTextAtCaret(line + juce::newLine);
            for (int i=0; i<tokens.size()-1; i++) {
                
                juce::String trimmedToken = tokens[i].trimCharactersAtEnd("\t");
                
                if (counter > 3) {
                    textContent->insertTextAtCaret(trimmedToken + " ");
                }
            }
            
            if (counter > 3) {
                textContent->insertTextAtCaret(juce::newLine);
            }
            
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
