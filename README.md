# TauControls
A small library of a few simple window controls like button, checkbox, divider, slider and some other. If you need non-standard controls to your program, the TauControls might be the right thing. Especially suitable for "hacker" styled programs.

Copyright © Jan Ringoš, zlib license.
Marked by Jan as disconninued and archived.


**Available controls**

    BButton
    This control acts almost like common button but it shows graphics not text.
    
    MSStatic
    Simple static text, but can be filled with many strings that can be switched with single message.
    
    TauButton
    This control acts almost like common button except it animates when mouse passes it and when clicked.
    
    TauDivider
    Simple horizontal or vertical divider.
    
    TauEdit
    Edit box with styled border.
    
    TauCheck
    Nice checkbox. Can have any size. Checking and unchecking is animated, also mouse movement is animated.
    
    TauIndex
    Vertical control to index lines.
    
    TauList
    This control can be used like ListBox. Contains also TauIndex, no scrollbar.
    
    TauTickControl
    Control like TrackBar. This slider can be vertical, horizontal and contains many styles to have different looks.
    
    TauTitle
    Simple text with interesting effect applied.
    
    TauVU
    VU level indicatior.


**Usage**

First, there is need to call initialization function for each used control. Then call CreateWindow and use class: [TauControlName]_CLASS. You can also specify additional style if there is any. Call SendMessage to set font and color. Catch WM_COMMAND as usual.
