/*
TO DO LIST:
add begin->end algorithm for void test
    recursively build a graph (state, counters[]) (Filiot's proposition)
    (i was thinking about sending the tuple as parameter without creating graph)
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
std::size_t Graph::Edge::numberOfLinks = 0;

Graph::Graph(std::string fileName)
{
    // local variables declarations
    std::string word;
    char value, FILE_SEPARATOR;
    Node *newNode, *origin, *target;
    Edge *newEdge;
    std::size_t valueBegin, valueEnd;
    int counter = 0, HEADER_SIZE, LINE_BEGIN_SIZE, intValue;

    // open file
    std::ifstream file(fileName);
    std::string line, valuesInLine;

    // find const variables in file
    // default values
    HEADER_SIZE = 0;
    NUMBER_OF_COUNTERS = 0;
    REVERSAL_BOUND = -1;
    LINE_BEGIN_SIZE = 6;
    FILE_SEPARATOR = ':';


    // real values
    file >> word;
    do
    {
        if (word == "HEADER_SIZE")
        {
            file >> intValue;
            HEADER_SIZE = intValue;
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
        else if (word == "REVERSAL_BOUND")
        {
            file >> intValue;
            REVERSAL_BOUND = intValue;
        }
        else if (word == "ALPHABET")
        {
            while (file >> word && word != "END")
            {
                value = word[0];
                ALPHABET.push_back(value);
            }
        }
        counter++;
    } while (file >> word && counter < HEADER_SIZE);

    // reset streampos and counter
    file.clear();
    file.seekg( 0 );
    counter = 0;

    std::cout << "HEADER_SIZE = " << HEADER_SIZE << std::endl;
    std::cout << "NUMBER_OF_COUNTERS = " << NUMBER_OF_COUNTERS << std::endl;
    std::cout << "REVERSAL_BOUND = " << REVERSAL_BOUND << std::endl;
    std::cout << "LINE_BEGIN_SIZE = " << LINE_BEGIN_SIZE << std::endl;
    std::cout << "FILE_SEPARATOR = " << FILE_SEPARATOR << std::endl;
    std::cout << "ALPHABET = { ";
    for (char letter : ALPHABET)
    {
        std::cout << letter << " ";
    }
    std::cout << "}" << std::endl;

    // read and construct graph from file
    int counters[NUMBER_OF_COUNTERS];
    int countersChanges[NUMBER_OF_COUNTERS];
    std::string countersOperators[NUMBER_OF_COUNTERS];
    int counterIndex, numberOfLinks = 0, numberOfNodes = 0;
    for (int i=0;i<HEADER_SIZE;i++){std::getline(file,line);}
    while (std::getline(file,line))
    {
        if (line.substr(0,LINE_BEGIN_SIZE)=="state>")
        {
            newNode = new Node(bool(std::stoi(line.substr(LINE_BEGIN_SIZE,1))));
            addNode(newNode);
            numberOfNodes++;
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
            numberOfLinks++;
            for (int i=0;i<NUMBER_OF_COUNTERS;i++)
            {
                newEdge->addCounter(countersOperators[i], counters[i], countersChanges[i]);
            }
            origin->addEdge(newEdge);
        }
    }
    VOID_TEST_BOUND = pow((NUMBER_OF_COUNTERS * numberOfLinks), (NUMBER_OF_COUNTERS * CONSTANT_C));
}

Graph::Graph(Graph *g)
{
    NUMBER_OF_COUNTERS = g->NUMBER_OF_COUNTERS;
    REVERSAL_BOUND = g->REVERSAL_BOUND;
    ALPHABET = g->ALPHABET;
    Node *actualNode = g->getHead();
    Node *origin, *target;
    std::stack<Node*> nodeStack;
    std::stack<Edge*> edgeStack;
    int numberOfLinks;
    // invert edges
    while (actualNode != nullptr)
    {
        Edge *actualEdge = actualNode->getFirstEdge();
        while (actualEdge != nullptr)
        {
            numberOfLinks++;
            target = actualEdge->getTarget();
            origin = actualEdge->getOrigin();
            actualEdge->setTarget(origin);
            actualEdge->setOrigin(target);
            for (int i; i<NUMBER_OF_COUNTERS; i++){actualEdge->setCounterChange(i,-(actualEdge->getCounterChange(i)));}
            actualEdge = actualEdge->getNext();
        }
        actualNode = actualNode->getNext();
    }
    // invert nodes
    actualNode = g->getHead();
    actualNode->setIsResponse(true);
    while (actualNode != nullptr)
    {
        nodeStack.push(actualNode);
        actualNode=actualNode->getNext();
    }
    actualNode = nodeStack.top();
    actualNode->setIsResponse(false);
    while(!nodeStack.empty())
    {
        addNode(nodeStack.top());
        nodeStack.pop();
    }
    VOID_TEST_BOUND = pow((NUMBER_OF_COUNTERS * numberOfLinks), (NUMBER_OF_COUNTERS * CONSTANT_C));
}

Graph::~Graph()
{
    Node *actualNode = _head;
    Node *nextNode = nullptr;
    while (actualNode != nullptr)
    {
        nextNode = actualNode->getNext();
        delete actualNode;
        actualNode = nextNode;
    }
}

int Graph::addNode(Node* node)
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
    Node *actualNode = _head, *lastNode = nullptr;
    int numberOfReversals = 0;  // number of times we go from INC to DEC or the other way around
    int counters[NUMBER_OF_COUNTERS] = {0};
    int lastReversals[NUMBER_OF_COUNTERS] = {0}; // save last operations on counters
    if (actualNode == nullptr) {return FAILURE;}
    for (char letter : word)
    {
        lastNode = actualNode;
        actualNode = processNode(actualNode, letter, counters, numberOfReversals, lastReversals);
        if ((actualNode == nullptr) || (REVERSAL_BOUND != -1  &&  numberOfReversals > REVERSAL_BOUND))
        {
            return lastNode->getIsResponse();
        }
    }
    return actualNode->getIsResponse();
}

/*
        // BUG caused by -> no short-circuit after actualEdge!=nullptr because there is a || in condition
        while ((actualEdge!=nullptr // no more edges
               && actualEdge->getValue()!=letter && actualEdge->getValue()!='-') // wrong value && not ignored
               || !(actualEdge->checkCounters(counters))) // wrong counters
        {
            actualEdge = actualEdge->getNext();
        }
        */
