#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <deque>
#include <sstream>
#include <stdlib.h>
#include <algorithm>

using namespace std;

/* Function Prototypes */
string userFileDialogue();                                          // Displays the Enter File Dialogue for the User
deque<string> linesInFile(string filePath);                         // Breaks the Text File into Lines and Places them in a Queue
vector< vector<string> > tokenizeLines(deque<string> linesInFile);  // Tokenizes Each Line into Recoznized Words/Syntax
string delSpaces(string &str);                                      // Deletes Whitespace in a String
int depthOfNest(vector<string> keywords);                           // Calculates the Program's Nested Loop Depth
void LexicalAnalysis(vector< vector<string> > masterVector);        // Conducts the Lexical Analysis
void printVector(vector<string> inputVector);                       // Outputs a Desired Vector
bool is_number(const string& s);                                    // Checks to See if a String is a Number
bool is_lowercase(string name);                                     // Checks to See if a String is all Lower-case (a Variable)

int main(){

    // Get File Path
    string file_path = userFileDialogue();

    // Get Lines of Code
    deque<string> file_lines = linesInFile(file_path);

    // Get Tokenized Lines
    vector< vector<string> > resultsVector = tokenizeLines(file_lines);

    // Perform Lexical Analysis
    LexicalAnalysis(resultsVector);

    // Return Success
    return 0;

}


string userFileDialogue(){

    // Stores the File Path
    string userFile;

    // Queries Information
    cout << endl;
    cout << "Please Enter the File Path: ";
    cin >> userFile;
    cout << endl;

    // Returns File Path
    return userFile;

}

deque<string> linesInFile(string filePath){

    // File Stream Variables
    ifstream infile;
    string line;
    deque<string> fileLines;

    // Open File
    infile.open(filePath.c_str());

    // If the File was Successfully Opened
    if(infile.is_open()){

        // Read the File Line by Line
        while(getline(infile, line)){

            // Line Storage
            fileLines.push_back(line);

        }

        // Close the File
        infile.close();
    }

    // If the File Could Not be Opened:
    else{
        cout << "Unable to Read File!" << endl;
        exit(1);
    }

    // Return the Lines
    return fileLines;

}

