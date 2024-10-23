//============================================================================
// Name        : HashTable.cpp
// Author      : Jamar Sampson, Sept 28, 2025
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Lab 4-2 Hash Table
//============================================================================

#include <algorithm>
#include <climits>
#include <iostream>
#include <string> // atoi
#include <time.h>
#include <Windows.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

const unsigned int DEFAULT_SIZE = 179;
const int GLOBAL_SLEEP_TIME = 5000;

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
    string bidId; // unique identifier
    string title;
    string fund;
    double amount;
    Bid() {
        amount = 0.0;
    }
};

//============================================================================
// Hash Table class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a hash table with chaining.
 */
class HashTable {

private:
    // Define structures to hold bids
    struct Node {
        Bid bid;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a bid
        Node(Bid aBid) : Node() {
            bid = aBid;
        }

        // initialize with a bid and a key
        Node(Bid aBid, unsigned int aKey) : Node(aBid) {
            key = aKey;
        }
    };

    vector<Node> nodes;

    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Bid bid);
    void PrintAll();
    void Remove(string bidId);
    Bid Search(string bidId);
};

/**
 * Default constructor
 */
HashTable::HashTable() {
    // FIXME (1): Initialize the structures used to hold bids

    // Initalize node structure by resizing tableSize
    nodes.resize(tableSize);

}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    // resize nodes size

    this->tableSize = size;
    nodes.resize(size);
}


/**
 * Destructor
 */
HashTable::~HashTable() {
    // FIXME (2): Implement logic to free storage when class is destroyed

    // erase nodes beginning
    nodes.erase(nodes.begin());

}

/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {
    // FIXME (3): Implement logic to calculate a hash value
    // return key tableSize

    //use the modulo hash from Zybooks 5.7
    return key % tableSize;
}

/**
 * Insert a bid
 *
 * @param bid The bid to insert
 */
void HashTable::Insert(Bid bid) {
    // FIXME (5): Implement logic to insert a bid
    // create the key for the given bid
    // retrieve node using key
    // if no entry found for the key
        // assign this node to the key position
    // else if node is not used
         // assing old node key to UNIT_MAX, set to key, set old node to bid and old node next to null pointer
    // else find the next open node
            // add new newNode to end

    unsigned int tempKey = hash(stoi(bid.bidId));


    //look to see if the node exists
    //If the key is set to the default constructor overwrite
    if (nodes.at(tempKey).key == UINT_MAX) {

        //make the new Node
        Node newNode = Node(bid, tempKey);

        //add the item
        nodes.at(tempKey) = newNode;
    }

    else {//now we need to get to the bottom of the  linked list

        //make a cursor node as an iterator and point at the hash location
        Node* currNode = &nodes.at(tempKey);

        //make the new node pointer
        Node* newNode = new Node(bid, tempKey);

        // scan the linked list to find the last position
        while (currNode->next != NULL) {

            currNode = currNode->next;

        }

        //now append the new node
        currNode->next = newNode;

    }

}

/**
 * Print all bids
 */
void HashTable::PrintAll() {
    // FIXME (6): Implement logic to print all bids
    // for node begin to end iterate
    //   if key not equal to UINT_MAx
            // output key, bidID, title, amount and fund
            // node is equal to next iter
            // while node not equal to nullptr
               // output key, bidID, title, amount and fund
               // node is equal to next node 

    int j = 0;//small logical test

    for (unsigned int i = 0; i < tableSize; i++) {

        Node* currNode = &nodes.at(i);

        if (currNode->key != UINT_MAX) {

            j++;

            cout << currNode->key << ": " << currNode->bid.bidId << " | " << currNode->bid.amount
                << " | " << currNode->bid.fund << endl;

            while (currNode->next != nullptr) {

                j++;

                currNode = currNode->next;

                cout << currNode->key << ": " << currNode->bid.bidId << " | " << currNode->bid.amount
                    << " | " << currNode->bid.fund << endl;

            }
        }
    }

    //cout << j << endl;;
    return;

}

/**
 * Remove a bid
 *
 * @param bidId The bid id to search for
 */
