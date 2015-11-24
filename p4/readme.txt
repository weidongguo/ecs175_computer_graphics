Project 4 
1. Name:      Weidong Guo
   Username:  weidongg
   Email:     wdguo@ucdavis.edu

   de Casteljau:           bezier.cpp:17-48
   de Boor                 bspline.cpp:29-73
   modify a control point  curve.cpp:141-149  
   insert a control point  curve.cpp:155-162  
   add    a control point  curve.cpp:164-171
   delete a control point  curve.cpp:14-23
   
   please note that both Bezier and Bspline is a subclass of Curve

2. To compile the source code, please run the command "make" since I have a makefile ready
3. To execute the program, please type a.out
   a. You will then be prompted to enter a input file
      For exampke, 

      [weidongg@pc55 p2]$ a.out
      Please enter name of the input file: notTheRightFile
      Error: file not found, please try again
      Please enter name of the input file: datafile
      Successfully loaded file
      
      After succesfully loading the file, the window will be created and render the curves read from the datafile
      
      You can find sample input datafiles in the same directory; they are named datafile, datafile2;


4. If user right click within the openGl window, a menu will pop up. User can click on "status" to see the default resolution
    
   To increase the resolution, the user can press '='
   To decrease the resolution, the user can press '-'
   The above two operations are done on all curves 
   
   To select a curve, the user can use the 'h' key or the 'l' key to move left or right. The selected curve will have its 
   control polygons highlighted with green
   -->>All operations below are done on the ****selected curve only***:   
   1) To select a control point, user just move the cursor of the mouse to the control point and then do a left-click. 
   2) A control point with green color is the selected control point. To modify this control point, user can simply
   move the cursor with his mouse, and click on the MIDDLE BUTTON of the mouse at a location where this control point will be 
   changed to. 
   
   3) To delete the selected control point, user can press 'd'
   4) To add(putting a control point to the left of the selected control point) a control point, user can move the cursor again 
   with his mouse, and press 'i' at a location where the new point will be.

   5) To insert(putting a control point to the right of the selected control point) a control point, user can move the cursor 
   again with his mouse, and press the key 'a' at a location where the new point will be.

   6) To get input for the K-value and the Knot Values, user can do a right click within the opengl window, a user menu will 
      pop up. User can select input->B-Spline Param. A prompt for entering values will be shown in the terminal.

  7) To save curves to a file called "output", press 's'

5. Please note that I draw a grey square(bigger than a pixel) at each control point to aid the user for selecting a control point with the mouse. And Bezier curves' control polygons are drawn with blue(Royal Blue) and Bspline curves' control polygons are draw with red(Crimson);
