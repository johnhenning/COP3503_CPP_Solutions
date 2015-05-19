#include <iostream>
#include <string>
#include <stdlib.h>
#include <iomanip>

using namespace std;

/* Function Prototypes */
void bestFit();                                                                 // Best Fit Algorithm to Allocate Contiguous Memory
void worstFit();                                                                // Worst Fit Algorithm to Allocate Contiguous Memory
void displayMenu();                                                             // Displays User Option Menu
void initPage(struct page *head, string programName);                           // Initializes First Page in Linked List
void addPage(struct page *head, string programName);                            // Adds Page to Linked List
bool deletePage(struct page **head, page *pageDelete);                          // Deletes Specific Page from Linked List
void deleteEnd(struct page *head);                                              // Deletes Last Page in Linked List
void outputMemory(struct page *head, struct page *head2);                       // Outputs Linked List for Used and Free Memory
void allocateFree(struct page **head, page *pagetoFree);                        // Traverses a Linked List and Marks a Specific Page as Free
//void countFragments(struct page *head);
void countFragments(struct page *head, struct page *head2);                     // Counts the Number of Fragments in Memory (Used and Free Linked Lists)
int kbtoPages(int kb);                                                          // Converts Program Size to KB
int listLength(struct page *head);                                              // Returns the Length of a Linked List
void writeToUsed(page *startNode, int numPages, string pgrmName);               // Writes to the Used Memeory Linked List
struct page *usedLargest(struct page *head, int &lrgstFree);                    // Returns the Starting Node for the Largest Free Space Block
struct page *usedSmallest(struct page *head, int &smlstFree, int minimumSize);  // Returns the Starting Node for the Smallest Possible Free Space Block
struct page *searchPage(struct page *head, string programName);                 // Searches Linked List for Program
bool inMemory(struct page *head, string programName);                           // Searches if a Program is Already in Memory

// Memory Page Structure:
struct page{
    string programName;
    page *nextPage;
};

int main(int argc, char *argv[]){

    // If the Incorrect Number of Arguments are Entered, Display Error
    if(argc != 2){
        cout << "Wrong Number of Arguments!" << endl;
        return -1;
    }

        // If the Worst Fit Algorithm is Chosen, Launch it.
    else if(string(argv[1]) == "worst"){
        worstFit();
    }

        // If the Best Fit Algorithm is Chosen, Launch it.
    else if(string(argv[1]) == "best"){
        bestFit();
    }

        // If the Parameter is Unrecognized, Display Error;
    else {
        cout << "Unrecognized Parameter!" << endl;
        return -1;
    }

    // Main Method Successful Completion.
    return 0;

}

