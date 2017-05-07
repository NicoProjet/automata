#include "Graph.h"

/*
++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++      Classe Graph     ++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++
*/
std::size_t Graph::Node::numberOfStates = 0;
Graph::Graph()
{
    //ctor
}

Graph::Graph(std::string fileName)
{
    std::ifstream file(fileName);
    std::string line, valuesInLine;
    char value;
    Node *newNode, *origin, *target;
    Edge *newEdge;
    std::size_t valueBegin, valueEnd;
    int counter = 0;
    for (int i=0;i<HEAD_SIZE;i++){std::getline(file,line);}
    while (std::getline(file,line))
    {
        if (line.substr(0,LINE_BEGIN_SIZE)=="state>")
        {
            newNode = new Node(bool(std::stoi(line.substr(LINE_BEGIN_SIZE,1))));
            addNode(newNode);
        }
        else
        {
            counter = 0;
            valuesInLine = line.substr(LINE_BEGIN_SIZE,std::string::npos);
            valueBegin = 0;
            valueEnd = 0;
            while (valueEnd != std::string::npos && counter <3)
            {
                valueEnd = valuesInLine.find_first_of(FILE_SEPARATOR, valueEnd);
                switch(counter)
                {
                    case 0:
                        origin = _head;
                        for (int i=0; i<std::stoi(valuesInLine.substr(valueBegin,valueEnd-valueBegin));i++)
                        {
                            origin = origin->getNext();
                        }
                        break;
                    case 1:
                        target = _head;
                        for (int i=0; i<std::stoi(valuesInLine.substr(valueBegin,valueEnd-valueBegin));i++)
                        {
                            target = target->getNext();
                        }
                        break;
                    case 2:
                        value = valuesInLine.substr(valueBegin,valueEnd-valueBegin)[0];
                }
                valueBegin = ++valueEnd;
                counter++;
            }
            newEdge = new Edge(origin, target, value);
            origin->addEdge(newEdge);
        }
    }
}

Graph::~Graph()
{
    // TO DO
    // read all nodes and delete them
    // read all links and delete them (should be inserted in node dtor)
}

int Graph::addNode(Node* node) // tested
{
    if (_head==nullptr)
    {
        _head=node;
        return SUCCESS;
    }
    else
    {
        Node* actualNode = _head;
        while(actualNode->getNext()!=nullptr)
        {
            actualNode=actualNode->getNext();
        }
        actualNode->setNext(node);
        node->setPrevious(actualNode);
        return SUCCESS;
    }
}

// dangerous because of the return of a pointer created dynamically
// => created a constructor with fileName as parameter
Graph* Graph::readFile(std::string fileName)
{
    Graph* newGraphPtr = new Graph();
    std::ifstream file(fileName);
    std::string line;
    for (int i=0;i<HEAD_SIZE;i++){file >> line;}
    while (file >> line)
    {
        if (line.substr(0,LINE_BEGIN_SIZE)=="state>")
        {
            //newGraphPtr->addNode(std::stoi(line.substr(LINE_BEGIN_SIZE,1)));
        }
        else
        {
            //
        }
    }
    return newGraphPtr;
}

void Graph::uglyPrint()
{
    Node *actualNode = _head;
    std::size_t numberOfEdges;
    Edge *actualEdge;
    while (actualNode != nullptr)
    {
        numberOfEdges = 0;
        actualEdge = actualNode->getFirstEdge();
        while (actualEdge != nullptr)
        {
            numberOfEdges++;
            actualEdge = actualEdge->getNext();
        }
        std::cout << "node " << actualNode->getID() << " created with " << numberOfEdges << " links" << std::endl;
        actualNode = actualNode->getNext();
    }
    std::cout << "number of nodes: " << Graph::Node::getNumberOfStates() << std::endl;
}

int Graph::wordEntry(std::string word)
{
    Node* actualNode = _head;
    Edge* actualEdge;
    if (actualNode == nullptr) {return FAILURE;}
    for (char letter : word)
    {
        actualEdge = actualNode->getFirstEdge();
        while (actualEdge!=nullptr && actualEdge->getValue()!=letter)
        {
            actualEdge = actualEdge->getNext();
        }
        if (actualEdge == nullptr)
        {
            return actualNode->getIsResponse();
        }
        else
        {
            actualNode = actualEdge->getTarget();
        }
    }
    return actualNode->getIsResponse();
}

/*
++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++      Classe Node     +++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++
*/

Graph::Node::Node()
{
    //ctor
}


Graph::Node::~Node()
{
    //dtor
}

int Graph::Node::addEdge(Edge* edge)
{
    if (_firstEdge==nullptr)
    {
        _firstEdge=edge;
        return SUCCESS;
    }
    else
    {
        Edge* actualEdge = _firstEdge;
        while(actualEdge->getNext()!=nullptr)
        {
            actualEdge=actualEdge->getNext();
        }
        actualEdge->setNext(edge);
        return SUCCESS;
    }
}

/*
++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++      Classe Edge     +++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++
*/


Graph::Edge::Edge()
{
    //ctor
}


Graph::Edge::~Edge()
{
    //dtor
}
