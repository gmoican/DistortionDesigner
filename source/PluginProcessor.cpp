#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), state(*this, nullptr, "parameters", createParams())
{
}

PluginProcessor::~PluginProcessor()
{
}

bool PluginProcessor::symOn = true;
int PluginProcessor::posFunc = DEFAULT_FUNC;
int PluginProcessor::negFunc = DEFAULT_FUNC;
float PluginProcessor::driveGain = juce::Decibels::decibelsToGain(DEFAULT_DRIVE);
float PluginProcessor::posCorr = DEFAULT_CORR;
float PluginProcessor::negCorr = DEFAULT_CORR;
float PluginProcessor::posFx = DEFAULT_FX;
float PluginProcessor::negFx = DEFAULT_FX;

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

// =========== PARAMETER LAYOUT ====================
juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // Parameters in UI header
    params.push_back(std::make_unique<juce::AudioParameterFloat>("INPUT", "Input Level", juce::NormalisableRange<float>(-30.0f, 30.0f, 0.1f), DEFAULT_INPUT, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("OUTPUT", "Output Level", juce::NormalisableRange<float>(-30.0f, 30.0f, 0.1f), DEFAULT_OUTPUT, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("GATE", "Gate Threshold", juce::NormalisableRange<float>(-100.0f, 0.0f, 0.1f), DEFAULT_GATE, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MIX", "Mix", juce::NormalisableRange<float>(0.0f, 100.0f, 0.1f), DEFAULT_MIX, "%"));
    
    // Parameters in PreEQ section
    params.push_back(std::make_unique<juce::AudioParameterBool>("PRE_ON", "PreEQ On/Off", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_HPF", "PreEQ HPF", juce::NormalisableRange<float>(10.0f, 200.0f, 0.1f), DEFAULT_HPF, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_LPF", "PreEQ LPF", juce::NormalisableRange<float>(2000.0f, 20000.0f, 0.1f), DEFAULT_LPF, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_LOW_GAIN", "PreEQ Low Gain", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_LOW_FREQ", "PreEQ Low Freq", juce::NormalisableRange<float>(60.0f, 200.0f, 0.1f), DEFAULT_LOW_FREQ, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_LOW_Q", "PreEQ Low Q", juce::NormalisableRange<float>(0.5f, 4.0f, 0.1f), DEFAULT_Q, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_MID_GAIN", "PreEQ Mid Gain", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_MID_FREQ", "PreEQ Mid Freq", juce::NormalisableRange<float>(250.0f, 1000.0f, 0.1f), DEFAULT_MID_FREQ, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_MID_Q", "PreEQ Mid Q", juce::NormalisableRange<float>(0.5f, 4.0f, 0.1f), DEFAULT_Q, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_HIGH_GAIN", "PreEQ High Gain", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_HIGH_FREQ", "PreEQ High Freq", juce::NormalisableRange<float>(750.0f, 3000.0f, 0.1f), DEFAULT_HIGH_FREQ, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PRE_HIGH_Q", "PreEQ High Q", juce::NormalisableRange<float>(0.5f, 4.0f, 0.1f), DEFAULT_Q, ""));
    
    // Parameters in Distortion section
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRIVE", "Drive Gain", juce::NormalisableRange<float>(-18.0f, 30.0f, 0.1f), DEFAULT_DRIVE, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("BIAS", "Drive Bias", juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f), DEFAULT_BIAS, ""));
    params.push_back(std::make_unique<juce::AudioParameterBool>("SYM", "Drive Symetry", true));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POS_CORR", "Positive Drive Correction", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), DEFAULT_CORR, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("NEG_CORR", "Negative Drive Correction", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), DEFAULT_CORR, ""));
    params.push_back(std::make_unique<juce::AudioParameterInt>("POS_FUNC", "Positive Drive Function", 0, 5, DEFAULT_FUNC));
    params.push_back(std::make_unique<juce::AudioParameterInt>("NEG_FUNC", "Negative Drive Function", 0, 5, DEFAULT_FUNC));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POS_FX", "Positive Drive Custom f(x)", juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f), DEFAULT_FX, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("NEG_FX", "Negative Drive Custom f(x)", juce::NormalisableRange<float>(-1.0f, 1.0f, 0.01f), DEFAULT_FX, ""));
    
    // Parameters in Compression section
    params.push_back(std::make_unique<juce::AudioParameterBool>("COMP_ON", "Comp On/Off", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_RATIO", "Comp Ratio", juce::NormalisableRange<float>(2.0f, 40.0f, 0.1f), DEFAULT_RATIO, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_THRES", "Comp threshold", juce::NormalisableRange<float>(-20.0f, 0.0f, 0.1f), DEFAULT_THRES, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_ATT", "Comp Attack", juce::NormalisableRange<float>(0.1f, 50.0f, 0.1f), DEFAULT_ATT, "ms"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("COMP_REL", "Comp Release", juce::NormalisableRange<float>(5.0f, 2500.0f, 0.1f), DEFAULT_REL, "ms"));
    
    // Parameters in PostEQ section
    params.push_back(std::make_unique<juce::AudioParameterBool>("POST_ON", "PostEQ On/Off", true));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_HPF", "PostEQ HPF", juce::NormalisableRange<float>(10.0f, 200.0f, 0.1f), DEFAULT_HPF, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_LPF", "PostEQ LPF", juce::NormalisableRange<float>(2000.0f, 20000.0f, 0.1f), DEFAULT_LPF, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_LOW_GAIN", "PostEQ Low Gain", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_LOW_FREQ", "PostEQ Low Freq", juce::NormalisableRange<float>(60.0f, 200.0f, 0.1f), DEFAULT_LOW_FREQ, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_LOW_Q", "PostEQ Low Q", juce::NormalisableRange<float>(0.5f, 4.0f, 0.1f), DEFAULT_Q, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_MID_GAIN", "PostEQ Mid Gain", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_MID_FREQ", "PostEQ Mid Freq", juce::NormalisableRange<float>(250.0f, 1000.0f, 0.1f), DEFAULT_MID_FREQ, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_MID_Q", "PostEQ Mid Q", juce::NormalisableRange<float>(0.5f, 4.0f, 0.1f), DEFAULT_Q, ""));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_HIGH_GAIN", "PostEQ High Gain", juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f), 0, "dB"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_HIGH_FREQ", "PostEQ High Freq", juce::NormalisableRange<float>(750.0f, 3000.0f, 0.1f), DEFAULT_HIGH_FREQ, "Hz"));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("POST_HIGH_Q", "PostEQ High Q", juce::NormalisableRange<float>(0.5f, 4.0f, 0.1f), DEFAULT_Q, ""));
    
    return { params.begin(), params.end() };
}

// ============ VALUE UPDATERS =====================
void PluginProcessor::updateInLevel()
{
    auto INPUT = state.getRawParameterValue("INPUT");
    inputLevel.setGainDecibels(INPUT->load());
}

void PluginProcessor::updateOutLevel()
{
    auto OUTPUT = state.getRawParameterValue("OUTPUT");
    outputLevel.setGainDecibels(OUTPUT->load());
}

void PluginProcessor::updateGate()
{
    auto GATE = state.getRawParameterValue("GATE");
    noiseGate.setThreshold(GATE->load());
}

void PluginProcessor::updateMix()
{
    auto MIX = state.getRawParameterValue("MIX");
    dryWetMix.setWetMixProportion(MIX->load() / 100.0f);
}

void PluginProcessor::updatePreEQ()
{
    auto ONOFF = state.getRawParameterValue("PRE_ON") -> load();
    preeqOn = ONOFF;

    auto HPF = state.getRawParameterValue("PRE_HPF") -> load();
    auto LPF = state.getRawParameterValue("PRE_LPF") -> load();
    auto LO_GAIN = state.getRawParameterValue("PRE_LOW_GAIN") -> load();
    auto LO_FREQ = state.getRawParameterValue("PRE_LOW_FREQ") -> load();
    auto LO_Q = state.getRawParameterValue("PRE_LOW_Q") -> load();
    auto MID_GAIN = state.getRawParameterValue("PRE_MID_GAIN") -> load();
    auto MID_FREQ = state.getRawParameterValue("PRE_MID_FREQ") -> load();
    auto MID_Q = state.getRawParameterValue("PRE_MID_Q") -> load();
    auto HIGH_GAIN = state.getRawParameterValue("PRE_HIGH_GAIN") -> load();
    auto HIGH_FREQ = state.getRawParameterValue("PRE_HIGH_FREQ") -> load();
    auto HIGH_Q = state.getRawParameterValue("PRE_HIGH_Q") -> load();
    
    LO_GAIN = juce::Decibels::decibelsToGain(LO_GAIN);
    MID_GAIN = juce::Decibels::decibelsToGain(MID_GAIN);
    HIGH_GAIN = juce::Decibels::decibelsToGain(HIGH_GAIN);
    
    //
    
    if (preeqOn)
    {
        double sampleRate = getSampleRate();
        
        *preEq.get<0>().state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, HPF);
        *preEq.get<1>().state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, LPF);
        *preEq.get<2>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, LO_FREQ, LO_Q, LO_GAIN);
        *preEq.get<3>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, MID_FREQ, MID_Q, MID_GAIN);
        *preEq.get<4>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, HIGH_FREQ, HIGH_Q, HIGH_GAIN);
    }
}

