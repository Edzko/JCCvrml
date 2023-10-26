# Jumbo Container Crane VRML Simulator #

This project creates an interactive simulation of a Gantry Container Port Crane. Any Windows compatible game controller can be used to move the trolley on the crane and to lift the container.

This simulator was part of a research project:


- [Automatic Skew Control on Container Transhipment Cranes](https://doi.org/10.1016/S1474-6670(17)39272-8 "IFAC Paper"). G.E.Smid, J.B.Klaassens, H.R.van Nauta Lemke, A.El Azzouzi, R.van der Wekken. *IFAC Conference on Mechatronic Systems*, Darmstadt, Germany, 18-20 September 2000
Volume 33, Issue 26,
Pages 1-1129 (September 2000)
- [3D modeling visualization for studying controls of the jumbo container crane](https://ieeexplore.ieee.org/document/786141 "ACC 1999"). J.B. Klaassens, G. Honderd, A. El Azzouzi, Ka C. Cheok, G.E. Smid. Proceedings of the *1999 American Control Conference* (Cat. No. 99CH36251) ISBN: 0-7803-4990-3.

 

## Prerequisites ##

This project requires the [Cortona VRML plugin](http://www.cortona3d.com/en/cortona3d-viewer-download "Cortona plugin download") with the [Cortona SDK](http://download.cortona3d.com/238/edb2b7ad7a30b72d1c2ba364c161ea81/Cortona_SDK_41.zip "Cortona SDK 4.1"). 
It is built with Visual Studio 2017 with MFC extensions, and tested on Windows 10.

## User Guide ##

![](hlp/jccvrml_gui.png)

Although the initial view is from the side of the crane as seen in the above Figure, the primary perspective for the Crane Simulator is from the Crane Master's perspective - in the Trolley above the container, riding along on the boom. Any conventional game controller can be used to control the trolley motion and container hoisting.

The Simulator therefore supports multiple:

1. From the Side - Initial view
2. From the Top - Perspective from the top down
3. From the Front - Perspective from the Sea side
4. The Crane Master - As if in the seat on the controls of the trolley
5. Follow the Trolley - Side view from the trolley, riding along the boom.

The three primary harmonic motions that are introduced when moving the container are:

1. Swing - harmonic motion perpendicular to the boom direction
2. Sway - harmonic motion along the boom direction
3. Skew - rotational harmonic motion in the vertical direction

The compensation motions controllers can be activated via the game controller buttons.


## Contact ##

For any further inquiries, contact [Edzko Smid](mailto:Edzko.Smid@tecontrol.us "G. Edzko Smid, Ph.D.").