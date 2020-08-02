# Admer's SoHL 1.2 for VS2019
  
Based on [Solokiller's updated SDK](https://github.com/Solokiller/halflife-updated) and [Spirit of Half-Life 1.2](https://www.moddb.com/mods/spirit-of-half-life/downloads/sohl-v12-with-source-code). Also contains some new entities, new functionality and small bugfixes.   

## Differences from vanilla SoHL 1.2

First of all, it's based on Solokiller's updated HL SDK, which by itself is based on [HL SDK 2.4](https://github.com/ValveSoftware/halflife). It is ideal for Steam Half-Life, but it won't run on WON Half-Life or any pre-2013 build of GoldSrc.  

The new entities include:  
* ambient_mp3 - plays MP3s from the `media` folder by name
* ambient_2d - plays 2D WAV sounds on the client, there's no stereo panning that happens with ambient_generic  

Other than that, it features a wireframe overlay for entity bounding boxes and player movement, small bugfixes and other minor enhancements. 

## Why?

A few months ago, a friend told me he was experiencing crashes in a map transition. I told him I'd debug it. His mod was using SoHL 1.2 and I couldn't find any SoHL ported to VS2017 or 2019. So I said "Screw it, I'll port it myself."   
  
Now you can enjoy your C++11/14/17/20 features. No need to get VS6.0 to compile this thing now. :P   

### A note for future forks. And "Why did you put your name in there?"

I've heard several stories about unofficial SoHL updates, after Laurie Cheers stopped working on the project. SoHL 1.3, 1.5, then 1.8, I don't know. The point is, it didn't sound standardised whatsoever. It was almost as if random people took SoHL 1.2, made some changes and published it as the next official version. I don't think that's a good way to do it, since there might be multiple different versions under the same number. So this is just my way of letting people know whose version of SoHL this is, and whose it's based on.  

So in case you are going to fork this, not just to contribute, but to make your own version separate from this one (with significant changes/differences), it'd be a good idea to follow a similar convention, e.g. "BillySuperCoder's SoHL 1.25", in the description of which you'd write "Based on Admer's SoHL 1.2 for VS2019" with a link to it.

Half Life 1 SDK LICENSE
======================

Half Life 1 SDK Copyright© Valve Corp.  

THIS DOCUMENT DESCRIBES A CONTRACT BETWEEN YOU AND VALVE CORPORATION (“Valve”).  PLEASE READ IT BEFORE DOWNLOADING OR USING THE HALF LIFE 1 SDK (“SDK”). BY DOWNLOADING AND/OR USING THE SOURCE ENGINE SDK YOU ACCEPT THIS LICENSE. IF YOU DO NOT AGREE TO THE TERMS OF THIS LICENSE PLEASE DON’T DOWNLOAD OR USE THE SDK.

You may, free of charge, download and use the SDK to develop a modified Valve game running on the Half-Life engine.  You may distribute your modified Valve game in source and object code form, but only for free. Terms of use for Valve games are found in the Steam Subscriber Agreement located here: http://store.steampowered.com/subscriber_agreement/ 

You may copy, modify, and distribute the SDK and any modifications you make to the SDK in source and object code form, but only for free.  Any distribution of this SDK must include this license.txt and third_party_licenses.txt.  
 
Any distribution of the SDK or a substantial portion of the SDK must include the above copyright notice and the following: 

DISCLAIMER OF WARRANTIES.  THE SOURCE SDK AND ANY OTHER MATERIAL DOWNLOADED BY LICENSEE IS PROVIDED “AS IS”.  VALVE AND ITS SUPPLIERS DISCLAIM ALL WARRANTIES WITH RESPECT TO THE SDK, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY, NON-INFRINGEMENT, TITLE AND FITNESS FOR A PARTICULAR PURPOSE.  

LIMITATION OF LIABILITY.  IN NO EVENT SHALL VALVE OR ITS SUPPLIERS BE LIABLE FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR INABILITY TO USE THE ENGINE AND/OR THE SDK, EVEN IF VALVE HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  
 
 
If you would like to use the SDK for a commercial purpose, please contact Valve at sourceengine@valvesoftware.com.


Half-Life 1
======================

This is the README for the Half-Life 1 engine and its associated games.

Please use this repository to report bugs and feature requests for Half-Life 1 related products.

Reporting Issues
----------------

If you encounter an issue while using Half-Life 1 games, first search the [issue list](https://github.com/ValveSoftware/halflife/issues) to see if it has already been reported. Include closed issues in your search.

If it has not been reported, create a new issue with at least the following information:

- a short, descriptive title;
- a detailed description of the issue, including any output from the command line;
- steps for reproducing the issue;
- your system information.\*; and
- the `version` output from the in‐game console.

Please place logs either in a code block (press `M` in your browser for a GFM cheat sheet) or a [gist](https://gist.github.com).

\* The preferred and easiest way to get this information is from Steam's Hardware Information viewer from the menu (`Help -> System Information`). Once your information appears: right-click within the dialog, choose `Select All`, right-click again, and then choose `Copy`. Paste this information into your report, preferably in a code block.

Conduct
-------


There are basic rules of conduct that should be followed at all times by everyone participating in the discussions.  While this is generally a relaxed environment, please remember the following:

- Do not insult, harass, or demean anyone.
- Do not intentionally multi-post an issue.
- Do not use ALL CAPS when creating an issue report.
- Do not repeatedly update an open issue remarking that the issue persists.

Remember: Just because the issue you reported was reported here does not mean that it is an issue with Half-Life.  As well, should your issue not be resolved immediately, it does not mean that a resolution is not being researched or tested.  Patience is always appreciated.
