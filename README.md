# calculator

Calculator written using C and without bitwise operators and integer arrays.

Tested on:

RaspberryPi 4b aarch64

Debian GNU/Linux 12 (bookworm)

X.Org X Server 1.21.1.7

X Protocol Version 11, Revision 0

  - X11 is not used in most linux distributions by default and might be discontinued.

Operating system workflow:

1. Install debian or ubuntu server command line (headless - no GUI) from the official imager.

2. Install X11.

Read about the computer:

- https://www.raspberrypi.com/documentation/computers/os.html

- Google: 'university machine organization and assembly class'

- Learn 'C' syntax

Usage (after downloading the folder):

```
gcc main.c ./src/*.c -o calculator200 && ./calculator200
```

### Todo:

- Math

  - Binary division.

  - Screen size 1.) arithmatic buttons i.e. 'sqrt()', exponents, etc.

  - Division -> powers of 2 to reduce iterations and theres another algorithm (Newton-Raphson Division) for larger numbers.

  - Include negative calculations and subtraction for '0'.

    - There are decimals randomly in the char array instead of binary.

  - Double check if binary arithmatic can increase from 200 -> 256 otherwise cap at 128. Otherwise arithmatic calculations are from 0-200.

    - Larger numbers are reprsented by small numbers in an array using a maximum size i.e. 256 or 128 and is called arbitrary-precision arithmatic or 'bignum arithmatic'.

    - Internet also calls this 'Horner's method' and uses base-200 for the example:

```
//             250,678 / 200 = 1,253 remainder (%) = 78  (200^0)
//                             1,253 / 200 = 6 (%) = 53  (200^1)
//                                 6 < 200         = 6   (200^2)
//
//                                                 = [ 6, 53, 78] < - the 'char[]' to store the number
//           - Reconstruct:
//                                                   6 X (200^2) = 6  X 40,000 = 240,000
//                                                  53 X (200^1) = 53 X 200    = 10,600
//                                                  78 X (200^0) = 78 X 1      = 78
//                                                                             = 250,678
//
//           - Since division using 200 is a nested loop with longer time complexity, divide by 256 (2^8)
//             where the division step is O(n) since it's a power of two
//
```

- GUI issues

  - Text buttons - 'screenCheckZero' multiple characters don't resize with the window.

  - Remove space button and off button mouse click repeating characters

  - Pressing 'Enter' after the '0' and '1' character elements without '=' or '-' resets the character array.

  - initial screensize '0' and 'eventCode == 12' buttons are off by several coordinates

  - minor todo

    - continuous mouse coordiantes based on event to verify border values.

    - 'errorCode == 12' succeeds within 1 or 2. Should probably use a 'iteration' counter.

    - Google AI suggested this method to reduce memory but would probably crash now since theres several 'write()':

      - Allocate a single large unsigned char buffer, append all the opcodes back-to-back (ensuring each individual opcode is padded to a multiple of 4 bytes), and send them all out using one single write() call. The X11 server will parse them sequentially out of the TCP/Unix stream automatically and is supposedly faster.

- GUI features

  - Change the non-decimal button color and '=' button color

  - Text input window.

    - larger display/history

      - The character goes over the text window to the left and repeats writing over itself and after deleting there is text on the left of the window.

    - history window (temporarily extend text input window)

      - After calculation, 'del' removes the entire string -> move to the history window

      - Starting '0' is removed when the screen resizes and the text is drawn over itself in a small area.

  - Replace button polygons with polylines.

    - Rounded corners that increase in size with use to mimic the passage of time.

  - Disable window manager and get the device's screen dimensions.

    - Make an 'x' and minimize button to close the program.

  - Click text input window to insert number - the coordinates of the text, the 'keyInputIterator' value for the 'charKeyInput[]'.

### Index

- *Binary arithmatic*

- *Text user interface*

- *Graphical user interface (GUI)*

### Features:

- *Binary arithmatic*

  - Depends on built-in C headers:

        ```
        #include <stdio.h>
        #include <string.h>
        #include <stdlib.h>
        ```

  - Mostly uses 'char[]' arrays (1 byte) instead of 'int' arrays (4 bytes).

    - I was learning C and didn't understand heap vs stack memory so there are a lot of 'malloc()' uses. I'm guessing the fastest is one giant C file without functions.

    - Decimal limit is ~200 without a loop and is similar to computer hardware while avoiding electrical fires.

  - ```./src/arithmatic.c```

    - Binary addition and utility functions for subtraction and multiplication.

  - ```./src/binaryChar.c```

    - Converts decimal 'int' to binary 'int' and back to decimal for the interfaces.

  - ```./src/arithmaticSteps.c```

    - Runs binary addition, subtraction, and multiplication (Todo - division).

    - 'window.c' utilizes an abbreviated division that rounds numbers (GUI section).

- *Text user interface*

    ```

    Enter two numbers less than 200
    with arithmatic operators
    +  ,  -  ,  *  ,  /  ,  **

    Example: 1 + 1 (press enter):

    _____________________________

    ```

  - ```./src/userInput.c```

    - Formats user input and writes to a '.log' file.

    - Used in "interface.c" 'textGUI()'.

  - ```./src/interface.c```

    - Runs the functions from 'userInput.c'.

- *Graphical user interface (GUI)*

  - Works on Linux X11 server.

  - Written without the X11 library. Information from Google AI to avoid documentation.

  - 'write()' and 'read()' functions from '/sys' folder to make a window.

        ```
        // Set two structs and several variables
        // https://www.man7.org/linux/man-pages/man7/unix.7.html
        // used in "server.c" and "client.c" for window.
        #include <sys/socket.h>
        #include <sys/un.h>
        // ./usr/include/unistd.h
        #include <unistd.h>
        ```

  - ```./src/window.c```

    - After connecting to the server, the packets are specified in several thousand lines 'drawWindow()'.

    - 'char[]' array elements have an integer limit of 255.

      - The workaround for X11 Server packets is 2-4 low/high packets to represent the entire range of C integers for 64 bit OS (~ 4 billion). For example:

            ```
            Little-endian (no spell check joke) byte-ordering allows larger
            numbers with four 1-byte elements representing a 32 bit integer.

             char  example[32];

                   example  [28] 128               =  128     +
                   example  [29] 62 ( * 256)       =  15872   +
                   example  [30] 0  ( * 65536)     =  ( 0 ) ) +
                   example  [31] 0  ( * 16777216)  =  ( 0 ) )

            ```

      - The main challenge is a continuous 'while' loop that waits for feedback from the server.

        - There are three long code duplicates for button drawing that would be more readable with functions. However, functions were avoided to save heap memory:

          - The initial window before the continuous 'while' feedback.

          - 'eventCode==12' (window has to be redrawn including the buttons).

          - 'eventCode==22' (window is resizing).

  - Three screen sizes that mimic smartphone, tablet, desktop device orientation.

  - Resizes buttons using powers of 2 division to reduce nested loops.

  - Since the desktop view (dubbed 2.)) is undecided, the obvious answer is to make a map.

    - Requires polylines to draw the features.

    - Would have to download CSV coordinates for basic features.

      - GeoJSON file parser.

      - Reverse engineer the JPG and shapefiles for the basemap.

      - X11 is not used in most linux by default anymore and might be discontinued.

      - 'Google Maps v 8billion' should probably a different project.

  - The button layout schematics are from the './src/window.c' file:

    ```
      // To resize with the window size, use the dimensions of the window to change the button dimension
      // values in step 6 to redraw the butons. The most efficient method is having three predesignated layouts
      // and resize. The default window dimensions are below.
      //
      //  s = screen text input -> opcode 76 (imageText8) write text -> opcode 45 (openFont) resize -> fontid
      //     - text box accepts things like 'sqrt()' and other common inputs
      //
      //   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      //  {                              }                                 20  <> = switch buttons
      //   ______________                }  0.) 4 X 5 = 20     1.) 8 X 5 = 40   o = scientific
      //   -----         ]               }     -------------      _____________________________________
      //  |     |        ]               }    | s s s s s s |    [                                      ]
      //  |     |        ]               }    | s s s s s s |    [ s s s s s s s s s s s s s s s s s s  ]
      //  | 0.) |        ]               }    |             |    [ s s s s s s s s s s s s s s s s s s  ]
      //   -----         ]               }    |() del %  /  |    [ s s s s s s s s s s s s s s s s s s  ]
      //  [              ]               }    | 7  8  9  *  |    [                                      ]
      //  [              ]               }    | 4  5  6  -  |    [ <>  rad  sqrt  |x|  ()  del   %    / ]
      //  [      1.)     ]               }    | 1  2  3  +  |    [                                      ]
      //  [ ____________ ]               }    | c  0  .  =  |    [ sin  cos  tan  pi   7    8    9    * ]
      //  {                              }     -------------     [                                      ]
      //  {                              }                       [ ln   log  1/x  e    4    5    6    - ]
      //  {                              }                       [                                      ]
      //  {                    todo 2.)  }                       [ e^2  x^2  x^x  +/-  1    2    3    + ]
      //   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                        [                                      ]
      //                                                         [ o    o    o    o    c    0    .    = ]
      //                                                         [ ____________________________________ ]
      //
    ```