void PluginProcessor::updatePostEQ()
{
    auto ONOFF = state.getRawParameterValue("POST_ON") -> load();
    posteqOn = ONOFF;
    
    auto HPF = state.getRawParameterValue("POST_HPF") -> load();
    auto LPF = state.getRawParameterValue("POST_LPF") -> load();
    auto LO_GAIN = state.getRawParameterValue("POST_LOW_GAIN") -> load();
    auto LO_FREQ = state.getRawParameterValue("POST_LOW_FREQ") -> load();
    auto LO_Q = state.getRawParameterValue("POST_LOW_Q") -> load();
    auto MID_GAIN = state.getRawParameterValue("POST_MID_GAIN") -> load();
    auto MID_FREQ = state.getRawParameterValue("POST_MID_FREQ") -> load();
    auto MID_Q = state.getRawParameterValue("POST_MID_Q") -> load();
    auto HIGH_GAIN = state.getRawParameterValue("POST_HIGH_GAIN") -> load();
    auto HIGH_FREQ = state.getRawParameterValue("POST_HIGH_FREQ") -> load();
    auto HIGH_Q = state.getRawParameterValue("POST_HIGH_Q") -> load();
    
    LO_GAIN = juce::Decibels::decibelsToGain(LO_GAIN);
    MID_GAIN = juce::Decibels::decibelsToGain(MID_GAIN);
    HIGH_GAIN = juce::Decibels::decibelsToGain(HIGH_GAIN);
    
    //
    
    if (posteqOn)
    {
        double sampleRate = getSampleRate();
        
        *postEq.get<0>().state = *juce::dsp::IIR::Coefficients<float>::makeHighPass(sampleRate, HPF);
        *postEq.get<1>().state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(sampleRate, LPF);
        *postEq.get<2>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, LO_FREQ, LO_Q, LO_GAIN);
        *postEq.get<3>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, MID_FREQ, MID_Q, MID_GAIN);
        *postEq.get<4>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, HIGH_FREQ, HIGH_Q, HIGH_GAIN);
    }
}