Graph::Node* Graph::processNode(Node *actualNode, char letter, int counters[], int &numberOfReversals, int lastReversals[])
{
    // 1) find the right link
    letter = toupper(letter);
    Edge *actualEdge = actualNode->getFirstEdge();
    if(actualEdge!=nullptr)
    {
        while ((actualEdge->getValue()!=letter && !actualEdge->getIgnoredValue()) // wrong value && not ignored
                || !(actualEdge->checkCounters(counters))) // wrong counters
        {
            actualEdge = actualEdge->getNext();
            if(actualEdge==nullptr){return nullptr;}
        }
    }
    // 2) go to next node and update counters
    else{return nullptr;}
    actualNode = actualEdge->getTarget();
    numberOfReversals += actualEdge->updateCounters(counters, lastReversals);
    return actualNode;
}

bool Graph::voidTest(int type)
{
    switch(type)
    {
        // I know breaks are useless because we return juste before ...
        // it is out of habit and safety
        case DEPTH_FIRST: return voidTestFullDFS(); break;
        case BREADTH_FIRST: return voidTestFullBFS(); break;
        case DEPTH_FIRST_FROM_END: return voidTestFromEnd(DEPTH_FIRST); break;
        case BREADTH_FIRST_FROM_END: return voidTestFromEnd(BREADTH_FIRST); break;
        case DEPTH_FIRST_DYNAMIC: return voidTestDynamicDFS(DEPTH_FIRST_DYNAMIC); break;
        case DEPTH_FIRST_DYNAMIC_STATES_SAVE: break;
    }
}

bool Graph::voidTestDynamicDFS(int type)
{
    Node *actualNode = _head;
    int numberOfReversals = 0;
    int counters[NUMBER_OF_COUNTERS] = {0};
    int lastReversals[NUMBER_OF_COUNTERS] = {0};
    std::string word = "";
    switch (type)
    {
        case DEPTH_FIRST_DYNAMIC:
            return voidTestLoopDFS(actualNode, counters, numberOfReversals, lastReversals, word);
            break;
        case DEPTH_FIRST_DYNAMIC_STATES_SAVE:
            std::vector<StateSave> statesSave;
            return voidTestLoopDFS_withStatesSave(actualNode, counters, numberOfReversals, lastReversals, word, statesSave);
            break;
    }
}

