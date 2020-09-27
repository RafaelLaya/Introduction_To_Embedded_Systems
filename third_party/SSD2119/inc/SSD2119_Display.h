/*********************************************************************
 * SSD2119_Display.h
 * The header file of the SSD2119 LCD driver.
 ********************************************************************/

#pragma once

// exported constant
extern const unsigned short Color4[16];

// ************** LCD_Init ********************************
// - Initializes the LCD
// - Command sequence verbatim from original driver
// ********************************************************
void LCD_Init( void );

// ************** convertColor ****************************
// - Converts 8-8-8 RGB values into 5-6-5 RGB
// ********************************************************
unsigned short convertColor( unsigned char r,
                             unsigned char g,
                             unsigned char b );

// ************** LCD_ColorFill ***************************
// - Fills the screen with the color in the Color4[] array
// - or the return value of the convertColor() function.
// ********************************************************
void LCD_ColorFill( unsigned short color );

// ************** abs *************************************
// - Returns the absolute value of an integer
// ********************************************************
int abs( int a );

///////////////////////////////////////////////////////////
//                PRINTING FUNCTIONS                       
///////////////////////////////////////////////////////////

// ************** LCD_PrintChar ***************************
// - Prints a character to the screen
// ********************************************************
void LCD_PrintChar( unsigned char data );

// ************** LCD_PrintString *************************
// - Prints a string to the screen
// ********************************************************
void LCD_PrintString( char data[] );

// ************** LCD_SetCursor ***************************
// - Sets character printing cursor position
// ********************************************************
void LCD_SetCursor( unsigned short xPos,
                    unsigned short yPos );

// ************** LCD_Goto ********************************
// - Sets character printing cursor position in terms of 
//   character positions rather than pixels. 
// - Ignores invalid position requests.
// ********************************************************
void LCD_Goto( unsigned char x,
               unsigned char y );

// ************** LCD_SetTextColor ************************
// - Sets the color that characters will be printed in
// ********************************************************
void LCD_SetTextColor( unsigned char r,
                       unsigned char g,
                       unsigned char b );

// ************** printf **********************************
// - Supports:
//   - %d   Signed decimal integer
//   - %c   Character
//   - %s   String of characters
//   - %f   Decimal floating point    (Not Yet Implemented)
//   - %x   Unsigned hexadecimal integer
//   - %b   Binary integer
//   - %%   A single % output
// ********************************************************
void LCD_Printf( char fmt[], ... );

// ************** LCD_PrintInteger ************************
// - Prints a signed integer to the screen
// ********************************************************
void LCD_PrintInteger( long n );

// ************** LCD_PrintHex ****************************
// - Prints a number in hexidecimal format
// ********************************************************
void LCD_PrintHex( unsigned long n );

// ************** LCD_PrintBinary *************************
// - Prints a number in binary format
// ********************************************************
void LCD_PrintBinary( unsigned long n );

// ************** LCD_PrintFloat **************************
// - Prints a floating point number (doesn't work right now)
// ********************************************************
void LCD_PrintFloat( float num );

///////////////////////////////////////////////////////////
//               DRAWING FUNCTIONS                         
///////////////////////////////////////////////////////////

// ************** LCD_DrawPixel ***************************
// - Draws a 16-bit pixel on the screen
// ********************************************************
void LCD_DrawPixel( unsigned short x,
                    unsigned short y,
                    unsigned short color );

// ************** LCD_DrawPixelRGB ************************
// - Draws a 16-bit representation of a 24-bit color pixel
// ********************************************************
void LCD_DrawPixelRGB( unsigned short x,
                       unsigned short y,
                       unsigned char r,
                       unsigned char g,
                       unsigned char b );

// ************** LCD_DrawLine ****************************
// - Draws a line using the Bresenham line algrorithm from
//   http://rosettacode.org/wiki/Bitmap/Bresenham%27s_line_algorithm
// ********************************************************
void LCD_DrawLine( unsigned short startX,
                   unsigned short startY,
                   unsigned short endX,
                   unsigned short endY,
                   unsigned short color );

// ************** LCD_DrawRect ****************************
// - Draws a rectangle, top left corner at (x,y)
// ********************************************************
void LCD_DrawRect( unsigned short x,
                   unsigned short y,
                   short width,
                   short height,
                   unsigned short color );

// ************** LCD_DrawFilledRect **********************
// - Draws a filled rectangle, top left corner at (x,y)
// ********************************************************
void LCD_DrawFilledRect( unsigned short x,
                         unsigned short y,
                         short width,
                         short height,
                         unsigned short color );

// ************** LCD_DrawCircle **************************
// - Draws a circle centered at (x0, y0)
// ********************************************************
void LCD_DrawCircle( unsigned short x0,
                     unsigned short y0,
                     unsigned short radius,
                     short color );

// ************** LCD_DrawFilledCircle ********************
// - Draws a filled circle centered at (x0, y0)
// ********************************************************
void LCD_DrawFilledCircle( unsigned short x0,
                           unsigned short y0,
                           unsigned short radius,
                           short color );

// ************** LCD_DrawImage ***************************
// - Draws an image from memory
// - Image format is a plain byte array (no metadata)
// - User must specify:
//   - pointer to image data
//   - x, y location to draw image
//   - width and height of image
//   - bpp (bits per pixel) of image
//     - currently supports 4 and 8 bpp image data
// ********************************************************
void LCD_DrawImage( const unsigned char imgPtr[],
                    unsigned short x,
                    unsigned short y,
                    unsigned short width,
                    unsigned short height,
                    unsigned char bpp );

// ************** LCD_DrawBMP *****************************
// - Draws an image from memory
// - Image format is a BMP image stored in a byte array
// - Function attempts to resolve the following metadata
//   from the BMP format
//   - width 
//   - height
//   - bpp
//   - location of image data within bmp data
// - User must specify:
//   - pointer to image data
//   - x, y location to draw image
// ********************************************************
void LCD_DrawBMP( const unsigned char* imgPtr,
                  unsigned short x,
                  unsigned short y );

