/*
  Author: Nicholas Kucek
  3460:435 Algorithms
  Project 3: Seam Carving

  A program that takes an image and removes as many seams as specified on the command line
*/

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// find seam for deletion in the matrix
void findSeam(std::vector<std::vector<int>> &cumulativeMatrix, std::vector<std::vector<int>> &valMatrix) {

    // set dimensional integers to reference
    int y = cumulativeMatrix.size();
    int x = cumulativeMatrix[0].size();

    // find minimum index in bottom row of matrix to start seam from
    int minimum = 0;

    // iterate through the matrix's bottom row and find minimum index
    for(int i = 1; i < x; ++i) {
        if (cumulativeMatrix[y-1][minimum] > cumulativeMatrix[y-1][i]) minimum = i;
    }

    // set the value matrix
    valMatrix[y-1][minimum] = -1;

    // starting from the bottom, take the minimum value between leftUp, Up, and rightUp
    for(int i = y - 1; i >= 0; --i) {

        // Booleans for checking whether you're on the edge of the matrix
        bool widthMin = (minimum == 0);
        bool widthMax = (minimum == x - 1);

        // integers for cumulative matrix
        int leftUp =  INT_MAX,
        rightUp =  INT_MAX,
        Up =  0;

        if(i > 0) {
  
            // if you aren't on the edges, initialize the leftUp/rightUp as their actual values
            if(!widthMin) leftUp = cumulativeMatrix[i-1][minimum-1];
            if(!widthMax) rightUp = cumulativeMatrix[i-1][minimum+1];
            Up = cumulativeMatrix[i-1][minimum];

            // changing priority in the case of a tie depending on whether you have the converted matrix or not
            if (leftUp == std::min({leftUp, rightUp, Up})) {
                valMatrix[i-1][minimum-1] = -1; --minimum;
            }
            else if (Up == std::min({leftUp, rightUp, Up})) {
                valMatrix[i-1][minimum] = -1;
            }
            else {
                valMatrix[i-1][minimum+1]  = -1; ++minimum;
            }
        }
    }
}

// create energy matrix and cumulative energy matrix from the value matrix
void fillEnergyMatrices(std::vector<std::vector<int>> &valMatrix, std::vector<std::vector<int> > &energyMatrix, std::vector<std::vector<int>> &cumulativeMatrix) {

    // iterate through Matrix
    for(int i = 0; i < valMatrix.size(); ++i) {
        for(int j = 0; j < valMatrix[i].size(); ++j) {

        // set dimensional integers to reference
        int y = valMatrix.size();
        int x = valMatrix[i].size();

        // booleans to check if we are on the edges of the matrix
        bool widthMin   = (j == 0);
        bool widthMax   = (j == x - 1);
        bool heightMin  = (i == 0);
        bool heightMax  = (i== y - 1);

        // setting initial values for the possible seam paths
        int leftUp =  INT_MAX,
        rightUp =  INT_MAX,
        Up =  0;

        // starting Energy for matrix[i][j] is the initial entry in the Value Matrix
        int energy = valMatrix[i][j];

        // initialize final energy value to 0
        int finalEnergy = 0;

        // adding Energy values depending on whether the indices are on the edges of the matrix
        if(!widthMax)  finalEnergy += abs(energy - valMatrix[i][j+1]);
        if(!widthMin)  finalEnergy += abs(energy - valMatrix[i][j-1]);
        if(!heightMax) finalEnergy += abs(energy - valMatrix[i+1][j]);
        if(!heightMin) finalEnergy += abs(energy - valMatrix[i-1][j]);

        // Setting Energy Matrix and Cumulative Energy Matrix values
        energyMatrix[i][j] = finalEnergy;
        cumulativeMatrix[i][j] = finalEnergy;

        // if not on the top level (for Cumulative Energy Matrix)
        if(i > 0) {

            // if not on the far left or right of the matrix, calculate all possible values
            if (!widthMin) leftUp = finalEnergy + cumulativeMatrix[i-1][j-1];
            if (!widthMax) rightUp = finalEnergy + cumulativeMatrix[i-1][j+1];

            // calculate the path moving upwards
            Up = finalEnergy + cumulativeMatrix[i-1][j];

            // set the Cumulative Matrix at indices [i][j] to the minimum
            cumulativeMatrix[i][j] = std::min({leftUp, rightUp, Up});
            }
        }
    }
}

// initialize a new Matrix
void fillResultMatrix(std::vector<std::vector<int>> &numMatrix, std::vector<std::vector<int>> &finalMatrix) {

    // indicates if the seam to be removed has been hit
    bool flag = false;

    // iterate through the matrices and copy everything but the '-1's to the new Matrix
    for(int i = 0; i < numMatrix.size(); i++) {
        for(int j = 0; j < numMatrix[i].size(); j++) {
           if (numMatrix[i][j] == -1) flag = true;
           else finalMatrix[i][j-flag] = numMatrix [i][j];
        }
        flag = false;
    }
}