bool Graph::voidTestLoopDFS(Node *actualNode, int counters[], int numberOfReversals, int lastReversals[], std::string word)
{
    bool response = false;
    if (word.size() < VOID_TEST_BOUND){
        size_t index = 0;
        while (index < ALPHABET.size() && !response)
        {
            // deepCopy counters state and word
            int newCounters[NUMBER_OF_COUNTERS];
            int newLastReversals[NUMBER_OF_COUNTERS];
            for (int i = 0; i<NUMBER_OF_COUNTERS; i++)
            {
                newCounters[i] = counters[i];
                newLastReversals[i] = lastReversals[i];
            }
            int newNumberOfReversals = numberOfReversals;
            std::string newWord = word;

            // find new state
            newWord += ALPHABET[index];
            Node *lastNode = actualNode;
            Node *nextNode = processNode(actualNode, ALPHABET[index], newCounters, newNumberOfReversals, newLastReversals);
            if ((actualNode == nullptr) || (REVERSAL_BOUND != -1  &&  newNumberOfReversals > REVERSAL_BOUND))
            {
                return lastNode->getIsResponse();
            }
            response = nextNode->getIsResponse();
            if (!response)
            {
                response = voidTestLoopDFS(nextNode, newCounters, newNumberOfReversals, newLastReversals, newWord);
            }
            else
            {
                std::cout << "found accepted word: " << newWord << std::endl;
            }
            index++;
        }
    }
    return response;
}

bool Graph::voidTestLoopDFS_withStatesSave(Node *actualNode, int counters[], int numberOfReversals, int lastReversals[], std::string word, std::vector<StateSave> statesSave)
{
    bool response = false;
    if (word.size() < VOID_TEST_BOUND){
        size_t index = 0;
        while (index < ALPHABET.size() && !response)
        {
            // deepCopy counters state and word
            int newCounters[NUMBER_OF_COUNTERS];
            int newLastReversals[NUMBER_OF_COUNTERS];
            for (int i = 0; i<NUMBER_OF_COUNTERS; i++)
            {
                newCounters[i] = counters[i];
                newLastReversals[i] = lastReversals[i];
            }
            int newNumberOfReversals = numberOfReversals;
            std::string newWord = word;

            // find new state
            newWord += ALPHABET[index];
            Node *lastNode = actualNode;
            Node *nextNode = processNode(actualNode, ALPHABET[index], newCounters, newNumberOfReversals, newLastReversals);
            if ((actualNode == nullptr) || (REVERSAL_BOUND != -1  &&  newNumberOfReversals > REVERSAL_BOUND))
            {
                return lastNode->getIsResponse();
            }
            response = nextNode->getIsResponse();
            if (!response)
            {
                response = voidTestLoopDFS_withStatesSave(nextNode, newCounters, newNumberOfReversals, newLastReversals, newWord, statesSave);
            }
            else
            {
                std::cout << "found accepted word: " << newWord << std::endl;
            }
            index++;
        }
    }
    return response;
}

bool Graph::voidTestFullDFS()
{
    bool response = false;
    std::string word = "";
    std::stack<std::string> s;
    s.push("");
    do
    {
        word = s.top();
        s.pop();
        if (word.size() < VOID_TEST_BOUND)
        {
            for (int index = ALPHABET.size()-1; index >= 0; index--)
            {
                std::string newWord = word + ALPHABET[index];
                s.push(newWord);
            }
        }
        response = wordEntryWithCounters(word);
        if (response){std::cout << "found accepted word: " << word << std::endl;}
    } while (!response);
    return response;
}

bool Graph::voidTestFullBFS()
{
    bool response = false;
    std::string word = "";
    std::queue<std::string> heap;
    do
    {
        // 1) enfiler
        for (char letter : ALPHABET)
        {
            std::string newWord = word + letter;
            heap.push(newWord);
        }

        // 2) on sort le prochain mot
        word = heap.front();
        heap.pop();

        // on parcourt le graphe avec ce mot
        response = wordEntryWithCounters(word);
        if (response) {std::cout << "found accepted word: " << word << std::endl;}
    } while (!response && word.size() < VOID_TEST_BOUND);
    return response;
}

