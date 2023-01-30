# SPEAR

Welcome to SPEAR, the 
**S**tatic **P**redictive **E**nergy **A**nalysis Tool based on **R**APL.

The tool will be developed for my bachelor-thesis
**"Predictive, static energy consumption analysis based on experimentally determined energy models"**

## Introduction

Modern computers, especially in high computing applications require a lot of energy.
Data centers, which handle all of our modern Cloud-Infrastructure, are trying to save every last bit of energy to reduce costs.
Even though energy seems to be a problematic factor regarding cost and environment, most
developers are not well aware about the energy-consumption of their software. Either trough missing information about their
used architecture or through the abstraction their used language implements.

To work towards filling this knowledge-gap, this bachelor-thesis will try to develop a tool
for static analysis of LLVM-IR Code which will get populated with energy-consumption profiling
from the Intel RAPL Interface.

