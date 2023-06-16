#ifndef AKINATOR_H
#define AKINATOR_H

#include "akinator.hpp"

using namespace std;

struct Node;
shared_ptr<Node> loadTreeFromFile(const string& filename);
void saveTreeToFile(const string& filename, shared_ptr<Node> root, shared_ptr<Node> targetNode, shared_ptr<Node> newNode, shared_ptr<Node> newAns);
shared_ptr<Node> addNewQuestion(shared_ptr<Node> currentNode);
void playGame(shared_ptr<Node> currentNode, shared_ptr<Node> root, const string& filename);

#endif // AKINATOR_H