void HashTable::Remove(string bidId) {
    // FIXME (7): Implement logic to remove a bid
    // set key equal to hash atoi bidID cstring
    // erase node begin and key

    //see currNode to the head node
    Node* cursor = &nodes.at(hash(stoi(bidId)));
    Node* tempNode = NULL;

    //see if there is a chain; probably will be given 12000+ receords
    //if there is a chain adopt that methodology of scan and shift
    //if there is no chain just check the head node

    if (cursor->next != nullptr) { //if the bucket head is not null there is a chain

        //special case; matches the head of the chain
        if (cursor->bid.bidId == bidId) {

            //point tempNode at the next node
            //copy +1 node to headnode
            //delete +1 node
            tempNode = cursor->next;
            cursor->key = tempNode->key;
            cursor->bid = tempNode->bid;
            cursor->next = tempNode->next;

            free(tempNode);
            return;
        }
        else { //we now need to scan the chain for a match

            while (cursor != NULL && cursor->bid.bidId != bidId) {

                tempNode = cursor; //set temp to the current currsor
                cursor = cursor->next; //move cursor ahead
            }

            if (cursor == NULL) {//scanned tjhe whole list and didn't find the key

                cout << " Key not found." << endl;
                return;
            }

            tempNode->next = cursor->next;

            delete cursor;

        }

    }
    else {

        if (cursor->bid.bidId == bidId) { //match the head node with no chaining

            //set the bucket back to uninitiated
            Node newNode = Node();

            cursor->key = newNode.key;
            cursor->bid = newNode.bid;
            cursor->next = nullptr;

            return;

        }
        else { //no chain and no match at the head; maybe previously deleted or never existed

            cout << "No Bid exists." << endl;
        }
    }

}

/**
 * Search for the specified bidId
 *
 * @param bidId The bid id to search for
 */
Bid HashTable::Search(string bidId) {
    Bid bid;

    // FIXME (8): Implement logic to search for and return a bid

    // create the key for the given bid
    // if entry found for the key
         //return node bid

    // if no entry found for the key
      // return bid
    // while node not equal to nullptr
        // if the current node matches, return it
        //node is equal to next node

        //see currNode to the head node
    Node* cursor = &nodes.at(hash(stoi(bidId)));

    while (cursor != NULL) {

        if (cursor->bid.bidId == bidId) {

            return cursor->bid;

        }

        cursor = cursor->next;

    }

    return bid;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
    cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
        << bid.fund << endl;
    return;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
void loadBids(string csvPath, HashTable* hashTable) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    //for (auto const& c : header) {
    //    cout << c << " | ";
    //}
    //cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Bid bid;
            bid.bidId = file[i][1];
            bid.title = file[i][0];
            bid.fund = file[i][8];
            bid.amount = strToDouble(file[i][4], '$');

            //cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

            // push this bid to the end
            hashTable->Insert(bid);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, bidKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        bidKey = "98109";
        break;
    case 3:
        csvPath = argv[1];
        bidKey = argv[2];
        break;
    default:
        csvPath = "U:/CS 300 Hash Table Assignment Student Files/eBid_Monthly_Sales_Dec_2016.csv";
        bidKey = "98010";//no collision
        //bidKey = "98011";//collision head
        //bidKey = "98190";//collision tail
    }

    // Define a timer variable
    clock_t ticks;

    // Define a hash table to hold all the bids
    HashTable* bidTable = new HashTable();

    Bid bid;

    int choice = 0;
    string anyKey = " ";
    bool goodInput;

    while (choice != 9) {
        cout << "Menu:" << endl;
        cout << "  1. Load Bids" << endl;
        cout << "  2. Display All Bids" << endl;
        cout << "  3. Find Bid" << endl;
        cout << "  4. Remove Bid" << endl;
        cout << "  9. Exit" << endl;
        cout << "Enter choice: ";

        try {
            cin >> choice;

            if ((choice > 0 && choice < 5) || (choice == 9)) {// limit the user menu inputs to good values
                goodInput = true;
            }
            else {//throw error for catch
                goodInput = false;
                throw 1;
            }

            switch (choice) {
            case 1:
                //bidTable = new HashTable(); //comment out to have many many collision upon reload

                // Initialize a timer variable before loading bids
                ticks = clock();

                // Complete the method call to load the bids
                loadBids(csvPath, bidTable);

                // Calculate elapsed time and display result
                ticks = clock() - ticks; // current clock ticks minus starting clock ticks
                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 2:
                bidTable->PrintAll();

                cout << "\n Enter \'y\' to continue..." << endl;

                cin >> anyKey;

                break;

            case 3:
                ticks = clock();

                bid = bidTable->Search(bidKey);

                ticks = clock() - ticks; // current clock ticks minus starting clock ticks

                if (!bid.bidId.empty()) {
                    displayBid(bid);
                }
                else {
                    cout << "Bid Id " << bidKey << " not found." << endl;
                }

                cout << "time: " << ticks << " clock ticks" << endl;
                cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 4:
                bidTable->Remove(bidKey);

                Sleep(GLOBAL_SLEEP_TIME);

                break;

            case 9:

                break;

            default:

                throw 2;
            }
        }

        catch (int err) {

            std::cout << "\nPlease check your input." << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }

        //need to clear the cin operator of extra input, e.g., 9 9, or any errors generated by bad input, e.g., 'a'
        cin.clear();
        cin.ignore();

        //clear the consolse to redraw a fresh menu
        system("cls");
    }

    cout << "Good bye." << endl;

    Sleep(GLOBAL_SLEEP_TIME);

    return 0;
}