// removes [VERTICAL_SEAMS] or [HORIZONTAL_SEAMS] number of seams from the image (this is where the magic happens)
void removeSeam(int &y, int &x, int seams, std::vector<std::vector<int>> &valMatrix) {

    // iterate through [SEAMS] number of times
    for (int i = 0; i < seams; ++i) {

        // initialize vectors needed for energy/cumulative energy/final Matrices
        std::vector<std::vector<int>> energyMatrix(y, std::vector<int>(x, 0));
        std::vector<std::vector<int>> cumulativeMatrix(y, std::vector<int> (x, 0));
        std::vector<std::vector<int>> finalMatrix(y, std::vector<int> (x-1, 0));

        // fill energy matrices
        fillEnergyMatrices(valMatrix, energyMatrix, cumulativeMatrix);

        // find seam to be deleted
        findSeam(cumulativeMatrix, valMatrix);

        // fill in new matrix
        fillResultMatrix(valMatrix, finalMatrix);

        // decrement the width for the new matrices
        --x;

        // set the value matrix to the new values/dimensions
        valMatrix.clear();
        valMatrix = finalMatrix;

    }
}

// read in array from input .pgm file
void fillValMatrix(std::vector<std::vector<int>> &matrix, std::ifstream &input) {

    // read in the pgm values by iterating through the empty matrix and reading in integers
    for(int i = 0; i < matrix.size(); ++i) {
        for(int j = 0; j < matrix[i].size(); ++j) {
            int temp;
            input >> temp;
            matrix[i][j] = temp;
       }
    }
}

// convert a matrix
void convertMatrix(std::vector<std::vector<int>> &valMatrix) {
    // temporary matrix for rotation
    std::vector<std::vector<int>> RotatedMatrix(valMatrix[0].size(), std::vector<int>(valMatrix.size(), 0));

    // set height/width, iterate through and fill in new values
    int x = valMatrix[0].size();
    int y = valMatrix.size();
    for(int i = 0; i < y; i++) {
        for(int j = 0; j < x; j++) {
            RotatedMatrix[j][i] = valMatrix[i][j];
        }
    }

    // clear current matrix and set it to new one
    valMatrix.clear();
    valMatrix = RotatedMatrix;
}

// output final matrix values to file with ofstream object
void outputFinalMatrix(const std::vector<std::vector<int>> &matrix, std::ofstream &output, int max) {

    // output the literal "P2"/dimensions
    output << "P2\n";
    output << matrix[0].size() << " " << matrix.size() << "\n";
    output << max << "\n";

    // iterate through matrix and output values to file
    for(int i = 0; i < matrix.size(); i++) {
        for(int j = 0; j < matrix[i].size(); j++) {
            output << matrix[i][j] << " ";
        }
        if (i != matrix.size()-1) output << "\n";
    }
}

// get values from infile
void getValuesFromFile(int &x, int &y, int &max, std::ifstream &input) {

    // get literal "P2" from the .pgm file
    std::string P2;
    std::getline(input, P2);

    // skip the comment in the .pgm file
    input.ignore(500, '\n');

    // take in dimensions from infile
    input >> x >> y;
    input >> max;
}

int main(int argc, char * argv[]) {

    // ensure that the correct number of arguments are passed in
    if (argc != 4) {
        std::cout << "Please use the following format: '[filename] [VERTICAL_SEAMS] [HORIZONTAL_SEAMS]'\n"; return 0;
    }

    // initialize our width/height variables, as well as the max B/W value (0-255)
    int max, x, y;
    int v_seams = atoi(argv[2]);
    int h_seams = atoi(argv[3]);

    // open file and load it into a string
    std::ifstream input(argv[1]);
    std::string filename = argv[1];

    // get values from infile
    getValuesFromFile(x, y, max, input);

    // create initial Matrix to read in from infile
    std::vector<std::vector<int>> valMatrix(y, std::vector<int> (x, 0));
    fillValMatrix(valMatrix, input);
    removeSeam(y, x, v_seams, valMatrix);
    convertMatrix(valMatrix);
    std::swap(x, y);
    removeSeam(y, x, h_seams, valMatrix);
    convertMatrix(valMatrix);

    // create new filename for outfile
    std::size_t pos = filename.find_last_of(".");
    if (pos != std::string::npos) filename = filename.substr(0, pos) + "_processed" + filename.substr(pos, pos + 2);

    // create the outfile and fill it with the decompressed string
    std::ofstream output(filename);
    outputFinalMatrix(valMatrix, output, max);
    output.close();
    input.close();
}