void PluginProcessor::updateDistortion()
{
    auto DRIVE = state.getRawParameterValue("DRIVE") -> load();
    auto BIAS = state.getRawParameterValue("BIAS") -> load();
    
    auto SYM = state.getRawParameterValue("SYM") -> load();
    auto PCORR = state.getRawParameterValue("POS_CORR") -> load();
    auto NCORR = state.getRawParameterValue("NEG_CORR") -> load();
    auto PFUNC = state.getRawParameterValue("POS_FUNC") -> load();
    auto NFUNC = state.getRawParameterValue("NEG_FUNC") -> load();
    auto PFX = state.getRawParameterValue("POS_FX") -> load();
    auto NFX = state.getRawParameterValue("NEG_FX") -> load();
    
    //
    
    PluginProcessor::driveGain = juce::Decibels::decibelsToGain(DRIVE);
    PluginProcessor::symOn = SYM;
    PluginProcessor::posCorr = PCORR;
    PluginProcessor::negCorr = NCORR;
    PluginProcessor::posFunc = PFUNC;
    PluginProcessor::negFunc = NFUNC;
    PluginProcessor::posFx = PFX;
    PluginProcessor::negFx = NFX;
    
    drive.get<0>().setGainDecibels(DRIVE);
    drive.get<1>().setBias(BIAS);
}

void PluginProcessor::updateComp()
{
    auto ONOFF = state.getRawParameterValue("COMP_ON") -> load();
    compOn = ONOFF;
    
    auto RAT = state.getRawParameterValue("COMP_RATIO") -> load();
    auto THR = state.getRawParameterValue("COMP_THRES") -> load();
    auto ATT = state.getRawParameterValue("COMP_ATT") -> load();
    auto REL = state.getRawParameterValue("COMP_REL") -> load();
    
    if (compOn)
    {
        comp.setRatio(RAT);
        comp.setThreshold(THR);
        comp.setAttack(ATT);
        comp.setRelease(REL);
    }
}

