#include "akinator.h"

/*!
    *@brief добавляет новый вопрос и ответ на него  в дерево
    *@param currentNode текущий узел дерева
*/
shared_ptr<Node> addNewQuestion(shared_ptr<Node> currentNode) {
    string newQuestion, newAnswer; 
    cout << "I don't know what is it. What do you thinking about:"<<endl;
    cin.ignore();
    getline(cin, newAnswer);
    if (newAnswer == ""){
        cout<<"Invalid input"<<endl;
        return nullptr;
    }
    cout << "Enter the new question for " << newAnswer << " that could be answered 'yes': ";
    getline(cin, newQuestion);
    if (newQuestion == ""){
        cout<<"Invalid input"<<endl;
        return nullptr;
    }
    shared_ptr<Node> newQuestionNode = createNode(newQuestion);
    shared_ptr<Node> newAnswerNode = createNode(newAnswer);

    newQuestionNode->no = currentNode->no;
    currentNode->yes = newQuestionNode;
    newQuestionNode->yes = newAnswerNode;
    

    return newQuestionNode;
}

/*!
    *@brief функция игры, отвечающая за переход между узлами дерева и сохранение новых вопрос в дерево
    *@param currentNode текущий узел
    *@param root корень дерево
    *@param filename название файла, из которого берется дерево
*/
void playGame(shared_ptr<Node> currentNode, shared_ptr<Node> root, const string& filename){
    string choice;
    cout << currentNode->data << " (y/n): ";
    cin >> choice;
    shared_ptr<Node> parent = nullptr;

    if (choice =="y") {
        if (currentNode->yes != nullptr) {
            currentNode->parent = currentNode;
            playGame(currentNode->yes, root, filename);
        } else {
            cout << "I guessed it! Is it correct? (y/n): ";
            cin >> choice;
            if (choice == "y") {
                cout << "Great! I win." << endl;
            } else {
                shared_ptr<Node> newQuestionNode = addNewQuestion(currentNode);
                cout << "Thanks for the game! I learned something new." << endl;
                saveTreeToFile(filename, root,currentNode->parent,newQuestionNode,newQuestionNode->yes);
            }
        }
    } else if (choice == "n"){
        if (currentNode->no != nullptr) {
            currentNode->parent = currentNode;
            playGame(currentNode->no,root,filename);
        } else {
            shared_ptr<Node> newQuestionNode = addNewQuestion(currentNode);
            cout << "Thanks for the game! I learned something new." << endl;
            saveTreeToFile(filename, root, currentNode->parent->yes, newQuestionNode, newQuestionNode->yes);
        }
    } else if (choice == "back"){
        currentNode = currentNode->parent;
        playGame(currentNode,root,filename);
    } else {
        cout<<"incorrect input. Print 'y' or 'n' to answer"<<endl;
        playGame(currentNode, root, filename);
    }
}

/*!
    *@brief главная функция, отвечающая последовательный вызов всех функций
    *@return 0
*/
int main() {
    cout << "Welcome to the Akinator game!" << endl;
    cout << "Think of an object and I'll try to guess what it is.\nPrint 'back' if ypu want to back to the previous question" << endl;

    char choice;
    do {
        cout << "select a gamemode: \n[1] Counrties \n[2] Characters \n";
        char gamemode;
        cin >>gamemode;
        if (gamemode == '1'){
            shared_ptr<Node> root = loadTreeFromFile("countries.txt");
            playGame(root, root, "countries.txt");
        }else if (gamemode == '2'){
            shared_ptr<Node> root = loadTreeFromFile("characters.txt");
            playGame(root, root, "characters.txt");
        } else{
            cout << "Incorect input\n";
        }
        cout << "Do you want to play again? (y/n): ";
        cin >> choice;
    } while (choice == 'y');
    return 0;
}