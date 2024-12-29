/*
 * Author: Theo Snoey
 * this file redistric.cpp serves to work through a grid of integers/districts and tells us if its possible to redistrict
 * the population distribution of a state/grid to match a new criteria of a new set of districts/subsection of a Grid.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "grid.h"
#include "gridgraphics.h"
#include "SimpleTest.h"
using namespace std;


/* This function subgridSum takes a Grid g and a Grid Location range and adds up the individual values of that range mapped onto the grid
 * returning it as an int.
 */

int subgridSum(Grid<int>& g, GridLocationRange range) {

    GridLocation upperLeft = range.startLocation();

    int numRows = range.numRows();
    int numCols = range.numCols();

    int sum = 0;

    for (int i = upperLeft.row; i < upperLeft.row + numRows; i++){
        for (int j = upperLeft.col; j < upperLeft.col + numCols; j++){
            int locVal = g[i][j];
            sum += locVal;
        }
    }

    return sum;
}


// this function targetSize takes a grid g and an integer n and calls the function subgridSum calculating the
// int target size for a district, returning it.
int targetSize(Grid<int>& g, int n){

    int sum = subgridSum(g, g.locations());
    int result = sum / n;

    return result;
}



// this function ConvertRangeToGrid takes a Grid and a GridlocationRange and returns that grid location range in Grid form (mapped based on the
// Grid passed into the params.
Grid<int> convertRangeToGrid(Grid<int>& g, GridLocationRange range){

    Grid<int> newGrid(range.numRows(), range.numCols());

    int y = 0;

    for (int j = range.startCol(); j < range.startCol() + range.numCols(); j++){
        int x = 0;
        for (int i = range.startRow(); i < range.startRow() + range.numRows(); i++){
            newGrid[x][y] = g[i][j];
            x++;
        }
        y ++;
    }
    return newGrid;
}

// this function, divideHorizontal takes a GridlocationRange, an integer representing the end row of our top
// GridlocationRange and two GridlocationRanges. It returns void, but it reassigns the values of both top and bottom to equal the
// intended top and bottom GridlocationRanges based off of a "split" between two subsections.

void divideHorizontal(GridLocationRange range, int topEndRow, GridLocationRange& top, GridLocationRange& bottom) {


    GridLocationRange top1(0, 0, topEndRow, range.endCol());
    top = top1;

    GridLocationRange bottom1(topEndRow + 1, 0, range.endRow(), range.endCol());
    bottom = bottom1;

}


// this function, divideVertical takes a GridlocationRange, an integer representing the end column of our left
// GridlocationRange and two GridlocationRanges. It returns void, but it reassigns the values of both left and right to equal the
// intended left and right GridlocationRanges based off of a "split" between two subsections.

void divideVertical(GridLocationRange range, int leftEndCol, GridLocationRange& left, GridLocationRange& right) {

    GridLocationRange left1(0, 0, range.endRow(), leftEndCol);
    left = left1;

    GridLocationRange right1(0, leftEndCol + 1, range.endRow(), range.endCol());
    right = right1;

}


// this funciton canRedistrict takes a grid g and a GridlocationRange range and serves as a wrapper function to the original
//funciton canRedistrict. This function recursively splits a grid into sub grids checking if the sums of the new grids match
// certain permaters that convey whether or not that original grid can be redistricted (it returns this as a boolean)
//Its peramters aer a Grid of integers, an integer of district, and a target size of one subgrid.
bool canRedistrict(Grid<int>& populationGrid, int nDistricts, int targetSize) {

    if (subgridSum(populationGrid, populationGrid.locations()) == targetSize){
        return true;
    }

    if (subgridSum(populationGrid, populationGrid.locations()) % targetSize != 0){
        return false;
    }


    GridLocationRange thisGridRange = populationGrid.locations();


    GridLocationRange top;
    GridLocationRange bottom;
    GridLocationRange left;
    GridLocationRange right;

    for (int i = 0; i < populationGrid.numRows() - 1; i++){
        divideHorizontal(thisGridRange, i, top, bottom);

        Grid<int> topGrid = convertRangeToGrid(populationGrid, top);
        Grid<int> bottomGrid = convertRangeToGrid(populationGrid, bottom);

        if (canRedistrict(topGrid, nDistricts, targetSize) && canRedistrict(bottomGrid, nDistricts, targetSize) == true){
            return true;
        }
    }


    for (int j = 0; j < populationGrid.numCols() - 1; j++){
        divideVertical(thisGridRange, j, left, right);

        Grid<int> leftGrid = convertRangeToGrid(populationGrid, left);
        Grid<int> rightGrid = convertRangeToGrid(populationGrid, right);

        if (canRedistrict(leftGrid, nDistricts, targetSize) && canRedistrict(rightGrid, nDistricts, targetSize) == true){
            return true;
        }

    }

    return false;
}


/*
 * This function, canRedistrict takes a grid populationGrid, and an int nDistrics(the number of districts) and calls the helper
 * function target size to then call the recursive wrapper function canRedistrict to obtain a true or false statement of
 * whether or not the grid can be "redistricted" with n districts.
 */
bool canRedistrict(Grid<int>& populationGrid, int nDistricts) {

   int tSize = targetSize(populationGrid, nDistricts);
   return canRedistrict(populationGrid, nDistricts, tSize);

}






/* * * * * * Test Cases * * * * * */

//___________________________________STUDENT_TESTS________________________________________