void bestFit(){

    // Variables
    int userChoice = 0;
    int programSize;
    int pagesUsed = 0;
    int pageDeleteCounter = 0;
    int smallestFreePages = 0;
    string pName;
    struct page *freeMemory = new page;
    struct page *usedMemory = new page;

    // Initializing 32 Pages of Free Memory
    for(int counter = 1; counter <= 32; counter++){
        addPage(freeMemory, "Free");
    }

    // Output User Menu
    cout << "Using the Best Fit Algorithm" << endl;
    cout << endl;
    displayMenu();
    cout << "Option: ";
    cin >> userChoice;

    while(userChoice != 5){

        switch (userChoice) {

            // Unrecognized Input
            default: {
                cout << "Sorry, Unrecognized Input!" << endl;
                cout << endl;
                displayMenu();
                cout << "Option: ";
                cin >> userChoice;
                break;
            }

            case 1:{

                // If the Used Memory is Empty
                if(listLength(usedMemory) == 0){

                    // Add Programs to Free Memory
                    cout << "Enter the Program Name: ";
                    cin >> pName;
                    cout << "Program Size (KB): ";
                    cin >> programSize;
                    programSize = kbtoPages(programSize);

                    // If the Program that Wants to be Added is Within the Memory Quota, Add it.
                    if (pagesUsed + programSize <= 32 && !inMemory(usedMemory, pName)) {

                        // If there is Enough Free Space in the Free Space Linked List, Add Program to it.
                        if (listLength(freeMemory) >= programSize) {

                            // Updating the Number of Pages Used
                            pagesUsed = pagesUsed + programSize;

                            // For Each Page of Memory Used, Delete a Page from the Free Memory List and Add the
                            // Program to the Used Memory List
                            for (int counter = 1; counter <= programSize; counter++) {
                                addPage(usedMemory, pName);
                                deleteEnd(freeMemory);
                            }

                            // Success Output
                            cout << "Program " << pName << " Successfully Added! ("
                                    << programSize << " Page(s) Used)" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;

                        }

                            // If there is Not Enough Free Space in the Free Linked List, Query the Used Memory for Free Space
                        else{

                            cout << "Error, Not Enough Memory to Add " << pName << "!" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;

                        }

                    }

                        // If the Program that Wants to be Added Exceeds the Memory Quota, Display Error.
                    else if(pagesUsed + programSize > 32){
                        cout << "Error, Not Enough Memory to Add " << pName << "!" << endl;
                        displayMenu();
                        cout << "Option: ";
                        cin >> userChoice;
                    }

                        // If the Program that Wants to be Added is Already in Memory, Display Error.
                    else if(inMemory(usedMemory, pName)){
                        cout << "Error, Program " << pName << " is Already in Memory!" << endl;
                        displayMenu();
                        cout << "Option: ";
                        cin >> userChoice;
                    }

                        // If Anything Else Happens, Display Unknown Error
                    else{
                        cout << "Unknown Error!" << endl;
                        displayMenu();
                        cout << "Option: ";
                        cin >> userChoice;
                    }

                }

                // If Used Memory List is Not Empty
                else if(listLength(usedMemory) != 0){

                    // Add Programs to Memory
                    cout << "Enter the Program Name: ";
                    cin >> pName;
                    cout << "Program Size (KB): ";
                    cin >> programSize;
                    programSize = kbtoPages(programSize);

                    page *startingBlock = usedSmallest(usedMemory, smallestFreePages, programSize);

                    if(startingBlock != NULL && smallestFreePages != 0) {

                        if (smallestFreePages >= programSize) {

                            pagesUsed = pagesUsed + programSize;
                            writeToUsed(startingBlock, programSize, pName);

                            // Success Output
                            cout << "Program " << pName << " Successfully Added! ("
                                    << programSize << " Page(s) Used)" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;

                        }

                        else if(smallestFreePages < programSize){
                            cout << "Error, Not Enough Memory to Add " << pName << "!" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;
                        }

                        smallestFreePages = 0;

                    }

                    else if(startingBlock == NULL || smallestFreePages == 0){

                        // If the Program that Wants to be Added is Within the Memory Quota, Add it.
                        if (pagesUsed + programSize <= 32 && !inMemory(usedMemory, pName)) {

                            // If there is Enough Free Space in the Free Space Linked List, Add Program to it.
                            if (listLength(freeMemory) >= programSize) {

                                // Updating the Number of Pages Used
                                pagesUsed = pagesUsed + programSize;

                                // For Each Page of Memory Used, Delete a Page from the Free Memory List and Add the
                                // Program to the Used Memory List
                                for (int counter = 1; counter <= programSize; counter++) {
                                    addPage(usedMemory, pName);
                                    deleteEnd(freeMemory);
                                }

                                // Success Output
                                cout << "Program " << pName << " Successfully Added! ("
                                        << programSize << " Page(s) Used)" << endl;
                                displayMenu();
                                cout << "Option: ";
                                cin >> userChoice;

                            }

                                // If there is Not Enough Free Space in the Free Linked List, Query the Used Memory for Free Space
                            else{

                                cout << "Error, Not Enough Memory to Add " << pName << "!" << endl;
                                displayMenu();
                                cout << "Option: ";
                                cin >> userChoice;

                            }

                        }

                            // If the Program that Wants to be Added Exceeds the Memory Quota, Display Error.
                        else if(pagesUsed + programSize > 32){
                            cout << "Error, Not Enough Memory to Add " << pName << "!" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;
                        }

                            // If the Program that Wants to be Added is Already in Memory, Display Error.
                        else if(inMemory(usedMemory, pName)){
                            cout << "Error, Program " << pName << " is Already in Memory!" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;
                        }

                            // If Anything Else Happens, Display Unknown Error
                        else{
                            cout << "Unknown Error!" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;
                        }

                    }

                }

                break;
            }

                // Kill Program
            case 2: {

                cout << "Enter the Program Name: ";
                cin >> pName;

                // If the Program is not in Memory
                if (!inMemory(usedMemory, pName)) {
                    cout << "Program " << pName << " is not in Memory!" << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                }

                    // If the Program IS in Memory
                else if (inMemory(usedMemory, pName)) {

                    // While There is Still a Page in the Memory Allocated to the User Program
                    while (inMemory(usedMemory, pName)) {
                        page *pageInMemory = searchPage(usedMemory, pName);
                        allocateFree(&usedMemory, pageInMemory);
                        pagesUsed--;
                        pageDeleteCounter++;
                    }

                    cout << "Program " << pName << " Killed Successfully! (" << pageDeleteCounter << " page(s) freed)" << endl;
                    pageDeleteCounter = 0;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;

                }

                    // Unknown Error
                else {
                    cout << "Unknown Error!" << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                }

                break;
            }

                // Count Fragments
            case 3:{

                if(listLength(freeMemory) == 32){
                    cout << "There is No Fragmentation" << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                    break;
                }

                else {
                    countFragments(usedMemory, freeMemory);
                    cout << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                    break;
                }

            }

                // Output Memory
            case 4: {
                outputMemory(usedMemory, freeMemory);
                cout << endl;
                displayMenu();
                cout << "Option: ";
                cin >> userChoice;
                break;
            }

                // Exit Program
            case 5: {
                exit(0);
                break;
            }

        }

    }

}

