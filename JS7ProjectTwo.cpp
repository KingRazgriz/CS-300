﻿//============================================================================
// Name        : JS7ProjectTwo.cpp
// Author      : Jamar Sampson, October 19, 2024
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Week Seven Project Two Final
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

using namespace std;

// Displays main menu optins to the end user.
void DisplayMenu() {
    cout << "1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit Program." << endl << endl;
    cout << "Please Select An Above Option. ";
}

// Construct to store information.
struct Course {
    string courseId;
    string courseName;
    vector<string> prerequisites;
};

// Node structure for the binary search tree.
struct Node {
    Course course;
    Node* left;
    Node* right;

    // Constructor initializes node pointers to null.
    Node() : left(nullptr), right(nullptr) {}

    // Constructor initializing with a given course
    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// Binary search tree class to manage the courses.
class BinarySearchTree {
private:
    void FreeNode(Node* node);

public:
    Node* root;
    BinarySearchTree();
    ~BinarySearchTree();
    void Insert(Node* node);
    void Search(const string& courseId) const;
    void PrintCourses(Node* node) const;
};

// Constructor initializes an empty tree.
BinarySearchTree::BinarySearchTree() : root(nullptr) {}

// Destructor deallocates all nodes in the tree.
BinarySearchTree::~BinarySearchTree() {
    FreeNode(root);
}

// Recursively reallocates memory for nodes in the tree.
void BinarySearchTree::FreeNode(Node* node) {
    if (node) {
        FreeNode(node->left);
        FreeNode(node->right);
        delete node;
    }
}

// Inserts a course into the correct position in the tree.
void BinarySearchTree::Insert(Node* node) {
    if (!root) {
        root = node;
        return;
    }

    Node* current = root;
    while (true) {
        if (node->course.courseId < current->course.courseId) {
            if (!current->left) {
                current->left = node;
                break;
            }
            current = current->left;
        }
        else {
            if (!current->right) {
                current->right = node;
                break;
            }
            current = current->right;
        }
    }
}

// Search out specific courses by their ID and display details.
void BinarySearchTree::Search(const string& courseId) const {
    Node* currentNode = root;

    while (currentNode) {
        if (currentNode->course.courseId == courseId) {
            cout << currentNode->course.courseId << ", " << currentNode->course.courseName << endl;
            cout << "Prerequisites: ";
            for (size_t i = 0; i < currentNode->course.prerequisites.size(); ++i) {
                cout << currentNode->course.prerequisites[i];
                if (i < currentNode->course.prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
            return;
        }
        currentNode = (courseId < currentNode->course.courseId) ? currentNode->left : currentNode->right;
    }

    cout << "Course " << courseId << " not found." << endl;
}

// In-order traversal to print courses in alphabetical order
void BinarySearchTree::PrintCourses(Node* node) const {
    if (!node) return;

    PrintCourses(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    PrintCourses(node->right);
}

// Load course data from a file and insert into the binary search tree
void LoadCourses(const string& filename, BinarySearchTree* bst) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Could not open file " << filename << "." << endl;
        return;
    }

    cout << "File loaded successfully." << endl;
    string line, word;
    while (getline(file, line)) {
        stringstream str(line);
        Node* node = new Node();

        getline(str, node->course.courseId, ',');
        getline(str, node->course.courseName, ',');

        while (getline(str, word, ',')) {
            node->course.prerequisites.push_back(word);
        }

        bst->Insert(node);
    }
}

// Main function to handle user interaction and menu-driven program flow
int main() {
    BinarySearchTree* bst = new BinarySearchTree();
    int userInput = 0;
    string fileChoice, courseChoice;

    cout << "Welcome to the course planner." << endl << endl;

    while (userInput != 9) {
        DisplayMenu();
        cin >> userInput;

        switch (userInput) {
        case 1:
            cout << "Enter the filename to load (default is CS300 Courses.csv): ";
            cin >> fileChoice;
            LoadCourses(fileChoice, bst);
            break;
        case 2:
            cout << "******************************************" << endl;
            cout << "COURSE ID |  COURSE NAME" << endl;
            cout << "******************************************" << endl;
            bst->PrintCourses(bst->root);
            break;
        case 3:
            cout << "Enter the course ID you want to know about: ";
            cin >> courseChoice;
            transform(courseChoice.begin(), courseChoice.end(), courseChoice.begin(), ::toupper);
            bst->Search(courseChoice);
            break;
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            break;
        default:
            cout << userInput << " is not a valid option." << endl;
            break;
        }
        cout << endl;
    }

    delete bst;
    return 0;
}