- Screensize 0.)

![Screensize zero](./ScreenSizeZero.png)

- Screensize 1.)

![Screensize one](./ScreenSizeOne-1.png)

  - Button input explanation 'eventCode == 4' and 'eventCode == 6'.

    ```
      // char limits int to 255 and this method overflows into two char elements which uses multiplication to achieve
      // larger numbers (i.e. the mouse click input responseWindowInput[24] and [25]). To find the bottom border for the
      // button, the larger input '[25]' is compared using greater than or equal '>=' to the previously calculated array
      // 'buttonBorder[j][13]' that contains the larger bottom 'x' coordinate border. The larger number is the second
      // element in the two char array elements used to represent larger int numbers and are refered to as 'x2' or 'y2' in
      // contrast with the first element 'x1' or 'y1'. The equation is ((x1 * 256^0) + (x2 * 256^1)...etc). If the 'x2' input
      // (responseWindowInput[25]) is '>=' to the comparison (buttonBorder[j][13]), 'x2LowCheck' passes and proceeds to the
      // next check for 'x1LowCheck'.
      //
      // The 'x1' 'x2' 'low' and 'high' values are difficult to visualize and a diagram is included:
      //
      //          y1y2 low
      //          ________
      //     x1  |        |  x1
      //     x2  |        |  x2
      //    low  |        |  high
      //         |________|
      //          y1y2 high
      //
      // Summary: -'x1x2 low' represents one number.
      //
      //      x1   = 55    =   ( 55  X  256^0 )       x1 = 55
      //      x2   = 0     =   ( 0   X  256^1 )       x2 =  0
      //                                               x =  55    ( 55 + 0 )
      //
      //      y1   = 100   =   ( 100 X  256^0 )       y1 =   100
      //      y2   = 1     =   ( 1   X  256^1 )       y2 =   256
      //                                               y =   356   ( 100 + 256 )
      //
      //          - The standard graph (not flipped) with a point.
      //
      //  400   _|
      //  300   _|     * (x,y)
      //  200   _|
      //  100   _|
      //    0   _|___________
      //         |    |    |
      //        0    50   100
      //
      //          - Coordinates are an upside down graph for y and usual left to right x direction.
      //
      //        0    50   100
      //    0   _|____|____|__
      //  100   _|
      //  200   _|
      //  300   _|
      //  400   _|     * (x,y)
      //
      //         - The coordinates represent a polyline boundary.
      //
      //        0    50   100
      //    0   _|____|____|__
      //  100   _|     |
      //  200   _|     | x = 55
      //  300   _|     |
      //  400   _| ----|-------
      //             y = 356


      Example conditional (there are (4 X 2) for each side of the button):


        // x2 low boundary
        if (responseWindowInput[25] >= buttonBorderOne[j][13]) {
          x2LowCheck = 1;

          // The next comparison splits based on if the larger 'x2' number is '==' or '>'. The first 'if' has another
          // condition for the 'x1' value which asks if the mouse click input 'responseWindowInput[24]' is '>=' the
          // 'buttonBorder[j][12]' 'x1' border. If both those conditions are true, 'x1Lowcheck' is set to '1'. If they're
          // not, the second split asks if only the 'x2' is '>' 'buttonBorder[j][13]' and sets 'x1LowCheck' to '1'. Otherwise
          // 'x1LowCheck' remains '0' and a 'break;' would probably work to reduce further iterations.

          if (responseWindowInput[25] == buttonBorderOne[j][13] && responseWindowInput[24] >= buttonBorderOne[j][12]) {
            x1LowCheck = 1;
          }
          else if (responseWindowInput[25] > buttonBorderOne[j][13]) {
            x1LowCheck = 1;
          }
        }


      //
      // x low  = 55
      // x high = 105
      // y low  = 356
      // y high = 506
      //     +  = button clickable area
      //
      //        0    50   100   ...n
      //    0   _|_____|_____|_______
      //  100   _|      |    |
      //  200   _| xlow |    | x high
      //  300   _|      |    |
      //  400   _| -----|----|----
      //         |      |++++| y low
      //         |      |++++|
      //  ...n   | -----|----|----
      //                 y high
      //
      // Clicking the button returns '0-9' or the other calculator inputs.
    ```