void worstFit(){

    // Variables
    int userChoice = 0;
    int programSize;
    int pagesUsed = 0;
    int pageDeleteCounter = 0;
    int largestFreePages = 0;
    string pName;
    struct page *freeMemory = new page;
    struct page *usedMemory = new page;

    // Initializing 32 Pages of Free Memory
    for(int counter = 1; counter <= 32; counter++){
        addPage(freeMemory, "Free");
    }

    // Output User Menu
    cout << "Using the Worst Fit Algorithm" << endl;
    cout << endl;
    displayMenu();
    cout << "Option: ";
    cin >> userChoice;

    while(userChoice != 5) {

        // User Choice Switch
        switch (userChoice) {

            // Unrecognized Input
            default: {
                cout << "Sorry, Unrecognized Input!" << endl;
                cout << endl;
                displayMenu();
                cout << "Option: ";
                cin >> userChoice;
                break;
            }

                // Add Program
            case 1: {

                // If Memory is not Already Full, Allow the Addition of a Program
                if (pagesUsed < 32) {

                    cout << "Enter the Program Name: ";
                    cin >> pName;
                    cout << "Program Size (KB): ";
                    cin >> programSize;
                    programSize = kbtoPages(programSize);

                    // If the Program that Wants to be Added is Within the Memory Quota, Add it.
                    if (pagesUsed + programSize <= 32 && !inMemory(usedMemory, pName)) {

                        // If there is Enough Free Space in the Free Space Linked List, Add Program to it.
                        if (listLength(freeMemory) >= programSize) {

                            // Updating the Number of Pages Used
                            pagesUsed = pagesUsed + programSize;

                            // For Each Page of Memory Used, Delete a Page from the Free Memory List and Add the
                            // Program to the Used Memory List
                            for (int counter = 1; counter <= programSize; counter++) {
                                addPage(usedMemory, pName);
                                deleteEnd(freeMemory);
                            }

                            // Success Output
                            cout << "Program " << pName << " Successfully Added! ("
                                    << programSize << " Page(s) Used)" << endl;
                            displayMenu();
                            cout << "Option: ";
                            cin >> userChoice;

                        }

                            // If there is Not Enough Free Space in the Free Linked List, Query the Used Memory for Free Space
                        else{

                            page *startingBlock = usedLargest(usedMemory, largestFreePages);

                            if(largestFreePages >= programSize){

                                pagesUsed = pagesUsed + programSize;
                                writeToUsed(startingBlock, programSize, pName);

                                // Success Output
                                cout << "Program " << pName << " Successfully Added! ("
                                        << programSize << " Page(s) Used)" << endl;
                                displayMenu();
                                cout << "Option: ";
                                cin >> userChoice;

                            }

                            else if(largestFreePages < programSize){
                                cout << "Error, Not Enough Memory to Add " << pName << "!" << endl;
                                displayMenu();
                                cout << "Option: ";
                                cin >> userChoice;
                            }

                            largestFreePages = 0;

                        }

                    }
                        // If the Program that Wants to be Added Exceeds the Memory Quota, Display Error.
                    else if(pagesUsed + programSize > 32){
                        cout << "Error, Not Enough Memory to Add " << pName << "!" << endl;
                        displayMenu();
                        cout << "Option: ";
                        cin >> userChoice;
                    }

                        // If the Program that Wants to be Added is Already in Memory, Display Error.
                    else if(inMemory(usedMemory, pName)){
                        cout << "Error, Program " << pName << " is Already in Memory!" << endl;
                        displayMenu();
                        cout << "Option: ";
                        cin >> userChoice;
                    }

                        // If Anything Else Happens, Display Unknown Error
                    else{
                        cout << "Unknown Error!" << endl;
                        displayMenu();
                        cout << "Option: ";
                        cin >> userChoice;
                    }

                }

                    // If Memory is Already Full, Display Error
                else {
                    cout << "Not Enough Memory to Add More Programs!" << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                }

                break;

            }

                // Kill Program
            case 2: {

                cout << "Enter the Program Name: ";
                cin >> pName;

                // If the Program is not in Memory
                if (!inMemory(usedMemory, pName)) {
                    cout << "Program " << pName << " is not in Memory!" << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                }

                    // If the Program IS in Memory
                else if (inMemory(usedMemory, pName)) {

                    // While There is Still a Page in the Memory Allocated to the User Program
                    while (inMemory(usedMemory, pName)) {
                        page *pageInMemory = searchPage(usedMemory, pName);
                        allocateFree(&usedMemory, pageInMemory);
                        pagesUsed--;
                        pageDeleteCounter++;
                    }

                    cout << "Program " << pName << " Killed Successfully! (" << pageDeleteCounter << " page(s) freed)" << endl;
                    pageDeleteCounter = 0;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;

                }

                    // Unknown Error
                else {
                    cout << "Unknown Error!" << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                }

                break;
            }

                // Count Fragments
            case 3:{

                if(listLength(freeMemory) == 32){
                    cout << "There is No Fragmentation" << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                    break;
                }

                else {
                    countFragments(usedMemory, freeMemory);
                    cout << endl;
                    displayMenu();
                    cout << "Option: ";
                    cin >> userChoice;
                    break;
                }

            }

                // Output Memory
            case 4: {
                outputMemory(usedMemory, freeMemory);
                cout << endl;
                displayMenu();
                cout << "Option: ";
                cin >> userChoice;
                break;
            }

                // Exit Program
            case 5: {
                exit(0);
                break;
            }

        }
    }

}

