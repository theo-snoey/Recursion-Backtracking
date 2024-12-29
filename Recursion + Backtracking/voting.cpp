/*
 * Author: Theo Snoey
 * This file uses recursion to compute power indexes for different voting factions and how their
 * Positioning in a group of voters and they ways that these voting factions make choices, influence the power
 * of a given faction, even when they may not necessarily have the most votes.
 */
#include <iostream>    // for cout, endl
#include <string>      // for string class
#include "backtracking.h"
#include "SimpleTest.h"
using namespace std;


// this function, countCriticalVotes, takes an empty vector of blockTs soFar, a vector of blockTs rest (that excludes the
// block whose votes we want to count, takes a winnning threshold for that entire vector of blockTs (including the vector whose
// votes we are trying to count and takes an int blockCount which represents the count of votes from the blockT we excluded.
// Using recursive processes, this function returns the number of critical votes that the block in question has.

int countCriticalVotes(Vector<blockT>& soFar, Vector<blockT>& rest, int winningThreshold, int blockCount, int restIndex){

    if (restIndex == rest.size()){
        int totalVotes = 0;
        for (blockT block: soFar){
            totalVotes += block.count;
        }

        if (totalVotes < winningThreshold && (totalVotes + blockCount) >= winningThreshold){
            return 1;
        }

        else{
            return 0;
        }
    }

    blockT thisOne = rest[restIndex];


    int without = countCriticalVotes(soFar, rest, winningThreshold, blockCount, restIndex + 1);

    Vector<blockT> soFar1 = soFar;
    soFar1.add(thisOne);

    int with = countCriticalVotes(soFar1, rest, winningThreshold, blockCount, restIndex + 1);

    return without + with;
}


// this wrapper function for the recursive function countCriticalVotes takes a Vector of blockTs blocks and an
// int index and calls the recursive function to return the number of critical votes for that block.
int countCriticalVotes(Vector<blockT>& blocks, int index){

    int blockCount = blocks[index].count;

    // compute winning threshold
    int allBlocksSum = 0;

    for (blockT block: blocks){
        allBlocksSum += block.count;
    }

    int winningThreshold = (allBlocksSum / 2) + 1;


    //initialize empty soFar vector
    Vector<blockT> soFar = {};


    //initialize rest vector

    Vector<blockT> rest;

    if (index < blocks.size() - 1 ){
        rest = blocks.subList(0, index) + blocks.subList(index + 1);
    }

    else{
        rest = blocks.subList(0, blocks.size() - 1);
    }


    return countCriticalVotes(soFar, rest, winningThreshold, blockCount, 0);

}


/*
 * This functino computePowerIndexes takes a Vector of blockTs and calls the two helper functions countCriticalVotes
 * to compute the block Bahzhaf number for each blockT in the blocks vector, this function then takes those values and puts them
 * and their ids into a map, mapping ids to Bahzhaf scores.
 */
Map<string,int> computePowerIndexes(Vector<blockT>& blocks) {
    Map<string,int> result;

    // count total critical votes
    int totalCriticalVotes = 0;
    for (int i = 0; i < blocks.size(); i++){
        totalCriticalVotes += countCriticalVotes(blocks, i);
    }

    // for every block in blocks compute Banzhaf power index and add id and Banzhaf to result map
    for (int j = 0; j < blocks.size(); j++){

        int blockCritical = countCriticalVotes(blocks, j);

        int blockBanzhaf = (blockCritical * 100) / totalCriticalVotes;

        result[blocks[j].id] = blockBanzhaf;
    }

    return result;
}



/* * * * * * Test Cases * * * * * */

//______________________________________STUDENT_TESTS_______________________________________________


STUDENT_TEST("test my non-recursive countCriticalVotes function for accuracy, plugging in Lion"){
    Vector<blockT> blocks = {{"Lions",50}, {"Tigers",49}, {"Bears",1}};
    int index = 0;
    int LionCriticalVotes = countCriticalVotes(blocks, index);
    EXPECT_EQUAL(LionCriticalVotes, 3);
}


STUDENT_TEST("test my recursive countCriticalVotes function for accuracy"){
    Vector<blockT> rest = {{"Lions",50}, {"Tigers",49}};
    Vector<blockT> soFar = {};
    int winningThreshold = 51;
    int blockCount = 1;
    int bearCriticalVotes = countCriticalVotes(soFar, rest, winningThreshold, blockCount, 0);
    EXPECT_EQUAL(bearCriticalVotes, 1);
}



//______________________________________PROVIDED_TESTS_______________________________________________


PROVIDED_TEST("Power indexes: lions-50 tigers-49 bears-1") {
    Vector<blockT> blocks = {{"Lions",50}, {"Tigers",49}, {"Bears",1}};
    Map<string,int> expected = {{"Lions",60}, {"Tigers",20}, {"Bears",20}};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Power indexes: Hempshead 1-1-3-7-9-9") {
    Vector<blockT> blocks = {{"A",1}, {"B",1}, {"C",3}, {"D",7}, {"E",9}, {"F",9}};
    Map<string,int> expected = {{"A",0}, {"B",0}, {"C",0}, {"D",33}, {"E",33}, {"F",33}};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Power indexes: CA-55 TX-38 NY-29") {
    Vector<blockT> blocks = {{"CA",55}, {"TX",38}, {"NY",29}};
    Map<string,int> expected = {{"CA",33}, {"TX",33}, {"NY",33}};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Power indexes: CA-55 TX-38 GA-16") {
    Vector<blockT> blocks = {{"CA",55}, {"TX",38}, {"GA",16}};
    Map<string,int> expected = {{"CA",100}, {"TX",0}, {"GA",0}};
    EXPECT_EQUAL(computePowerIndexes(blocks), expected);
}

PROVIDED_TEST("Power indexes: time operation") {
    Vector<blockT> blocks;
    for (int i = 0; i < 24; i++) {
        blockT b = {charToString('A' + i), randomInteger(1, 10)};
        blocks.add(b);
    }
    TIME_OPERATION(blocks.size(), computePowerIndexes(blocks));
}
