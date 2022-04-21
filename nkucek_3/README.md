# Seam carving
* Algorithm for content-aware image resizing
* Performs a seemingly impossible task of resizing an image without cropping it or distorting its contents with ease
* Changes the size of an image by removing the least visible pixels in the image. The visibility of a pixel can be defined using an energy function. Seam carving can be done by finding an one-pixel wide path of lowest energy crossing the image from top to bottom (vertical path) or from left to right (horizontal path) and removing the path (seam).
* The basic algorithm steps are as follows:
    1. Start with an image.
    2. Calculate the weight/density/energy of each pixel.
    3. From the energy, make a list of seams.
    4. Remove low-energy seams as needed.
    5. Display the final image.

# How to Run
* Use Makefile to easily build (seamcarving435.cpp) and clean the executable for each run: ```make clean && make all```
* Commands are ran in the following format: <code> ./seam [filename] [VERTICAL_SEAMS] [HORIZONTAL_SEAMS] </code>
    * Example: <code> ./seam stars.pgm 100 50 </code> - removes 100 vertical seams and 50 horizontal seams
* The image after being ran through the algorithm will create a new image file under the name of image_name_**processed**.pgm
* After creating the new image, it is best to use an external file viewer to see the complete image. However, always check the original .pgm file in the text editor to ensure that the correct number of rows and columns are displayed after the file type (i.e. if stars.pgm is originally 284 X 308, then after running ```./seam stars.pgm 211 68```, the new file stars_**processed**.pgm should display "73 240" on the second or third line of the .pgm file, depending on if a comment was added to the file or not).