void displayMenu(){

    // User Dialog Screen
    cout << endl;
    cout << "1. Add Program" << endl;
    cout << "2. Kill Program" << endl;
    cout << "3. Fragmentation" << endl;
    cout << "4. Print Memory" << endl;
    cout << "5. Exit" << endl;
    cout << endl;

}

void initPage(struct page *head, string programName){

    // Assign Properties of the First Node in the Linked List
    head -> programName = programName;
    head -> nextPage = NULL;

}

void addPage(struct page *head, string programName){

    // Initialize First Page if Not Initialized
    if(head == NULL){
        initPage(head, programName);
        return;
    }

    // Setup the New Page
    page *newPage = new page;
    newPage -> programName = programName;
    newPage -> nextPage = NULL;

    // Set the Pointer to the Beginning of the Linked List
    page *current = head;

    // While Traversing the Linked List
    while(current){

        // If the End of the List is Reached, Append the Page
        if(current -> nextPage == NULL){
            current -> nextPage = newPage;
            return;
        }

        // Grab the Next Page (If not at the End of the Page)
        current = current -> nextPage;
    }

}

bool deletePage(struct page **head, page *pageDelete){

    page *current = *head;

    // If a Page Matches the Desired Page to Delete in the Main (head) Linked List, Delete it
    if(pageDelete == *head){
        *head = current -> nextPage;
        delete(pageDelete);
        return true;
    }

    // Reassign nextPage for Adjacent Node to Route Over Deleted Node
    while(current){

        if(current -> nextPage == pageDelete){
            current -> nextPage = pageDelete -> nextPage;
            delete(pageDelete);
            return true;
        }

        current = current -> nextPage;

    }

    return false;

}

