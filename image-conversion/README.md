Program to convert 90x30 bmp image to byte array of dimensions 30x30.
The images to be converted are stored in ./bmpouts/9x9
there are two different configurations of the neopixel matrix: horizontal zigzag and horizontal snakeline.
there are programs for each of the conversion types with 
zigzag storing converted files in d[x] directory
snakeline storing converted files in s[x] directory
where d[x] means d1,d2,d3 while s[x] means s1,s2,s3
x means the neopixel matrix number.
the output files can then be stored in sdcard loaded into esp32