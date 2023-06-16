#include <iostream>
#include <fstream>
#include <string>
#include <memory>
using namespace std;

/*!
    *@struct Node
    *@param data параметр, хранящий текст внутри узла
    *@param yes ветка дерева, хранящая узел с ответом "да"
    *@param no ветку дерева, хранящаяя узел с ответом "нет"
    *@param parent родительский узел для текущего узла
*/
struct Node {
    string data;
    shared_ptr<Node> yes;
    shared_ptr<Node> no;
    shared_ptr<Node> parent;
};

/*!
    *@brief функция создания нового узла дерева
    *@param data данные, содержащиеся в узле
    *@param yes ветка дерева, хранящая узел с ответом "да"
    *@param no ветку дерева, хранящаяя узел с ответом "нет"
    *@param parent родительский узел для текущего узла
*/
shared_ptr<Node> createNode(const string& data, shared_ptr<Node> yes = nullptr, shared_ptr<Node> no = nullptr, shared_ptr<Node> parent = nullptr) {
    return make_shared<Node>(Node{ data, yes, no, parent });
}

/*!
    *@brief функция, заргужающая элементы дерева из файла
    *@param filename название файла в библиотеке
*/
shared_ptr<Node> loadTreeFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return nullptr;
    }

    string line;
    getline(file, line);  // Read the first line

    shared_ptr<Node> root = createNode(line.substr(1));
    shared_ptr<Node> currentNode = root;
    shared_ptr<Node> parent = nullptr;

    while (getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        if (line[0] == '?') {
            shared_ptr<Node> newNode = createNode(line.substr(1));
            if (currentNode != nullptr && currentNode->yes == nullptr) {    //если нету правой ветки то идём направо
                currentNode->yes = newNode;
                newNode->parent = currentNode;
                currentNode = newNode;
            } else if (currentNode != nullptr && currentNode->yes != nullptr && currentNode->no == nullptr) {
                currentNode->no = newNode;      //если есть правая ветка идём налево
                newNode->parent = currentNode;
                currentNode = newNode;
            } else if (currentNode != nullptr && currentNode->yes != nullptr && currentNode->no != nullptr) {
                if (currentNode == root && currentNode->no && nullptr){
                    break;  //останавливаем если вернулись к корню
                } else{
                    while (currentNode->no != nullptr){     // если не корень, то возвращаемся к родительскому узлу 
                        currentNode = currentNode->parent;  // пока не найдём не заполненный узел
                    }
                    currentNode->no = newNode;
                    newNode->parent = currentNode;
                    currentNode=newNode;
                }
            } else {        //если не currentNode = nullptr то это корень
                root = newNode;
                parent = root;
                currentNode = newNode;
            }
        } else {    //если начинается не с вопроса
            if (currentNode->yes == nullptr) {  //если нет правой ветки добавляем
                currentNode->yes = createNode(line);
                currentNode->yes->parent = currentNode;
            } else if (currentNode->yes != nullptr && currentNode->no == nullptr) {     //если есть правая но нет левой
                currentNode->no = createNode(line);                                     //добавляем левый
                currentNode->no->parent = currentNode;
            } else if (currentNode->yes != nullptr && currentNode->no != nullptr){
                while (currentNode->no != nullptr){     //если есть и левый и правый то возвращаемся к родителю
                        currentNode = currentNode->parent;
                }
                currentNode->no = createNode(line);
                currentNode->no->parent = currentNode;
            }
        }
    }
    file.close();
    return root;
}

/*!
    *@brief сохраняет изменённое дерево в файл
    *@param filename имя файла, в который сохраняется дерево
    *@param root корень дерева
    *@param targetNode узел, над которым происходит вставка нового вопроса и ответа
    *@param newNode новый узел с вопросом
    *@param newAns новый узел с ответом
*/
void saveTreeToFile(const string& filename, shared_ptr<Node> root, shared_ptr<Node> targetNode, shared_ptr<Node> newNode, shared_ptr<Node> newAns) {
    ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        cerr << "Failed to open the file: " << filename << endl;
        return;
    }

    string fileContents((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>()); // Чтение содержимого файла в память
    inputFile.close();

    size_t targetPos = fileContents.find(targetNode->data); // Поиск целевого узла в тексте файла
    if (targetPos == string::npos) {
        cerr << "Target node not found in the file." << endl;
        return;
    }

    string newNodeQ = "?" + newNode->data+"\n";  // Вставка нового узла после целевого узла
    string newNodeA = "\n" + newAns->data;
    fileContents.insert(targetPos + targetNode->data.length(), newNodeA);
    fileContents.insert(targetPos + targetNode->data.length()+1, newNodeQ);

    ofstream outputFile(filename); // Запись обновленного содержимого в файл
    if (!outputFile.is_open()) {
        cerr << "Failed to open the file for writing: " << filename << endl;
        return;
    }

    outputFile << fileContents;
    outputFile.close();
}