vector< vector<string> > tokenizeLines(deque<string> linesInFile){

    // Token Lists
    // Keyword List
    vector<string> keyword_list;
    keyword_list.push_back("BEGIN");
    keyword_list.push_back("END");
    keyword_list.push_back("FOR");
    keyword_list.push_back("WHILE");
    keyword_list.push_back("IF");
    keyword_list.push_back("ELSE");
    keyword_list.push_back("ELSEIF");

    // Delimiter List
    vector<string> delimiter_list;
    delimiter_list.push_back("(");
    delimiter_list.push_back(")");
    delimiter_list.push_back(";");
    delimiter_list.push_back(",");

    // Operator List
    vector<string> operator_list;
    operator_list.push_back("*");
    operator_list.push_back("/");
    operator_list.push_back("++");
    operator_list.push_back("--");
    operator_list.push_back("==");
    operator_list.push_back("+");
    operator_list.push_back("-");
    operator_list.push_back("=");
    operator_list.push_back("<");
    operator_list.push_back(">");
    operator_list.push_back("&&");
    operator_list.push_back("||");
    operator_list.push_back("!");

    // Tokens Found Lists
    vector<string> found_keyword_list;
    vector<string> found_delimiter_list;
    vector<string> found_operator_list;
    vector<string> found_identifier_list;
    vector<string> found_constants;
    vector<string> found_errors;

    // Get Length of the Input
    int inputVectorLength = linesInFile.size();

    // Iterate Over the Contents of the Input
    while(!linesInFile.empty()){

        // Gets the First Element in the Queue
        string current_line = linesInFile.front();

        // Tokenize/Categorize Line Contents
        // Keyword Tokens
        for(int i=0; i < keyword_list.size(); i++){

            string token = keyword_list.at(i);

            // If a Keyword Token is Found
            while(current_line.find(token) != string::npos){

                // Add Word to the Found List and Remove from the String
                found_keyword_list.push_back(token);
                current_line.replace(current_line.find(token),token.length(),"");

            }

        }

        // Delimiter Tokens
        for(int i=0; i < delimiter_list.size(); i++){

            string token = delimiter_list.at(i);

            // If a Delimiter Token is Found
            while(current_line.find(token) != string::npos){

                // Add Word to the Found List and Remove from the String
                found_delimiter_list.push_back(token);
                current_line.replace(current_line.find(token),token.length(),"");

            }

        }

        // Operator Tokens
        for(int i=0; i < operator_list.size(); i++){

            string token = operator_list.at(i);

            // If a Operator Token is Found
            while(current_line.find(token) != string::npos){

                // Add Word to the Found List and Remove from the String
                found_operator_list.push_back(token);
                current_line.replace(current_line.find(token),token.length()," ");

            }

        }

        // Finds All Unrecognized or Not Explicitly Stated Elements
        istringstream ss(current_line);
        string token2;

        while(getline(ss, token2, ' ')) {

            string newString = delSpaces(token2);

            if(find(found_errors.begin(), found_errors.end(), newString) == found_errors.end()){

                found_errors.push_back(newString);

            }

        }

        // Removes the First Element
        linesInFile.pop_front();

    }

    // Remove Empty Error
    found_errors.erase(std::remove(found_errors.begin(), found_errors.end(), ""), found_errors.end());

    // Analyze Errors
    for(int i=0; i < found_errors.size(); i++){

        string vector_element = found_errors.at(i);

        // If the Error is a Number
        if(is_number(vector_element)){

            // Add the Number to the Constants
            found_constants.push_back(vector_element);
            found_errors.at(i) = "DONE";

        }


        // If the Error is an Eligible Variable Name

        if(!is_number(vector_element) && is_lowercase(vector_element)){

            // Add to the Identifier List
            found_identifier_list.push_back(vector_element);
            found_errors.at(i) = "DONE";

        }

    }

    // Remove Catagorized Errors
    found_errors.erase(std::remove(found_errors.begin(), found_errors.end(), "DONE"), found_errors.end());

    // Push Individual Vectors into the MasterVector
    vector< vector<string> > MasterVector;
    MasterVector.push_back(found_keyword_list);
    MasterVector.push_back(found_delimiter_list);
    MasterVector.push_back(found_operator_list);
    MasterVector.push_back(found_identifier_list);
    MasterVector.push_back(found_constants);
    MasterVector.push_back(found_errors);

    return MasterVector;

}

void printVector(vector<string> inputVector){

    // Get Vector Size
    int inputSize = inputVector.size();

    // Print Every Element of Vector
    for(int i = 0; i < inputSize; i++){
        cout << inputVector.at(i) << " ";
    }

    // Print Blank Line
    cout << endl;

}

string delSpaces(string &str){

    // Remove all Kinds of Whitespace
    str.erase(remove(str.begin(), str.end(), ' '), str.end());
    str.erase(remove(str.begin(), str.end(), '\t'), str.end());
    str.erase(remove(str.begin(), str.end(), '\n'), str.end());
    str.erase(remove(str.begin(), str.end(), '\v'), str.end());
    str.erase(remove(str.begin(), str.end(), '\f'), str.end());
    str.erase(remove(str.begin(), str.end(), '\r'), str.end());
    return str;

}

bool is_number(const string& s){

    string::const_iterator it = s.begin();
    while (it != s.end() && isdigit(*it)) ++it;
    return !s.empty() && it == s.end();

}

bool is_lowercase(string name){

    int stringSize = name.length();
    bool isLowercase = true;

    // Traverse the String
    for(int i = 0; i < stringSize; i++){

        // If the Character is Lower-case, Return True
        if(islower(name[i])){
            isLowercase = true;
        }

        // If the Character is not Lower-case, Return False
        else{
            return false;
        }

    }

    // Return Whether the String is in All Lower-case
    return isLowercase;

}

