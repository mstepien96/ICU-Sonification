/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
using namespace juce;

//==============================================================================
// public juce::HighResolutionTimer
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
        
        // Counter used in ensuring we don't load the 12 million rows
        juce::int64 counter = 0;
        // Counter for array indexing
        juce::int64 valueIndex = 0;
        
        while(!inputStream.isExhausted()) {
            auto line = inputStream.readNextLine();
            juce::String trimmed = line.trim();
            juce::StringArray tokens;
            
            // Splitting String into array of subStrings
            tokens.addTokens(trimmed, "\t");
            
            // Removing annotations from the input
            if (counter > 1) {
                // Increments through every entry in our StringArray
                for (int i = 0; i < tokens.size() - 1; i++) {
                    // Char to String
                    juce::String token = tokens[i].toStdString();
                    
                    if (token != "") {
                        //textContent->insertTextAtCaret(token + " ");
                        // String to Float
                        float floatValue = token.getFloatValue();
                        // Inserting value in respective entry & incrementing index
                        audioProcessor.dataArray[counter][valueIndex] = floatValue;
                        valueIndex++;
                        
                        // print to console
                        //DBG(token);
                    }
                }
                
                // Adding a new line
                //textContent->insertTextAtCaret(juce::newLine);
                
                // Resetting our array indexer
                valueIndex = 0;
            }
            
            // Breaker
            counter++;
            if (counter > 2502) {  // was 252
                audioProcessor.dataRead = true;
                textContent->insertTextAtCaret("Reading finished");
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

    /// Timer
    std::unique_ptr<juce::HighResolutionTimer> timer;

    /// Play/Pause button
    ToggleButton playPause;
    Label playPauseLabel;

    /// Rewind and Fast Forward
    TextButton Rewind;
    TextButton FastForward;
    Label RewindLabel;
    Label FastForwardLabel;

    /// Timesteps
    String currentTime;
};
