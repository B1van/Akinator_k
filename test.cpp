#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <sstream>
#include <stdio.h>
#include "akinator.h"
#include "doctest.h"

shared_ptr<Node> loadTreeFromFile(const string& filename);
void saveTreeToFile(const string& filename, shared_ptr<Node> root, shared_ptr<Node> targetNode, shared_ptr<Node> newNode, shared_ptr<Node> newAns);
shared_ptr<Node> addNewQuestion(shared_ptr<Node> currentNode);

TEST_CASE("createNode creates a new node with a data and null pointers for children and parent") {
    std::string data = "Is this a test data?";
    shared_ptr<Node> newNode = createNode(data);
    CHECK(newNode != nullptr);
    CHECK(newNode->data == data);
    CHECK(newNode->yes == nullptr);
    CHECK(newNode->no == nullptr);
    CHECK(newNode->parent == nullptr);
}

TEST_CASE("loading tree from file with valid path  - Returns Root Node"){
    ofstream testFile("tree.txt");
    testFile<<"?Is it wrong"<<endl;
    shared_ptr<Node> root = loadTreeFromFile("tree.txt");
    REQUIRE(root != nullptr);
    testFile.close();
    remove("tree.txt");
}

TEST_CASE("saveTreeToFile saves the updated decision tree to a file") {
    // Create the tree to save
    shared_ptr<Node> root = createNode("Is it an animal?");
    root->yes = createNode("Does it have fur?");
    root->yes->yes = createNode("It's a dog.");
    root->yes->no = createNode("It's a cat.");
    root->no = createNode("Does it have wings?");
    root->no->yes = createNode("It's a bird.");
    root->no->no = createNode("It's a reptile.");
    shared_ptr<Node> targetNode = root->no;
    shared_ptr<Node> newNode = createNode("Does it swim?");
    shared_ptr<Node> newAns = createNode("It's a penguin.");

    // Create the input file and save the tree
    ofstream testFile("test_tree.txt");
    testFile << "?Is it an animal?\n?Does it have fur?\nIt's a dog.\nIt's a cat.\n?Does it have wings?\nIt's a bird.\nIt's a reptile.";
    testFile.close();
    
    // Save the updated tree to the file
    saveTreeToFile("test_tree.txt", root, targetNode, newNode, newAns);

    // Load the updated tree from the file
    shared_ptr<Node> updatedTree = loadTreeFromFile("test_tree.txt");
    REQUIRE(updatedTree != nullptr);

    // Check that the new node is added after the target node and linked up correctly
    REQUIRE(updatedTree->no->yes->yes->yes == nullptr);
    REQUIRE(updatedTree->no->yes->no->yes == nullptr);
    REQUIRE(updatedTree->no->yes->data == "Does it swim?");
    REQUIRE(updatedTree->no->yes->parent == updatedTree->no);
    REQUIRE(updatedTree->no->yes->yes->data == "It's a penguin.");
    REQUIRE(updatedTree->no->yes->yes->parent == updatedTree->no->yes);
    REQUIRE(updatedTree->no->yes->no->data == "It's a bird.");
    REQUIRE(updatedTree->no->yes->no->parent == updatedTree->no->yes);

    remove("test_tree.txt");
}