- Data structures and algorithms 'window.c'.

    ```
      // Intermediate screen size 1.) implementation:
      //
      // Change button background when hovered 'eventCode == 6'.
      // Decreased button redrawing from all 40 to +-8 and improved user experience.
      // Three character arrays of different sizes:
      //     Dimensions of the previous button  -  buttonTextOne[40]
      //                         words > 1 char  -  charMultiButtonTextOne[84]
      //                         words = 1 char  -  charButtonTextOne[40]
      //
      // Worst case time complexity O(n) - size of the button label
      // array 'charMultiButtonTextOne[84]' using the coordinates from 'buttonTextOne[40]'.
      // The challenge was finding the first character in a multiple character word and
      // appears in '2.2'. Array 'charMultiButtonTextOne' doesn't have a space to start
      // the array so the button count was behind by one. Also the first character coordinates
      // are calculated using an offset from the length of the word (2, 3, 4) for accurate spacing.
      //
      // Step 0 - Calculates starting button 'k' and ending button 'm'
      //          using 'previousJ' +- 'buttonColumns (8).
      //          Button number 'j' is used to reset buttons to the
      //          hovered background color.
      //
      // Step 1 j > 0
      // Change hovered button background color darker, same border
      // color, write button labels
      //
      // Step 2
      // Redraw +-8 buttons directly after the mouse moves off the button.
      // Order of appearance in the loop: 2.0  2.5  2.2  2.4  2.3  2.1
      // Step 2.0
      // End of the multiple character loop when 'ii' equals 'm' or the end
      // of the 'charMultiButtonTextOne[]'.
      // Step 2.1
      // Counts the multiple char buttons and single char buttons.
      // Step 2.2 (Appears first but the conditional is met after 'Step 2.1').
      // Uses the starting button 'k'. 'iii' is a check that
      // indicates 'i' has been set. Starting char 'i' based on incrementing
      // char 'n' when incrementing button 'ii' = starting button 'k'.
      // Step 2.3
      // Length of the button word and first char 'write()'.
      // todo remove the first button since that button 'k-8' doesn't have a length.
      // Step 2.4
      // 'write()' characters from 'charMultiButtonTextOne[n]' '.1,2,...n' 
      // and skip the '0' char since it's written in 'Step 2.2'.
      // todo Step 2.5 <- doesnt make sense since this loop is to
      // reduce lage time between 'write()' the entire 40 buttons.
      // Remove the conditional to 'write()' the darker button
      // background color with a single loop.
      //
      // Step 3
      // Redraw all buttons when not hovered.
    ```

- There are more details in "./src/window.c" at the 'drawWindow()' function.