void deleteEnd(struct page *head){

    // Deletes the Last Node in Linked List (If the head is the Only Node)
    if (head -> nextPage == NULL) {
        delete head;
        head = NULL;
    }

    // Deletes the Last Node in the LInked List
    else {
        page *nextToEnd = head;
        page *end = head -> nextPage;

        while (end->nextPage != NULL) {
            nextToEnd = end;
            end = end->nextPage;
        }

        delete end;
        nextToEnd->nextPage = NULL;
    }

}

void outputMemory(struct page *head, struct page *head2){

    int counter = 0;

    // Sets a Pointer to the Beginning of the Linked List
    page *usedMemory = head;
    page *freeMemory = head2;

    usedMemory = usedMemory->nextPage;
    freeMemory = freeMemory->nextPage;

    // Print the Program Name Allocated to Each Page in the usedMemory List
    while(usedMemory){

        if(counter % 9 == 0){
            cout << endl;
            counter++;
        }

        else {
            cout << setw(10) << usedMemory->programName;
            usedMemory = usedMemory->nextPage;
            counter++;
        }

    }

    // Print the Program Name Allocated to Each Page in the freeMemory List
    while(freeMemory){

        if(counter % 9 == 0){
            cout << endl;
            counter++;
        }

        else {
            cout << setw(10) << freeMemory->programName;
            freeMemory = freeMemory->nextPage;
            counter++;
        }

    }

}

void allocateFree(struct page **head, page *pagetoFree){

    pagetoFree -> programName = "Free";

}

struct page *searchPage(struct page *head, string programName){

    // Set the Pointer at the Beginning of the Linked List
    page *current = head;

    // Traverse the Linked List and Return the First Instance of Selected Paged Program
    while(current){
        if(current -> programName == programName) return current;
        current = current -> nextPage;
    }

    // If the Program could not be Found in Memory, Throw an Exception
    throw 1;

}

int kbtoPages(int kb){

    // Number of Pages Storage
    int pages;

    // KB to Pages Conversion (1 Page = 4KB)
    pages = kb / 4;

    // Add Additional Page if there are KB Remaining
    if(kb % 4 > 0){
        pages++;
    }

    // Return the Number of Pages
    return pages;

}

int listLength(struct page *head){

    // Stores Count
    int counter = 0;

    // Traverse Each Node of the LInked List
    while (head != NULL) {
        /* Update the counter */
        counter++;
        /* move along to the next node */
        head = head -> nextPage;
    }

    // Count Excluding Blank Node
    counter = counter - 1;

    // Returns COunt
    return counter;

}

bool inMemory(struct page *head, string programName){

    // Set the Pointer at the Beginning of the Linked List
    page *current = head;

    // Traverse the Linked List and Return the First Instance of Selected Paged Program
    while(current){
        if(current -> programName == programName) return true;
        current = current -> nextPage;
    }

    return false;

}

struct page *usedSmallest(struct page *head, int &smlstFree, int minimumSize){

    // Smallest Number of Consecutive Free Pages Storage
    int smallestNumberFreePages = 32;
    int freePageCounter = 0;
    struct page *theSmallestBlock = NULL;
    struct page *smallestBlockStart = NULL;

    // Set the Pointer at the First Element of the Linked List
    page *current = head;
    current = current -> nextPage;

