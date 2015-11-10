Project 3 
1. Name:      Weidong Guo
   Username:  weidongg
   Email:     wdguo@ucdavis.edu

     Phong Lighting Model     polyhedron.cpp line 352 - 447
     Gouraud Shading          polyhedron.cpp line 618 - 727 (edge points)
                              graph.cpp      line 35 - 66 (scanline)
     Half-Toning              graph.cpp      line 330 - 426
     Painter's Algorithm      polyhedron.cpp line 468 - 539

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
      
      ** Please note that I use 3 subwindows for the xy-plane(top-left), xz-plane(top-right), and the yz-plane(bottom-left)
      The bottom-right is the *main window* 
      
      You can find sample input datafiles in the same directory; they are named datafile, datafile2;


4. If user right click within the openGl window, a menu will pop up. User can click on "status" to see the default parameters for half-toning,
   and phong lighting model
   
   For example, 

   ======================Status====================
   Window Size is 900 x 600
   Half Tone Color R: 0.86, G: 0.08 B: 0.24
   ** Phong Model Parameter **
   Light Source Intensity Il: 0.70
   Ambient Light Intensity Ia: 0.10
   From Point ff: (0.00, 0.00, 10.00)
   Light Source xx: (10.00, 10.00, 10.00)
   Phong Constant n: 9
   Ambient Coeff ka, R: 0.30, G: 0.30 B: 0.30
   Diffuse Coeff kd, R: 1.00, G: 0.00 B: 0.00
   Specular Coeff ks, R: 0.80, G: 1.00 B: 1.00
   =================End of Status==================


5. For changing half-toning color, user can right click within opengl's window, then select Input -> Set Half Tone Color
   After entering the parameter, my program will not update the screen. In order to see the effect,
   user can select Half Tone -> Toggle.  Toggle the first time is to change to half-tone mode. Toggle the 2nd time is to 
   go back to the normal mode (**simply restore the previous pixel buffer from the PixelBufferBackup**) 

   For modifying the phong lighting model parameters, one can select Input -> Set Phong Model Parameters
   After entering the parameters, the view will be updated automatically.

