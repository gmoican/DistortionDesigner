#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

#if (MSVC)
#include "ipps.h"
#endif

#define DEFAULT_INPUT 0.0f
#define DEFAULT_OUTPUT 0.0f
#define DEFAULT_GATE -80.0f
#define DEFAULT_MIX 100.0f
#define DEFAULT_HPF 60.0f
#define DEFAULT_LPF 12000.0f
#define DEFAULT_Q 1.0f
#define DEFAULT_LOW_FREQ 100.0f
#define DEFAULT_MID_FREQ 750.0f
#define DEFAULT_HIGH_FREQ 1500.0f
#define DEFAULT_DRIVE 0.0f
#define DEFAULT_BIAS 0.0f
#define DEFAULT_CORR 1.0f
#define DEFAULT_FUNC 0
#define DEFAULT_FX 0.5f
#define DEFAULT_RATIO 20.0f
#define DEFAULT_THRES -10.0f
#define DEFAULT_ATT 1.0f
#define DEFAULT_REL 10.f

class PluginProcessor : public juce::AudioProcessor
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    //=============== MY STUFF =====================================================
    juce::AudioProcessorValueTreeState state;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParams();
    using FilterBand = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
    using Gain = juce::dsp::Gain<float>;
    using Shaper = juce::dsp::WaveShaper<float>;
    using Bias = juce::dsp::Bias<float>;
    using OverSampling = juce::dsp::Oversampling<float>;
    using Compressor = juce::dsp::Compressor<float>;
    using Gate = juce::dsp::NoiseGate<float>;
    using Mix = juce::dsp::DryWetMixer<float>;
    
    // OnOff buttons
    bool preeqOn, posteqOn, compOn;
    static bool symOn;
    
    // Distortion extra parameters
    static int posFunc, negFunc;
    static float driveGain, posCorr, negCorr, posFx, negFx;
    
    // Processor chains
    juce::dsp::ProcessorChain<FilterBand, FilterBand, FilterBand, FilterBand, FilterBand> preEq, postEq;
    juce::dsp::ProcessorChain<Gain, Bias, Shaper> drive;
    
    // Other processors
    Compressor comp;
    Gain inputLevel, outputLevel;
    Gate noiseGate;
    Mix dryWetMix;
    
    // Oversampling - Much needed for any kind of distortion
    OverSampling driveOV { 2, 2, OverSampling::filterHalfBandPolyphaseIIR, true, false };
    
    // Updaters
    void updateInLevel();
    void updateGate();
    void updatePreEQ();
    void updateDistortion();
    void updateComp();
    void updatePostEQ();
    void updateMix();
    void updateOutLevel();
    void updateState();
    
    // Drive functions
    static float softClipping(float sample);
    static float hardClipping(float sample);
    static float arcTanClipping(float sample);
    static float tanhClipping(float sample);
    static float sinFold(float sample);
    static float customClipping(float sample, float driveVal, float fxVal);
    
    // Waveshaper blender ?
    static float shapeBlender(float sample);
        
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
