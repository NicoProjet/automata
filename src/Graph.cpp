/*
TO DO LIST:
add < and > tests on counters
add reversal bounded constraint
find maximum size of words (theorical boundary (c.s)^R if i remember correctly)
    c = #counters  |  s = #states  |  R = # of reversal (counters)
add begin->end algorithm for void test
    recursively build a graph (state, counters[]) (Filiot's proposition)
    (i was thinking about sending the tuple as parameter without creating graph
     Breadth First Search or/and Depth First Search
add end->begin algorithm
try combine both ->  <- find a common node in algorithms
*/
#include "Graph.h"

/*
++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++      Classe Graph     ++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++
*/
std::size_t Graph::Node::numberOfStates = 0;

Graph::Graph(std::string fileName)
{
    // local variables declarations
    std::string word;
    char value, FILE_SEPARATOR;
    Node *newNode, *origin, *target;
    Edge *newEdge;
    std::size_t valueBegin, valueEnd;
    int counter = 0, HEAD_SIZE, LINE_BEGIN_SIZE, intValue;

    // open file
    std::ifstream file(fileName);
    std::string line, valuesInLine;

    // find variables for parsing
    // default values
    HEAD_SIZE = 0;
    NUMBER_OF_COUNTERS = 0;
    LINE_BEGIN_SIZE = 6;
    FILE_SEPARATOR = ':';
    file >> word;
    do
    {
        if (word == "HEAD_SIZE")
        {
            file >> intValue;
            HEAD_SIZE = intValue;
        }
        else if (word == "NUMBER_OF_COUNTERS")
        {
            file >> intValue;
            NUMBER_OF_COUNTERS = intValue;
        }
        else if (word == "LINE_BEGIN_SIZE")
        {
            file >> intValue;
            LINE_BEGIN_SIZE = intValue;
        }
        else if (word == "FILE_SEPARATOR")
        {
            file >> value;
            FILE_SEPARATOR = value;
        }
        counter++;
    } while (file >> word && counter < HEAD_SIZE);
    // reset streampose and counter
    file.clear();
    file.seekg( 0 );
    counter = 0;

    std::cout << "HEAD_SIZE= " << HEAD_SIZE << std::endl;
    std::cout << "NUMBER_OF_COUNTERS= " << NUMBER_OF_COUNTERS << std::endl;
    std::cout << "LINE_BEGIN_SIZE= " << LINE_BEGIN_SIZE << std::endl;
    std::cout << "FILE_SEPARATOR= " << FILE_SEPARATOR << std::endl;

    // read graph in file
    int counters[NUMBER_OF_COUNTERS];
    int countersChanges[NUMBER_OF_COUNTERS];
    std::string countersOperators[NUMBER_OF_COUNTERS];
    int counterIndex;
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
            counterIndex = 0;
            valuesInLine = line.substr(LINE_BEGIN_SIZE,std::string::npos);
            valueBegin = 0;
            valueEnd = 0;
            while (valueEnd != std::string::npos && counterIndex < NUMBER_OF_COUNTERS)
            {
                valueEnd = valuesInLine.find_first_of(FILE_SEPARATOR, valueEnd);
                switch(counter)
                {
                    case 0:
                        origin = _head;
                        intValue = std::stoi(valuesInLine.substr(valueBegin,valueEnd-valueBegin));
                        for (int i=0; i<intValue;i++)
                        {
                            origin = origin->getNext();
                        }
                        break;
                    case 1:
                        target = _head;
                        intValue = std::stoi(valuesInLine.substr(valueBegin,valueEnd-valueBegin));
                        for (int i=0; i<intValue;i++)
                        {
                            target = target->getNext();
                        }
                        break;
                    case 2:
                        value = toupper(valuesInLine.substr(valueBegin,valueEnd-valueBegin)[0]);
                        break;
                    default:
                        // get counter comparaison operator
                        countersOperators[counterIndex] = valuesInLine.substr(valueBegin,valueEnd-valueBegin);

                        // get counter value
                        valueBegin = ++valueEnd;
                        valueEnd = valuesInLine.find_first_of(FILE_SEPARATOR, valueEnd);
                        counters[counterIndex] = std::stoi(valuesInLine.substr(valueBegin,valueEnd-valueBegin));

                        //get counter change
                        valueBegin = ++valueEnd;
                        valueEnd = valuesInLine.find_first_of(FILE_SEPARATOR, valueEnd);
                        countersChanges[counterIndex] = std::stoi(valuesInLine.substr(valueBegin,valueEnd-valueBegin));
                        counterIndex++;
                        break;
                }
                valueBegin = ++valueEnd;
                counter++;
            }
            newEdge = new Edge(origin, target, value);
            for (int i=0;i<NUMBER_OF_COUNTERS;i++)
            {
                newEdge->addCounter(counters[i]);
                newEdge->addCounterChange(countersChanges[i]);
            }
            origin->addEdge(newEdge);
        }
    }
}

