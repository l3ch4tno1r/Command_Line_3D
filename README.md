# Console3D

This is an attempt to display 3D graphics into command prompt console.

This project is inspired by a youtuber known as OneLoneCoder / javidx9 (https://www.youtube.com/channel/UC-yuWVUplUJZvieEligKBkA) who made a series of videos on this topic.
  - Link to the first video : https://www.youtube.com/watch?v=ih20l3pJoeU&list=PLrOv9FMX8xJE8NgepZR1etrsU63fDDGxO&index=23&t=6s

This project has been set up under Visual Studio Community 2017.

Be sure to set the "Console3D" project as the default project.

I recommend to use these parameters for the console window in order to have a correct display of the 3D scenes
(those parameters can be found by clicking the top left icon of the window, and under "Properties")
  - Window size :
    > Width  = 180
    > Height = 120
  - Font :
    > Polices Raster
    > Size = 8x8
    
Controls (ATTENTION : hard coded)
  - Z = go forward
  - Q = strafe left
  - S = go backward
  - D = strafe right
  - Mouse movement = camera control
  - P = pauses / resumes the simulation. It releases the mouse cursor so one can interact with Windows.
  - ESC = end the simulation