STUDENT_TEST("Test Divide Horizontal"){
    GridLocationRange range(0, 0, 3, 3);
    int topEndRow = 0;
    GridLocationRange top;
    GridLocationRange bottom;
    divideHorizontal(range, topEndRow, top, bottom);
    GridLocationRange expectedTop(0, 0, 0, 3);
    GridLocationRange expectedBottom(1, 0, 3, 3);
    bool topRow = top.numRows() == expectedTop.numRows();
    bool topCol = top.numCols() == expectedTop.numCols();
    bool bottomRow = bottom.numRows() == expectedBottom.numRows();
    bool bottomCol = bottom.numCols() == expectedBottom.numCols();

    EXPECT(topRow);
    EXPECT(topCol);
    EXPECT(bottomRow);
    EXPECT(bottomCol);
}


STUDENT_TEST("Test Divide Vertical"){

    GridLocationRange range(0, 0, 3, 3);
    int leftEndCol = 0;
    GridLocationRange left;
    GridLocationRange right;
    divideVertical(range, leftEndCol, left, right);
    GridLocationRange expectedLeft(0, 0, 3, 0);
    GridLocationRange expectedRight(0, 1, 3, 3);
    bool topRow = left.numRows() == expectedLeft.numRows();
    bool topCol = left.numCols() == expectedLeft.numCols();
    bool bottomRow = right.numRows() == expectedRight.numRows();
    bool bottomCol = right.numCols() == expectedRight.numCols();

    EXPECT(topRow);
    EXPECT(topCol);
    EXPECT(bottomRow);
    EXPECT(bottomCol);
}


STUDENT_TEST("test my convertRangeToGrid function"){
    Grid<int> grid = { {1, 2, 3, 4}, {4, 5, 6, 7}, {7, 8, 9, 8}, {10, 10, 10, 10} };
    GridLocationRange subGrid = GridLocationRange(1, 1, 3, 3);
    Grid<int> resultGrid = { {5, 6, 7}, {8, 9, 8}, {10, 10, 10} };
    EXPECT_EQUAL(convertRangeToGrid(grid, subGrid), resultGrid);
}


STUDENT_TEST("targetSize function given grid and 8 districts") {
    Grid<int> grid = { {1, 2, 3, 4}, {4, 5, 6, 7}, {7, 8, 9, 8}, {10, 10, 10, 10} };
    int n = 8;
    EXPECT_EQUAL(targetSize(grid, n), 13);
}



STUDENT_TEST("subgridSum sums values in grid range") {
    Grid<int> grid = { {1, 2, 3, 4}, {4, 5, 6, 7}, {7, 8, 9, 8}, {10, 10, 10, 10} };
    GridLocationRange all = grid.locations();
    GridLocationRange subGrid1 = GridLocationRange(1, 1, 3, 3);
    GridLocationRange subGrid2 = GridLocationRange(0, 0, 1, 1);

    EXPECT_EQUAL(subgridSum(grid, subGrid1), 73);
    EXPECT_EQUAL(subgridSum(grid, subGrid2), 12);
    EXPECT_EQUAL(subgridSum(grid, all), 104);
}



//___________________________________PROVIDED_TESTS_______________________________________

PROVIDED_TEST("subgridSum sums values in grid range") {
    // This is an example test of helper function subgridSum
    Grid<int> grid = { {1, 2, 3}, {4, 5, 6}, {7, 8, 9}, {10, 10, 10} };
    GridLocationRange all = grid.locations();
    GridLocationRange firstColumn = GridLocationRange(0, 0, 3, 0);
    GridLocationRange bottomCorner = GridLocationRange(3, 2, 3, 2);

    EXPECT_EQUAL(subgridSum(grid, firstColumn), 22);
    EXPECT_EQUAL(subgridSum(grid, bottomCorner), 10);
    EXPECT_EQUAL(subgridSum(grid, all), 75);
}

PROVIDED_TEST("nevada example from writeup can be divided into 4 districts") {
    Grid<int> nevada = { {20, 31, 20},
                         { 5, 19, 30},
                         {25, 25, 25} };


    EXPECT(canRedistrict(nevada, 4));
    waitForCloseWindow();
}

PROVIDED_TEST("wyoming example from writeup cannot be divided into 2 districts") {
    Grid<int> wyoming = { {10, 15},
                          { 5, 10} };


    EXPECT(!canRedistrict(wyoming, 2));
    waitForCloseWindow();
}

PROVIDED_TEST("small example, 5 districts") {
    Grid<int> small = { {3, 7, 10},
                        {6, 3, 1},
                        {1, 10, 9} };

    EXPECT(canRedistrict(small, 5));
    waitForCloseWindow();
}

PROVIDED_TEST("medium example, 12 districts") {
    Grid<int> medium = { {10, 1,  2, 5, 1, 4},
                          {1, 1,  8, 2, 1, 7},
                          {9, 8, 10, 3, 3, 4},
                          {5, 2,  2, 8, 7, 3},
                          {2, 1,  1, 7, 1, 1} };

    EXPECT(canRedistrict(medium, 12));
    waitForCloseWindow();
}

PROVIDED_TEST("large example can be divided into 25 districts") {
    Grid<int> large = { { 5,  2, 2,  1,  6, 4, 8, 2},
                        {10, 10, 2,  1,  3, 4, 2, 3},
                        { 1,  1, 4,  5,  1, 9, 1, 4},
                        { 9,  5, 5, 10, 10, 3, 3, 4},
                        { 3,  1, 1,  1,  2, 8, 6, 2},
                        { 2,  1, 1,  7,  3, 5, 1, 1},
                        { 1,  1, 2,  2,  7, 5, 1, 9},
                        { 4,  1, 2, 10,  1, 9, 5, 5} };

    EXPECT(canRedistrict(large, 25));
    waitForCloseWindow();
}
