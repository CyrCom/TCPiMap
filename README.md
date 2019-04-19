# TCPiMap

* [Video Presentation](https://www.youtube.com/watch?v=aB37wf6E8p4)

## Based on
* [openFrameworks, C++ Framework](https://github.com/openframeworks/openFrameworks)
* [ofxOMXPlayer, OpenMax Video Player](https://github.com/jvcleave/ofxOMXPlayer)
* [ofxGLWarper, Quad Warping Addon](https://github.com/CyrCom/ofxGLWarper)
* [CTL, a Pure-Data cue list](https://www.chnry.net/ch/spip.php?article291)

This is a video mapper running on Raspberry Pi, and controlled over network from a distant computer.
An almost unlimited number of Raspberry Pis can be controlled at the same time, and everything can be scripted.

Each Pi is attached to a projector with a small hdmi cable, and wired to the network, or connected to a WiFi router.
The medias are hosted on the Pi and can be transfered via network.

* Each be controlled, automated and scripted:
  * Purr-data patch to make all kind of **cue scripting** with very powerful features

* Videos are loaded directly on sd-cards:
  * No video cables required all over the place, only Ethernet cables (or WiFi).
  * autonomous playing (5w consumption) for installations purposes

	
### TCP available commands:
    
    move_corner
    	[1-4] x y
    move_warper
    	x y
    set_corner
    	[1-4] x y
    set_warper_pos
    	x y (position of TOP LEFT corner)
    set_all_corners
    	x1 y1 x2 y2 x3 y3 x4 y4 (reading order)
    setDisplayRect
    	<x> <y> <width> <height>
    get_all_corners

    load_video
    	<int>(pathNumber) <optional: paused>(default: playing)
    create_surface
    	<opt:direct>
    delete_surface
    select_surface
		 <positive int>
    select_corner
    	<1-4> (reading order)
    deselect_corner

    enableLooping(PLAYER)
		<true/false> or <0/1>
    pause
		<true/false> or <0/1>
    set_volume
		<0<float<=1>
    step_frame
		<int>
    seek_to
		<int:time in sec>

    bShowWarpers
		<true/false> or <0/1>
    bShowTestScreen
		<true/false> or <0/1>
    bInvertColors
		<true/false> or <0/1>
    bShowDebugText
		<true/false> or <0/1>

    adapt_warper_to_media
    adapt_font_to_warper
    set_alpha_testScreen
		<0-255>
    warpers_colors
		'attribute' R G B opt:A (0-255)
		 attribute: basic_corn basic_rect selsurf_corn selsurf_rect

    settings
		<true/false>(enableLooping def:false) <true/false>(enableFilters def:false)
    audio_settings
		<true/false>(enable) <opt:0 to 1>(initialVol def:0.5) <opt:true/false>(HDMIsnd def:true)
    you_are
		<name> (will be written in xml file on Rpi)
    who_is_there
    list_files
    exit
    reset
    disconnect
    info
	
you can use '+' to send mutliple commands in the same packet (more efficient).