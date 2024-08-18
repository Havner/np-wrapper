# What is this?

This is a very simple DLL library that acts like a wrapper for NPClient64.dll
(64bit version), which is a client library for TrackIR software.

# Purpose

The only purpose of this wrapper for now is to modify the values for
NP_GetData() so it's possible to modify values requested from TrackIR before
they are passed to the game.

This achieves a single purpose of changing the default angles when you recenter
your TrackIR and is particularly useful for games that don't allow to set camera
angles that are then honored with TrackIR turned on, namely Falcon 4 BMS.

And this solution works properly with the TrackIR configured deadzone.

# Installation

Due to the fact that the client library resides in the TrackIR software
installation directory and most games load them from there by a full path the
only method to install this wrapper is globally.

Find a directory where TrackIR software is installed. This will most probably
be: "C:\Program Files (x86)\NaturalPoint\TrackIR5" or something similar.

Rename the client library that is there:

    NPClient64.dll -> NPClient64-orig.dll

And then put the wrapper library inside that directory. Rename first so you
won't overwrite! Make use to rename exactly to the filename above. This is not
just for backup. The original library is still required.

Reinstallation or update of the TrackIR software necessitates reinstall of the
wrapper as the original libraries will be put in place.

# Configuration

Put the NPWrapper.ini inside that directory as well. This is a very simple file,
no comments, no sections, one line per game.

    TRACK_IR_GAME_ID ROLL_SHIFT PITCH_SHIFT YAW_SHIFT

The provided config has a line for BMS that shifts the pitch camera angle 5
degrees down.

Negative and float values are acceptable.

# Further remarks

This has been ONLY tested with:

- TrackIR 4 hardware (although it shouldn't matter)
- TrackIR 5.4.2 software (recently released 5.5 doesn't work for me)
- Falcon 4 BMS game
- only the 64bit library

If you want to add configuration for other games you need to know TrackIR game
ID for them. An easy way is to use a debug version of this library and check the
log file (C:\Users\USERNAME\NPWrapper64.txt).

    Read config called with ID: 1901
