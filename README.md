# NBody_SIMD_Test
Testing SIMD in an NBody Simulation environment

This was my first attempt at SIMD, trying out on a brute force n-body simulation. I looked at the results of a series of approaches - I thought they were quite interesting and wanted to share - the code can be found here https://github.com/s0lly/NBody_SIMD_Test :

AoS approach: this is just the typically naive approach of having one object for each entity, which contains its own location, velocity, mass info, and applies gravity calcs to each

SoA approach: this approach uses one object called "entities", which has separate arrays of location data, velocity data, and mass data. This ensures that all similar data is packed together tightly.

SoA + SIMD: this approach uses SIMD instructions to speed up the calculations by applying the same calculation to 8 floats at a time.

SoA + SIMD + mt: This approach adds multithreading in, to up them gains.

I was surprised that the SoA approach actually added time rather than sped up the program, but that may be because I am a terrible programmer (I'd love for anyone to analyse my code and criticise the hell out of it!). It may be because the entity size is small enough (just 5x floats) for the AoS approach to not be a burden, in this case.

The SoA approach does lead very nicely into using SIMD, however, and the gains there impressed me a lot!

I found it very interesting that a combination of optimisations increased the performance of a naive n-body approach by 14x, fast enough to run 8000 bodies on the CPU without any tree algorithms, at a decent FPS (about 30 FPS after drawing routines).

What also surprised me was that (to my understanding) the floating point error creates quite different results in the various simulations, as the step-by-step calculations are in slightly different orders. I didn't realise to what extent floating point error can change results so dramatically! (unless I got this vastly wrong!)

Interested in your thoughts on this, and if there is any way I should be approaching this instead! Cheers!
