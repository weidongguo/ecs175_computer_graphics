Project 2 
1. Name:      Weidong Guo
   Username:  weidongg
   Email:     wdguo@ucdavis.edu
  
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

   b. more on datafile:
      The 1st line of the datafile contains the dimension of the openGl window(so called "viewport on screen" as mentioned in assignemt sheet) 
      and the rest of lines specify the polyhedra

      For example,

      800 600                          dimension of the opengl window

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

   To change these parameter, user can again right click within the *main window*, and then select "Grab Input". A submenu will pop up and have the
   user to choose from getting input for rotation, scale, or translation.
   If anyone of those is selected, there will be a prompt displayed in the console asking for input. There is also a format description and an
   example input next to the prompt to give an idea what is the appropriate format of the input.
   User can enter the input from the console. 
    
   For example,
  
   Please enter the translation factor x: 1
   Please enter the translation factor y: 2
   Please enter the translation factor z: 3
   Translation factors (1.00, 2.00, 3.00) recorded!

   Another example of entering inputs when doing a right-click -> Grab Input -> Rotation Axis & Angle
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


5. With the cursor on the opengl's *main window*, user can select a polyhedron by typing the key 0, 1, 2, 3, ... , 9  
   The id is defined in the order that they were specified in the datafile
   The first object has id 0; the second object has id 1 and so on. (Currently, it only supports up to 9 objects);
   So any transformation done will only affect the currently selected object

6. With the cursor on the opengl's *main window*,
   To rotate, user can press the key 'r'    (r for rotate)
   To translate, user can press the key 't' (t for translate)
   To scale,  user can press the key 'z'    (z for zoom)
   All of the above commands(operations) are executed using the parameter shown in status 
   
   To save the polygons currently being displayed to a output file,  user can press the key 's' (s for save)
   The output file will be named "output"

   Please note that the rotational axis will not be saved to the output file