void LexicalAnalysis(vector< vector<string> > masterVector){

    // Imported Storage
    vector<string> found_keyword_list = masterVector[0];
    vector<string> found_delimiter_list = masterVector[1];
    vector<string> found_operator_list = masterVector[2];
    vector<string> found_identifier_list = masterVector[3];
    vector<string> found_constants = masterVector[4];
    vector<string> found_errors = masterVector[5];

    // Unique Storage
    vector<string> unique_keywords = found_keyword_list;
    vector<string> unique_delimiters = found_delimiter_list;
    vector<string> unique_operators = found_operator_list;
    vector<string> unique_identifier = found_identifier_list;
    vector<string> unique_constants = found_constants;
    vector<string> unique_errors = found_errors;

    // Make Keywords Vector Unique
    sort( unique_keywords.begin(), unique_keywords.end() );
    unique_keywords.erase( unique( unique_keywords.begin(), unique_keywords.end() ), unique_keywords.end() );

    // Make Delimiters Unique
    sort( unique_delimiters.begin(), unique_delimiters.end() );
    unique_delimiters.erase( unique( unique_delimiters.begin(), unique_delimiters.end() ), unique_delimiters.end() );

    // Make Operators Unique
    sort( unique_operators.begin(), unique_operators.end() );
    unique_operators.erase( unique( unique_operators.begin(), unique_operators.end() ), unique_operators.end() );

    // Make Identifiers Unique
    sort( unique_identifier.begin(), unique_identifier.end() );
    unique_identifier.erase( unique( unique_identifier.begin(), unique_identifier.end() ), unique_identifier.end() );

    // Make Constants Unique
    sort( unique_constants.begin(), unique_constants.end() );
    unique_constants.erase( unique( unique_constants.begin(), unique_constants.end() ), unique_constants.end() );

    // Make Errors Unique
    sort( unique_errors.begin(), unique_errors.end() );
    unique_errors.erase( unique( unique_errors.begin(), unique_errors.end() ), unique_errors.end() );

    // Get Depth of Loop(s)
    int depthofNest = depthOfNest(found_keyword_list);

    // Output Data
    cout << "The Depth of the Nested Loop(s) is: " << depthofNest << endl;
    cout << "Keywords: ";
    printVector(unique_keywords);
    cout << "Identifier: ";
    printVector(unique_identifier);
    cout << "Constant: ";
    printVector(unique_constants);
    cout << "Operators: ";
    printVector(unique_operators);
    cout << "Delimiters: ";
    printVector(unique_delimiters);
    cout << endl;

    // Error Analysis: Syntax
    for(int counter = 0; counter < unique_errors.size(); counter++){

        string current_Error = unique_errors.at(counter);

        cout << "ERROR: " << current_Error << endl;

    }

    // Error Analysis: Symmetry
    int rightBrace_counter = 0;
    int leftBrace_counter = 0;
    int loopcounter = 0;
    int beginCounter = 0;
    int endCounter = 0;
    int equalCounter = 0;
    int semicolonCounter = 0;
    int forCounter = 0;
    int colonCounter = 0;

    // Brace Symmetry
    for(int counter = 0; counter < found_delimiter_list.size(); counter++){

        string current_word = found_delimiter_list.at(counter);

        if(current_word == "("){

            leftBrace_counter = leftBrace_counter + 1;

        }

        if(current_word == ")"){

            rightBrace_counter = rightBrace_counter + 1;

        }

    }

    // If Braces are not Symmetrical, Display Error
    if(rightBrace_counter != leftBrace_counter){

        cout << "ERROR: Incorrect () Symmetry!" << endl;

    }

    // Loop BEGIN/END Symmetry:
    for(int counter = 0; counter < found_keyword_list.size(); counter++){

        string current_word = found_keyword_list.at(counter);

        if(current_word == "FOR" || current_word == "WHILE"){

            loopcounter = loopcounter + 1;

        }

        if(current_word == "BEGIN"){

            beginCounter = beginCounter + 1;

        }

        if(current_word == "END"){

            endCounter = endCounter + 1;

        }

    }

    // IF the Number of Loops and Begin Statements Don't Match
    if(beginCounter != endCounter) {

        if (loopcounter != beginCounter) {

            if (loopcounter > beginCounter) {

                int differenceCounter = loopcounter - beginCounter;

                cout << "ERROR: Missing " << differenceCounter << " BEGIN statments!" << endl;

            }

            if (loopcounter < beginCounter) {

                int differenceCounter = beginCounter - loopcounter;

                cout << "ERROR: " << differenceCounter << " Too Many BEGIN Statements!";

            }

        }

        // IF the Number of Loops and END Statements Don't Match
        if (loopcounter != endCounter) {

            if (loopcounter > endCounter) {

                int differenceCounter = loopcounter - endCounter;

                cout << "ERROR: Missing " << differenceCounter << " END statments!" << endl;

            }

            if (loopcounter < endCounter) {

                int differenceCounter = endCounter - loopcounter;

                cout << "ERROR: " << differenceCounter << " Too Many END Statements!";

            }

        }

        // Equal Symmentry
        for(int counter = 0; counter < found_delimiter_list.size(); counter++){

            string current_word = found_delimiter_list.at(counter);

            if(current_word == ";"){

                semicolonCounter = semicolonCounter + 1;

            }

        }

        for(int counter = 0; counter < found_operator_list.size(); counter++){

            string current_word = found_operator_list.at(counter);

            if(current_word == "="){

                equalCounter = equalCounter + 1;

            }

        }

        if(semicolonCounter != equalCounter){

            if(equalCounter > semicolonCounter) {

                cout << "ERROR: Missing ';' " << endl;

            }

            if(equalCounter < semicolonCounter){

                cout << "ERROR: Incorrect ';' Usage" << endl;

            }

        }

        // Colon Check
        for(int counter = 0; counter < found_delimiter_list.size(); counter++){

            string current_word = found_delimiter_list.at(counter);

            if(current_word == ","){

                colonCounter = colonCounter + 1;

            }

        }

        // FOR Check
        for(int counter = 0; counter < found_keyword_list.size(); counter++){

            string current_word = found_keyword_list.at(counter);

            if(current_word == "FOR"){

                forCounter = forCounter + 1;

            }

        }

        if(colonCounter != (2*forCounter)){

            if(colonCounter > (2*forCounter)){

                cout << "ERROR: " << colonCounter - (2*forCounter) << " Too Many ',' " << endl;

            }

            if(colonCounter < (2*forCounter)){

                cout << "ERROR: Missing " << (2*forCounter) - colonCounter << " ',' " << endl;

            }

        }

    }

    // If the BEGIN/END Statements Match, but Don't Match the Number of Loops:
    if(beginCounter == endCounter && beginCounter != loopcounter) {

        cout << "ERROR: END/BEGIN Statements Don't Match the Number of Loops!" << endl;

    }

}

int depthOfNest(vector<string> keywords){

    int depth = 0;
    int max_depth = 0;

    // Traverse the Keyword List
    for(int counter = 0; counter < keywords.size(); counter++){

        // Current Word
        string current_word = keywords.at(counter);

        // Increase the Depth by 1 if the Current Word is FOR or WHILE
        if(current_word == "FOR" || current_word == "WHILE"){

            depth = depth + 1;

        }

        // Once Reaching an END Check for New Max and Subtract Depth
        if(current_word == "END"){

            if(depth > max_depth){

                max_depth = depth;

            }

            depth = depth - 1;

        }

    }

    // Final Depth Check
    if (depth > max_depth){

        max_depth = depth;

    }

    // Return the Max Depth
    return max_depth;

}