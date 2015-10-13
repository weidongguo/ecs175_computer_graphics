1. Name:      Weidong Guo
   Username:  weidongg
   Email:     wdguo@ucdavis.edu
   
   Important Note: My origin is at the center of the openGl window. If the dimension is 1000x500, then x can take value from -500 to 500
                   and y can take value from -250 to 250. If the objects are drawn out side of the bounds, it might get a seg fault.
                   Similarly, if user scale, translate, or rotate in away that the x or y value is out of bound, it might get a seg fault or weird
                   behavior will be observed

2. To compile the source code, please run the command "make" since I have a makefile ready
3. To execute the program, please type a.out < datafile
   a. datafile is the file that will be redirected to stdin 
   b. a.out is the executable for the program
   
   c. more on datafile:
   The 1st line of the datafile contains the dimension of the openGl window(so called "viewport on screen" as mentioned in assignemt sheet) 
   and the rest of lines specify
   the points of the polygons. In other words, other than the 1st line of the datafile, the rest follows the same format as the one
   introduced in the project 1's assignment sheet.
   
   For example,
   1000 500         dimension of the opengl window
   2                number of polygons

   4                number of points for the 1st polygon
   0 0              coordinate of 1st point
   0 100            coordinate of 2nd point
   100 100          coordinate of 3rd point
   100 0            coordinate of 4th point

   3                number of points for the 2nd polygon
   -100 -100        coordinate of 1st point
   -200 -200        coordinate of 2nd point
   -190 -150        coordinate of 3rd point

4. The program starts up by drawing and rasterizing all the polygons specified in the datafile 
5. If user right click within the openGl window, a menu will pop up. User can click on "status" to see the default parameters for rotation,
   translation, scaling, and clipping. The output of the status will be directed to the console (note that console will only act as 2nd display in 
   my program)
   For example, 
   ======================Status====================
   Window Size is 1000 x 500
   Clipping Region: xMin = -200, xMax = 200, yMin = -200, yMax = 200
   Rotation Angle: 5.00
   Scale Factor: Alpha = 1.20,  Beta = 1.20
   Translation Factor: x = 20, y = 20
   =================End of Status==================

   To change these parameter, user can again right click within the openGl window, and then select "Grab Input". A submenu will pop up and have the
   user to choose from getting input for rotation, scale, translation, or clip region.
   If anyone of those is selected, there will be a prompt displayed in the console asking for input. There is also a format description and an
   example input next to the prompt to give an indea to the user about what is the appropriate format of the input.
   (Note, it currently doesn't support backspace. The program is expecting valid input)
   Once the valid input is typed out, press ENTER to proceed.
   
   For example, 
   Please enter the scale factors ( format <float><space><float> ; e.g. 1.2 1.2 ):      <- prompt
   0.8 0.7                                                                              <- what user typed 
   Data Recorded!                                                                       <- 1st message displayed aftering pressing ENTER 
   Back to Command Mode.                                                                <- 2nd message displayed aftering pressing ENTER

6. In Command Mode, to select a polygon, user can type the key 0, 1, 2, 3, ... , 9.
   The id is defined in the order that they were specified in the datafile
   The first polygon has id 0; the second polygon has id 1 and so on. (Currently, it only supports up to 9 polygons);
   So any operation done will only affect the selected polygon

7. In Command Mode, to rotate, user can press the key 'r'    (r for rotate)
   To translate, user can press the key 't' (t for translate)
   To scale,  user can press the key 'z'    (z for zoom)
   To clip, user can press the key 'c'      (c for clip)
   All of the above commands(operations) are executed using the parameter shown in status 
   
   To save the polygons currently being displayed to a output datafile,  user can press the key 's' (s for save)
   The output file will be named "output"