Graph::~Graph()
{
    Node *actualNode = _head;
    Node *nextNode = nullptr;
    while (actualNode != nullptr)
    {
        nextNode = actualNode->getNext();
        delete(actualNode);
        actualNode = nextNode;
    }
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

int Graph::wordEntryWithCounters(std::string word)
{
    Node* actualNode = _head;
    Edge* actualEdge;
    int counters[NUMBER_OF_COUNTERS] = {0};
    if (actualNode == nullptr) {return FAILURE;}
    for (char letter : word)
    {
        // 1) find the right link
        letter = toupper(letter);
        actualEdge = actualNode->getFirstEdge();
        /*
        // BUG caused by -> no short-circuit after actualEdge!=nullptr because there is a || in condition
        while ((actualEdge!=nullptr // no more edges
               && actualEdge->getValue()!=letter && actualEdge->getValue()!='-') // wrong value && not ignored
               || !(actualEdge->checkCounters(counters))) // wrong counters
        {
            actualEdge = actualEdge->getNext();
        }
        */
        if(actualEdge!=nullptr)
        {
            while ((actualEdge->getValue()!=letter && !actualEdge->getIgnoredValue()) // wrong value && not ignored
                   || !(actualEdge->checkCounters(counters))) // wrong counters
            {
                actualEdge = actualEdge->getNext();
                if(actualEdge==nullptr){return actualNode->getIsResponse();}
            }
        }
        // 2) go to next node and update counters
        else{return actualNode->getIsResponse();}
        actualNode = actualEdge->getTarget();
        for (int i = 0; i < NUMBER_OF_COUNTERS; i++)
        {
            counters[i] = counters[i] + actualEdge->getCounterChange(i);
        }
    }
    return actualNode->getIsResponse();
}

bool Graph::voidTest()
{
    //
}



/*
++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++      Classe Node     +++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++
*/

Graph::Node::~Node()
{
    // call edges destructors
    Edge *actualEdge = getFirstEdge();
    Edge *nextEdge = nullptr;
    while (actualEdge != nullptr)
    {
        nextEdge = actualEdge->getNext();
        delete(actualEdge);
        actualEdge = nextEdge;
    }
    // delete pointers
    delete(_next);
    delete(_previous);
    delete(_firstEdge);
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


Graph::Edge::~Edge()
{
    delete(_origin);
    delete(_target);
    delete(_next);
}

void Graph::Edge::addCounter(std::string op, int counterValue, int counterChange)
{
    addCounterOperator(op);
    addCounter(counterValue);
    addCounterChange(counterChange);
}

int Graph::Edge::checkCounters(int counters[])
{
    int response = 1, index = 0, countersSize = _counters.size();
    while (index<countersSize && response)
    {
        if (!(_counters[index]==-1))
        {
            if(_countersOperators[index] == "=")
            {
                response = (counters[index] == _counters[index]);
            }
            else if(_countersOperators[index] == "<")
            {
                response = (counters[index] < _counters[index]);
            }
            else if(_countersOperators[index] == ">")
            {
                response = (counters[index] > _counters[index]);
            }
        }
        index++;
    }
    return response;
}