    // Traverse the Linked List
    while(current){

        if(current -> programName == "Free" && current -> nextPage != NULL){
            if(smallestBlockStart == NULL){
                smallestBlockStart = current;
            }

            freePageCounter++;
        }

        else if(current -> programName == "Free" && current -> nextPage == NULL){
            if(smallestBlockStart == NULL){
                smallestBlockStart = current;
            }

            freePageCounter++;

            if(freePageCounter <= smallestNumberFreePages && freePageCounter >= minimumSize){
                theSmallestBlock = smallestBlockStart;
                smallestBlockStart = NULL;
                smallestNumberFreePages = freePageCounter;
                freePageCounter = 0;
            }

        }

        else if(current -> programName != "Free" && current ->nextPage == NULL){
            if(freePageCounter <= smallestNumberFreePages && freePageCounter >= minimumSize && freePageCounter != 0){
                theSmallestBlock = smallestBlockStart;
                smallestBlockStart = NULL;
                smallestNumberFreePages = freePageCounter;
                freePageCounter = 0;
            }

            else if(freePageCounter == 0 && (smallestNumberFreePages == 0 || smallestNumberFreePages == 32)){
                smallestNumberFreePages = 0;
            }

        }

        else if(current -> programName != "Free" && current ->nextPage != NULL){
            if(freePageCounter <= smallestNumberFreePages && freePageCounter >= minimumSize && freePageCounter != 0){
                theSmallestBlock = smallestBlockStart;
                smallestBlockStart = NULL;
                smallestNumberFreePages = freePageCounter;
                freePageCounter = 0;
            }
        }

        current = current -> nextPage;

    }

    smlstFree = smallestNumberFreePages;
    cout << "Number of Pages Available: " << smlstFree << endl;

    return theSmallestBlock;

}

struct page *usedLargest(struct page *head, int &lrgstFree){

    // Largest Number of Consecutive Free Pages Storage
    int largestNumberFreePages = 0;             // Stores the Largest Number of Consecutive Free Pages in Memory
    int freePageCounter = 0;                    // Stores the Count for Each Number of Consecutive Free Pages in Memory
    struct page *theLargestBlock = NULL;        // Stores the Largest Free Memory Block to Return
    struct page *largestBlockStart = NULL;      // Stores the Temporary Largest Free Memory Block

    // Set the Pointer at the First Element of the Linked List
    page *current = head;
    current = current -> nextPage;

    // Traverse the Linked List
    while(current){

        // If a Free Page is Found, Declare the Value for the largestBlockStart (if not already done) as the
        // Current Position in the Linked List and Increase the Free Page Counter by 1
        if(current -> programName == "Free" && current -> nextPage != NULL){

            if(largestBlockStart == NULL){
                largestBlockStart = current;
            }

            freePageCounter++;
        }

        // If this Node is the Last Node in the LInked List, Declare the Value for the largestBlockStart
        // (if not already done), Increase the Free Page Counter by 1, and if the Current Page Count is
        // Greater than the largestNumberFreePages, Assign Variables that Show that This is the Largest Area
        // in Memory of Free Blocks
        else if(current -> programName == "Free" && current -> nextPage == NULL){

            if(largestBlockStart == NULL){
                largestBlockStart = current;
            }

            freePageCounter++;

            if(freePageCounter > largestNumberFreePages) {
                theLargestBlock = largestBlockStart;
                largestBlockStart = NULL;
                largestNumberFreePages = freePageCounter;
                freePageCounter = 0;
            }

        }

        // If A Non-Free Page is Found Verify if it is the Largest Free Memory Block, and if it is,
        // Assign the Proper Variables
        else if(current -> programName != "Free"){
            if(freePageCounter > largestNumberFreePages) {
                theLargestBlock = largestBlockStart;
                largestBlockStart = NULL;
                largestNumberFreePages = freePageCounter;
                freePageCounter = 0;
            }
        }

        // Grabs the Next Node in the Linked List
        current = current -> nextPage;

    }

    // References the Number of Pages that the Largest Free Memory Block Had.
    lrgstFree = largestNumberFreePages;

    // Returns the Largest Free Memory Block Starting Node
    return theLargestBlock;

}

void writeToUsed(page *startNode, int numPages, string pgrmName){

    // Grab the Starting Node
    page *current = startNode;

    // For the Next X Pages, Replace the 'Free' Program Name with One That was Selected
    for(int counter = 1; counter <= numPages; counter++){

        if(current -> programName == "Free"){
            current -> programName = pgrmName;
        }

        // Error in Case Program Attempts to Overwrite a Program Name That is Not Free
        else if(current -> programName != "Free"){
            cout << "ERROR! Attempted Overwrite of Program in Memory!";
            return;
        }

        // Grabs Next Node
        current = current -> nextPage;

    }

}

