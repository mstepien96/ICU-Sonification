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
class ICUSonificationAudioProcessorEditor  : public juce::AudioProcessorEditor, public juce::FilenameComponentListener, public juce::Slider::Listener
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
                        // String to Float
                        float floatValue = token.getFloatValue();
                        
                        // If value index is 1 we will insert the mV value in our dynamic vector array
                        if (valueIndex == 1) {
                            audioProcessor.dataVector.push_back(floatValue);
                        }
                        valueIndex++;
                    }
                }
                valueIndex = 0;
            }
            
            // Breaker
            counter++;
            if (counter > 10000) {  // was 252 & 2502
                audioProcessor.dataRead = true;
                textContent->insertTextAtCaret("Reading finished");
                break;
            }
        }
        
        audioProcessor.dataRead = true;
    }
    
    void readFileST(const juce::File& fileToRead) {
        if (!fileToRead.existsAsFile()) {
            return;
        }
        
        auto fileText = fileToRead.loadFileAsString();
        
        juce::FileInputStream inputStream(fileToRead);
        
        if (!inputStream.openedOk()) {
            return;
        }
        
        textContent->clear();
        
        juce::int64 counter = 0;
        
        while(!inputStream.isExhausted()) {
            auto line = inputStream.readNextLine();
            juce::String trimmed = line.trim();
            
            float floatValue = trimmed.getFloatValue();
            
            audioProcessor.dataVector.push_back(floatValue);
        
            counter++;
            
            if (counter > 100000) {
                audioProcessor.dataRead = true;
                textContent->insertTextAtCaret("Reading finished");
                break;
            }
        }
        
        audioProcessor.dataRead = true;
    }
    
    void readFileST2(const juce::File& fileToRead) {
        if (!fileToRead.existsAsFile()) {
            return;
        }
        
        auto fileText = fileToRead.loadFileAsString();
        
        juce::FileInputStream inputStream(fileToRead);
        
        if (!inputStream.openedOk()) {
            return;
        }
        
        textContent->clear();
        
        juce::int64 counter = 0;
        
        while(!inputStream.isExhausted()) {
            auto line = inputStream.readNextLine();
            juce::String trimmed = line.trim();
            
            float floatValue = trimmed.getFloatValue();
            
            audioProcessor.dataVector2.push_back(floatValue);
            
            counter++;
            
            if (counter > 100000) {
                break;
            }
        }
        
        audioProcessor.dataReadTwo = true;
    }
    
    
    // Changing path for file and update the filename
    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override {
        if (fileComponentThatHasChanged == fileComp.get()) {
            if (audioProcessor.dataRead == false) {
                readFileST (fileComp->getCurrentFileText());
            } else if (audioProcessor.dataRead == true && audioProcessor.dataReadTwo == false) {
                readFileST2(fileComp->getCurrentFileText());
            }
        }
    }
    
    void sliderValueChanged(Slider* slider) override {
        if (slider == &freqCutOff) {
            audioProcessor.calculateBIQCoeff(freqCutOff.getValue() , 0.7);
        } else if (slider == &threshold) {
            audioProcessor.thresholdValue = threshold.getValue();
        }
    }
    
    void readDefaultData() {
        juce::File STElevation("/Users/duyx/Private/AAU/Semester1/SMC01/MatLab/ST-elevatedData.txt");
        juce::File normalData ("/Users/duyx/Private/AAU/Semester1/SMC01/MatLab/normalData.txt");
    
        readFileST(STElevation);
        readFileST2(normalData);
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
    String recordingLength;
    String ECGamplitude;

    TextButton stateChange;
    
    /// Sliders
    Slider freqCutOff;
    Slider threshold;
    Label freqCutOffLabel;
    Label thresholdLabel;
};
