Project 2 
1. Name:      Weidong Guo
   Username:  weidongg
   Email:     wdguo@ucdavis.edu

   3D Translation                                           polyhedron.cpp line 247 - 259
   3D Rotation About an Arbitray Axis                       polyhedron.cpp line 261 - 311
   3D Scalling                                              polyhedron.cpp line 225 - 246
   Orthnographic projections(xy-plane, xz-plane, yz-plane)  polyhedron.cpp line 56  - 77
   NDC (Normalized Device Cordinates)                       polyhedron.cpp line 151 - 220

2. To compile the source code, please run the command "make" since I have a makefile ready
3. To execute the program, please type a.out
   a. You will then be prompted to enter a input file
      For exampke, 

      [weidongg@pc55 p2]$ a.out
      Please enter name of the input file: notTheRightFile
      Error: file not found, please try again
      Please enter name of the input file: datafile
      Successfully loaded file
      
      After succesfully loading the file, the window will be created and render the polyhedra read from the datafile
      Please note that I use 3 subwindows for the xy-plane(top-left), xz-plane(top-right), and the yz-plane(bottom-left)
      The bottom-right is the *main window* 
      
      NOTE: the RED rotational axis is always being displayed.

   b. more on datafile:

      For example,
      2                                number of polyhedra(objects)

      4                                4 points (for the 1st object)
      0.0 0.0 0.0             
      1.0 0.0 0.0
      0.0 1.0 0.0
      0.0 0.0 1.0
      6                                6 edges (for the 1st object)
      1 2
      1 3
      1 4
      2 3
      2 4
      3 4

      3                                3 points (for the 2st object)
      2.0 1.0 0 
      1.0 0 2.0
      2.0 2.0 1.0
      3                                3 edges (for the 2nd object); 
      1 2
      2 3
      3 1

      You can find sample input datafiles in the same directory; they are named datafile, datafile2, datafile3, datafile4, and datafile5

4. If user right click within the openGl window, a menu will pop up. User can click on "status" to see the default parameters for rotation,
   translation, and scaling. The output of the status will be directed to the console 
   
   For example, 
   ======================Status====================
   Window Size is 800 x 600
   Rotation Axis (0.00, 0.00, 0.00) (1.00, 1.00, 1.00) Angle: 3.00
   Scale Factor: Alpha = 1.20
   Translation Factor: x = 3.00, y = 3.00, z = 3.00
   =================End of Status==================

   To change these parameter, user can again right click within the opengl's window, and then select "Grab Input". A submenu will pop up and have the
   user to choose from getting input for rotation, scale, or translation.
   If anyone of those is selected, there will be a prompt displayed in the console asking for input. There is also a format description and an
   example input next to the prompt to give an idea what is the appropriate format of the input.
   If user do a right-click within the opengl's window, then hover the mouse at Grab Input, then click on Translation Factors,
   a prompt will be displayed at the console.
    
   For example,
  
   Please enter the translation factor x: 1
   Please enter the translation factor y: 2
   Please enter the translation factor z: 3
   Translation factors (1.00, 2.00, 3.00) recorded!

   
   Example of entering inputs when doing a right-click -> Grab Input -> Rotation Axis & Angle
   There will be a prompt at the console.

   Please enter the two points defining the rotation axis
   Point 1's x: -3
   Point 1's y: -2
   Point 1's z: 1
   Point 2's x: 4
   Point 2's y: 10
   Point 2's z: -4
   Please enter the rotation angle ( format <float> ; e.g. 6.5):
   -3.3
   Rotation Axis (-3.00, -2.00, 1.00) (4.00, 10.00, -4.00) Angle: -3.30 saved

   
   Example of entering inputs hwen doing a right-click -> Grab Input -> Scaling Factors
   
   Please enter the scale factors ( format <float> ; e.g. 1.2 ):
   0.8
   Scale factor 0.80 recorded!

5. With the opengl's window being selected,  user can select a polyhedron by typing the key 0, 1, 2, 3, ... , 9  
   The id is defined in the order that they were specified in the datafile
   The first object has id 0; the second object has id 1 and so on. (Currently, it only supports up to 9 objects);
   So any transformation done will only affect the currently selected object
   When the object is selected, its attributes will be printed to the console ( e.g. the vertex coordinates and edges).

6. With the opengl's window being selected, 
   To rotate, user can press the key 'r'    (r for rotate)
   To translate, user can press the key 't' (t for translate)
   To scale,  user can press the key 'z'    (z for zoom)
   All of the above commands(operations) are executed using the parameter shown in status 
   
   To save the polygons currently being displayed to a output file,  user can press the key 's' (s for save)
   The output file will be named "output"

   Please note that the rotational axis will not be saved to the output file