void PluginProcessor::updateState()
{
    updateInLevel();
    updateGate();
    updatePreEQ();
    updateDistortion();
    updateComp();
    updatePostEQ();
    updateMix();
    updateOutLevel();
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    spec.sampleRate = sampleRate;
    
    inputLevel.prepare(spec);
    inputLevel.setRampDurationSeconds(0.1);
    outputLevel.prepare(spec);
    outputLevel.setRampDurationSeconds(0.1);
    
    noiseGate.prepare(spec);
    noiseGate.setThreshold(DEFAULT_GATE);
    dryWetMix.prepare(spec);
    
    preEq.prepare(spec);
    preEq.reset();
    postEq.prepare(spec);
    postEq.reset();
    
    comp.prepare(spec);
    comp.setRatio(DEFAULT_RATIO);
    comp.setThreshold(DEFAULT_THRES);
    comp.setAttack(DEFAULT_ATT);
    comp.setRelease(DEFAULT_REL);
    
    drive.prepare(spec);
    drive.reset();
    drive.get<0>().setRampDurationSeconds(0.1);
    drive.get<2>().functionToUse = shapeBlender;
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    updateState();
    
    juce::dsp::AudioBlock<float> audioBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> audioCtx(audioBlock);
    
    // Input processing
    inputLevel.process(audioCtx);
    dryWetMix.pushDrySamples(audioBlock);
    noiseGate.process(audioCtx);
    
    // PreEQ processing
    if (preeqOn)
        preEq.process(audioCtx);
    
    // Distortion processing
    auto& inputBlock = audioCtx.getInputBlock();
    driveOV.processSamplesUp(inputBlock);
    drive.process(audioCtx);
    auto& outputBlock = audioCtx.getOutputBlock();
    driveOV.processSamplesDown(outputBlock);
    
    // Compression processing
    if (compOn)
        comp.process(audioCtx);
    
    // PostEQ processing
    if (posteqOn)
        postEq.process(audioCtx);
    
    // Output processing
    dryWetMix.mixWetSamples(audioBlock);
    outputLevel.process(audioCtx);
    
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

// ========== DRIVE MATH FUNCTIONS =============================================
float PluginProcessor::softClipping(float sample)
{
    return sample / (abs(sample) + 1.f);
}

float PluginProcessor::hardClipping(float sample)
{
    if (sample > 1.f)
        return 2.f / 3.f;
    else if (sample < -1.f)
        return -2.f / 3.f;
    else
        return sample - std::pow(sample, 3.f) / 3.f;
}

float PluginProcessor::arcTanClipping(float sample)
{
    return 2.f / juce::MathConstants<float>::pi * std::atan(sample);
}

float PluginProcessor::tanhClipping(float sample)
{
    return 2.f / juce::MathConstants<float>::pi * juce::dsp::FastMathApproximations::tanh(sample);
}

float PluginProcessor::sinFold(float sample)
{
    return std::sin(sample);
}

float PluginProcessor::customClipping(float sample, float driveVal, float fxVal)
{
    return std::atan(sample) / std::atan(driveVal) * fxVal + sample / driveVal * (1.f - fxVal);
}

// This function manages all the previous drive functions in the processor
float PluginProcessor::shapeBlender(float sample)
{
    float processedSample;
    
    if (sample >= 0) {
        switch (posFunc) {
            case 0:
                processedSample = posCorr * tanhClipping(sample);
                break;
                
            case 1:
                processedSample = posCorr * arcTanClipping(sample);
                break;
                
            case 2:
                processedSample = posCorr * hardClipping(sample);
                break;
                
            case 3:
                processedSample = posCorr * softClipping(sample);
                break;
                
            case 4:
                processedSample = posCorr * sinFold(sample);
                break;
                
            case 5:
                processedSample = posCorr * customClipping(sample, driveGain, posFx);
                break;
                
            default:
                processedSample = 0.f;
                break;
        }
    } else {
        switch (negFunc) {
            case 0:
                processedSample = negCorr * tanhClipping(sample);
                break;
                
            case 1:
                processedSample = negCorr * arcTanClipping(sample);
                break;
                
            case 2:
                processedSample = negCorr * hardClipping(sample);
                break;
                
            case 3:
                processedSample = negCorr * softClipping(sample);
                break;
                
            case 4:
                processedSample = negCorr * sinFold(sample);
                break;
                
            case 5:
                processedSample = negCorr * customClipping(sample, driveGain, negFx);
                break;
                
            default:
                processedSample = 0.f;
                break;
        } 
    }
    
    return processedSample;
}

// Util for UI function graph
float PluginProcessor::getDriveGain() { return driveGain; }
