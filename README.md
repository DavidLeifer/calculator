# calculator

Calculator written using C and without bitwise operators and integer arrays.

Tested on:

RaspberryPi 4b aarch64
Debian GNU/Linux 12 (bookworm)
X.Org X Server 1.21.1.7
X Protocol Version 11, Revision 0

Operating system workflow:

1. Install debian or ubuntu server command line (no GUI) from the official imager.

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

- Resized buttons.

- Text buttons.

- Text input window.

- Binary division.

- Replace button polygons with polylines.

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
    with arithmatic operators\n +  ,  -  ,  *  ,  /  ,  **

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

      - The main challenge is a continuous 'while' loop that waits for feedback from the window.

        - There are three long code duplicates for button drawing that would be more readable with functions. However, functions were avoided to save heap memory:

          - The initial window before the continuous 'while' feedback.

          - 'eventCode==12' (window has to be redrawn including the buttons).

          - 'eventCode==22' (window is resizing).

  - Three screen sizes that mimic iPhone, iPad, desktop device orientation.

  - Since the desktop view is undecided, the obvious answer is to make a map for the desktop view '2.)'.

    - Requires polylines to draw the features.

    - Would have to download CSV coordinates.

      - GeoJSON file parser.

      - Reverse engineer the JPG and shapefiles.

      - X11 is not used in most linux by default and might be discontinued and 'Google Maps v 8billion' should probably a different project.

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
      //   -----         ]               }    | c  x  %  /  |    [ s s s s s s s s s s s s s s s s s s  ]
      //  [              ]               }    | 7  8  9  *  |    [                                      ]
      //  [              ]               }    | 4  5  6  -  |    [ <>  rad  sqrt  |x|  c    X    %    / ]
      //  [      1.)     ]               }    | 1  2  3  +  |    [                                      ]
      //  [ ____________ ]               }    | () 0 . =    |    [ sin  cos  tan  pi   7    8    9    * ]
      //  {                              }     -------------     [                                      ]
      //  {                              }                       [ ln   log  1/x  e    4    5    6    - ]
      //  {                              }                       [                                      ]
      //  {                         2.)  }                       [ e^2  x^2  x^x  +/-  1    2    3    + ]
      //   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~                        [                                      ]
      //                                                         [ o    o    o    o    ()   0    .    = ]
      //                                                         [ ____________________________________ ]
      //
    ```

  - Button input explanation.

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






