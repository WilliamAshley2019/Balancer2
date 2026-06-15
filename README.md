--------------------------------------------------------------------------------------------------
Copyright (c) 2026 William Ashley d/b/a William Ashley Music ( http://WilliamAshley.music )
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License  (v3) 

This program is distributed in the hope that it will be useful to other audio programmers and music makers in their own plugin designs.
There is no WARRANTY expressed or implied including for MERCHANTABILITY or FITNESS FOR ANY PURPOSE. 
See the GNU General Public License for more details.

Attributtion is requested where possible if you use or modify any of the source,
Notice of use is requested so I can familiarize myself with how the code has been adapted for personal interest.
contact@WilliamAshley.music   
-----------------------------------------------------------------------------------------------------
[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![JUCE](https://img.shields.io/badge/Built%20with-JUCE%208.0.12-blue)](https://juce.com)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20-lightgrey)]()
[![Format](https://img.shields.io/badge/Format-VST3%20%7C%20-orange)]()


# Balancer (Gain Mono) VST3 Plugin (Gain & Mono Utility)

**Version:** 2.01 (Alpha)  
**License:** GPL-3.0  

Balancer (GainMono) is a utility VST3 plugin designed for mixing, mastering, and gain staging. It provides precise control over audio levels and features a high-quality stereo-to-mono summing function. 

This is an update to "Balancer", this plugin focuses strictly on Gain staging and Mono Summing.*

## Features

- **Precise Gain Control**: Adjustable from -24dB to +24dB with smooth, zipper-noise-free automation.
- **Stereo-to-Mono Summing**: Toggle to sum all input channels to mono. Uses a -6dB (0.5x) attenuation scale for stereo sources to mimic a passive summer and prevent clipping.
- **Bypass Switch**: Instantly A/B the processed signal with the dry signal.
- **CPU Optimized**: Features an "early exit" architecture. If the plugin is bypassed, or set to 0dB gain with Mono off, it consumes virtually zero CPU.
- **Professional Audio Engine**: Utilizes JUCE's optimized, SIMD-friendly buffer operations (`addFrom` / `copyFrom`) for safe, alias-free channel summing.
- **Broad Compatibility**: Supports standard VST3 hosts (FL Studio, Cubase, Reaper, Ableton Live, etc.) with flexible Mono/Stereo I/O configurations. This plugin
- has only been tested in FL Studio 2025, if you have issues with another daw feel free to email contact@williamashley.music 

## Screenshots

![GainMono VST3 Plugin UI](https://raw.githubusercontent.com/WilliamAshley2019/Balancer2/main/balancergainmono.png)
## Build Instructions

### Requirements
- **Operating System:** Windows 10/11 (Tested on Windows 11)
- **IDE:** Visual Studio 2022 (Windows) or Xcode (macOS)
- **JUCE Framework:** Version 8.0.12
- **VST3 SDK:** Steinberg VST3 SDK 3.7 or newer

### Steps
1. Clone the repository:
   ```bash
   git clone https://github.com/WilliamAshley2019/Balancer2.git
   cd Balancer2
