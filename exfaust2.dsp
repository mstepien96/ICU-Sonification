
declare name "freeverb";
declare version "0.0";
declare author "RM";
declare description "Freeverb demo application.";

import("stdfaust.lib");

gate = button("gate") : si.smoo;
gatesound = button("gatesound") : si.smoo;


process = (pm.SFFormantModelFofSmooth_ui *gatesound + sine(freqsine)* (1 - gatesound))* gate <:_,_ : dm.freeverb_demo :_,_; 


declare name "Vocal FOF";
declare description "FOF vocal synthesizer.";
declare license "MIT";
declare copyright "(c)Mike Olsen, CCRMA (Stanford University)";


phasor(freq) = (A~B)
with{
    delta = freq/ma.SR;
    A = _;
    B = (_+delta) : ma.frac;
};

freqsine = hslider("freqsine",100,50,2000,0.01);

sine(freqsine) = phasor(freqsine)*2*ma.PI : sin;



