/* LEGACY CODE
void countFragments(struct page *head){

    page *current = head;
    int numberOfFragments = 0;
    string placeHolder;
    bool startingFragment = false;

    current = current->nextPage;
    placeHolder = current->programName;

    if (placeHolder == "Free") {
        startingFragment = true;
    }

    while (current) {

        if (current->programName == placeHolder && current->nextPage != NULL) {}

        else if (current->programName != placeHolder && current->nextPage != NULL) {

            if (current->programName != "Free" && placeHolder == "Free") {
                numberOfFragments++;
                placeHolder = current->programName;
            }

            else if (current->programName != "Free" && placeHolder != "Free") {
                placeHolder = current->programName;
            }

            else if (current->programName == "Free" && placeHolder != "Free") {
                placeHolder = current->programName;
            }

        }

        else if(current->nextPage == NULL && listLength(head) != 32 && current->programName != "Free" && !startingFragment){
            numberOfFragments++;
        }


        else if(current->nextPage == NULL && listLength(head) == 32 && !startingFragment){
            numberOfFragments++;
        }

        current = current->nextPage;

    }

    cout << "There are " << numberOfFragments << " fragment(s)" << endl;

}
*/

void countFragments(struct page *head, struct page *head2){

    // Variable Initialization and Storage
    int numberOfFragments = 0;      // Stores the Number of Fragments Counted
    string placeHolder;             // Stores the last value in List for Comparison to See if a Fragment Occured
    page *mergedPage = new page;    // Stores the Merged usedMemory and freeMemory Linked Lists
    bool startingFragment = false;  // Stores if the Merged List Beings with a Fragment

    // Sets Pointer to the Beginning of the First List
    page *current = head;
    current = current->nextPage;

    // Iterates Through the First Linked List and Adds Each Node to the Merged Linked List
    while (current) {
        addPage(mergedPage, current -> programName);
        current = current->nextPage;
    }

    // Sets Pointer to the Beginning of the Second Linked List
    current = head2;
    current = current->nextPage;

    // Iterates Through the Second Linked List and Adds Each Node to the Merged Linked List
    while (current) {
        addPage(mergedPage, current -> programName);
        current = current->nextPage;
    }

    // Sets Pointer to the Beginning of the Merged Linked List and Assigns the Placeholder to the First Value
    current = mergedPage;
    current = current -> nextPage;
    placeHolder = current->programName;

    // If the First Placeholder Begins with a Free Value, then it contains a Starting Fragment
    if (placeHolder == "Free") {
        startingFragment = true;
    }

    // Iterates Through the Merged Linked List and Determines When a Fragment Occurs
    while(current){

        // If the Current Porgram Name and the Last Program Name are the Same, Nothing Needs to be Done
        if (current->programName == placeHolder && current->nextPage != NULL) {}

        // If the Current Porgram Name and the Last Program Name are NOT the Same
        else if (current->programName != placeHolder && current->nextPage != NULL) {

            // If the Value Switches from a Free to a non-Free Value, Increase the Number of
            // Fragments by 1 and Reassign the Placeholder Value to the Current Program Name
            if (current->programName != "Free" && placeHolder == "Free") {
                numberOfFragments++;
                placeHolder = current->programName;
            }

            // If the Value Switches to Another Program's Name, a Fragment has NOT Occured
            else if (current->programName != "Free" && placeHolder != "Free") {
                placeHolder = current->programName;
            }

            // If the Value Switches to Free From Another Program's Name, a Fragment has NOT Occured
            else if (current->programName == "Free" && placeHolder != "Free") {
                placeHolder = current->programName;
            }

        }

        // Grab the Next Node
        current = current->nextPage;

    }

    // If Programs are in the First Slot of Memory, Increment the Number of Fragments by 1
    // because it has not been Accounted for
    if(!startingFragment){
        numberOfFragments++;
    }

    // Output Number of Fragments
    cout << "There are " << numberOfFragments << " fragment(s)" << endl;

}