bool Graph::voidTestFromEnd(int type)
{
    invertGraph();
    return voidTest(type);
}

Graph Graph::makeInvertGraph()
{
    Graph newGraph(this);
    return newGraph;
}

void Graph::invertGraph()
{
    Node *actualNode = _head;
    Node *previousNode = nullptr;
    Node *origin, *target;
    std::stack<Node*> nodeStack;
    while (actualNode != nullptr)
    {
        Edge *previousEdge = nullptr;
        Edge *actualEdge = actualNode->getFirstEdge();
        actualNode->setFirstEdge(nullptr);
        while (actualEdge != nullptr)
        {
            target = actualEdge->getTarget();
            origin = actualEdge->getOrigin();
            actualEdge->setTarget(origin);
            actualEdge->setOrigin(target);
            for (int i; i<NUMBER_OF_COUNTERS; i++){actualEdge->setCounterChange(i,-(actualEdge->getCounterChange(i)));}
            origin->addEdge(actualEdge);
            previousEdge = actualEdge;
            actualEdge = actualEdge->getNext();
            previousEdge->setNext(nullptr);
        }
        actualNode = actualNode->getNext();
    }
    // invert nodes
    actualNode = _head;
    actualNode->setIsResponse(true);
    while (actualNode != nullptr)
    {
        nodeStack.push(actualNode);
        previousNode = actualNode;
        actualNode = actualNode->getNext();
        previousNode->setNext(nullptr);
    }
    actualNode = nodeStack.top();
    actualNode->setIsResponse(false);
    while(!nodeStack.empty())
    {
        addNode(nodeStack.top());
        nodeStack.pop();
    }
}




/*
++++++++++++++++++++++++++++++++++++++++++++++++
+++++++++++      Classe Node     +++++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++
*/

Graph::Node::~Node()
{
    // call edges destructors
    Edge *actualEdge = _firstEdge;
    Edge *nextEdge = nullptr;
    while (actualEdge != nullptr)
    {
        nextEdge = actualEdge->getNext();
        delete(actualEdge);
        actualEdge = nextEdge;
    }
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
    // default dtor is enough
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
        if (!(_counters[index ]== -1))
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

int Graph::Edge::updateCounters(int counters[], int lastReversals[])
{
    int numberOfReversals = 0, lastCounterValue = 0;
    for (size_t index = 0; index < _counters.size(); index++)
    {
        if (!(_countersChanges[index] == -1))
        {
            lastCounterValue = counters[index];
            counters[index] = counters[index] + _countersChanges[index];
            if (lastReversals[index] > 0) // last op was INC
            {
                if (lastCounterValue > counters[index]) // we used DEC
                {
                    numberOfReversals++;
                    lastReversals[index] = -1;
                }
            }
            else if (lastReversals[index] < 0) // last op was DEC
            {
                if (lastCounterValue < counters[index]) // we used INC
                {
                    numberOfReversals++;
                    lastReversals[index] = 1;
                }
            }
            else // first time we touch the counter
            {
                if (lastCounterValue > counters[index]) // we used DEC
                {
                    lastReversals[index] = -1;
                }
                else // we used INC
                {
                    lastReversals[index] = 1;
                }
            }
        }
    }
    return numberOfReversals;
}

std::string Graph::Edge::toString()
{
    std::string str = "link >";
    str += _id;
    return str;
}


/*
++++++++++++++++++++++++++++++++++++++++++++++++
++++++++++     Classe StateSave     ++++++++++++
++++++++++++++++++++++++++++++++++++++++++++++++
*/

int Graph::StateSave::checkCounters(std::vector<int> counters)
{
    int response = 1, index = 0, countersSize = _counters.size();
    while (index<countersSize && response)
    {
        response = _counters[index] == counters[index];
        index++;
    }
    return response;
}


bool Graph::StateSave::operator==(const StateSave& other)
{
    return _state->getID() == other.getNodeID() && checkCounters(other._counters);
}

