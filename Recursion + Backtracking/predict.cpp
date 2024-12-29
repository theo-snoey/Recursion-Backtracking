/*
 * Author: Theo Snoey
 * This file, predict.cpp uses recursion to take a keypad sequence of digits and to provide suggested
 * real english words corresponding to that sequence.
 */
#include <iostream>
#include "backtracking.h"
#include "lexicon.h"
#include "set.h"
#include "SimpleTest.h"
#include "strlib.h"

using namespace std;

// keypad is a program-wide constant that stores the Map from digit char to
// its associated set of possible letters
static const Map<char, Set<char>> keypad = {
    {'2', {'a','b','c'} }, {'3', {'d','e','f'} }, {'4', {'g','h','i'} },
    {'5', {'j','k','l'} }, {'6', {'m','n','o'} }, {'7', {'p','q','r','s'} },
    {'8', {'t','u','v'} }, {'9', {'w','x','y','z'} } };




// this function, predict, takes a string of digits, a set of suggestions, a lexicon, and a string of
//stored combinations and recursively (as a wrapper function), stores suggested english words based off of the
//key pad digit string inputted adding it to the suggestions set, pass by reference).
void predict(string digits, Set<string>& suggestions, Lexicon& lex, string comb) {

    if (digits.length() == 0){
        if (lex.contains(comb)){
            suggestions.add(comb);
        }
        return;
    }

    if (!lex.containsPrefix(comb)){
        return;
    }

    char thisDigit = digits[0];
    Set<char> theseLetters = keypad[thisDigit];

    string restDigits;

    if (digits.length() == 1){
        restDigits = "";
    }

    else {
        restDigits = digits.substr(1);
    }


    for (char letter : theseLetters){
        predict(restDigits, suggestions, lex, comb + letter);
    }

}

/*
 * This function pertedict, calls the wrapper function, predict to go through a key pad of english chars mapped to
 * digits to fill a set of suggestions of english words for those keypad sequences.
 */
void predict(string digits, Set<string>& suggestions, Lexicon& lex) {

    return predict(digits, suggestions, lex, "");
}


/* Test helper function to return shared copy of Lexicon. Use to
 * avoid (expensive) re-load of word list on each test case. */
static Lexicon& sharedLexicon() {
    static Lexicon lex("res/EnglishWords.txt");
    return lex;
}

/* * * * * * Test Cases * * * * * */


//_______________________STUDENT_TESTS_______________________________________________________

STUDENT_TEST("Predict intended words for digit sequence 6263, example from writeup") {
    string digits = "523";
    Set<string> expected = {"kae", "kaf", "lad"};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}

//_______________________PROVIDED_TESTS_______________________________________________________

PROVIDED_TEST("Predict intended words for digit sequence 6263, example from writeup") {
    string digits = "6263";
    Set<string> expected = {"name", "mane", "oboe"};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}

PROVIDED_TEST("What words can I spell from my phone number?") {
    string digits = "3228679";  // my old phone number :-)
    Set<string> expected = {"factory"};
    Set<string> suggestions;

    predict(digits, suggestions, sharedLexicon());
    EXPECT_EQUAL(suggestions, expected);
}
