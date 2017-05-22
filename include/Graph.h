#ifndef GRAPH_H
#define GRAPH_H
#define SUCCESS 1
#define FAILURE 0
#include "Graph.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <math.h>
#include <queue>
#include <stack>


class Graph
{
    private:
        class Node;
        class Edge;

        Node *_head = nullptr;
        int NUMBER_OF_COUNTERS, REVERSAL_BOUND, CONSTANT_C = 2;
        size_t VOID_TEST_BOUND;
        std::vector<char> ALPHABET;

        class Node
        {
            private:
                Node *_next = nullptr;
                Node *_previous = nullptr;
                Edge *_firstEdge = nullptr;
                bool _isResponse;
                static std::size_t numberOfStates;
                std::size_t _id;
            public:
                Node(){};
                Node(bool isResponse): _isResponse(isResponse), _id(numberOfStates++) {};
                ~Node();

                //  getters/setters
                Node* getNext(){return _next;}
                void setNext(Node *node){_next=node;}
                Node* getPrevious(){return _previous;}
                void setPrevious(Node *node){_previous=node;}
                Edge* getFirstEdge(){return _firstEdge;}
                void setFirstEdge(Edge *edge){_firstEdge=edge;}
                bool getIsResponse(){return _isResponse;}
                void setIsResponse(bool boolean){_isResponse=boolean;}
                std::size_t getID(){return _id;}
                static std::size_t getNumberOfStates(){return numberOfStates;}
                void print();

                // other methods
                int addEdge(Edge *edge);

        };

        class Edge
        {
            private:
                Node *_origin = nullptr;
                Node *_target = nullptr;
                Edge *_next = nullptr;
                // constraints
                char _value;
                bool _ignoredValue = 0;
                std::vector<int> _counters, _countersChanges;
                std::vector<std::string> _countersOperators;
                static std::size_t numberOfLinks;
                std::size_t _id = numberOfLinks++;

            public:
                Edge(){};
                Edge(Node* origin, Node* target, char value): _origin(origin), _target(target), _value(value), _ignoredValue(value=='-') {};

                ~Edge();

                // getters/setters
                Node* getOrigin(){return _origin;}
                void setOrigin(Node *node){_origin=node;}
                Node* getTarget(){return _target;}
                void setTarget(Node *node){_target=node;}
                Edge* getNext(){return _next;}
                void setNext(Edge *edge){_next=edge;}
                char getValue(){return _value;}
                void setValue(char value){_value = value;}
                int getCounter(int index){return _counters[index];}
                void setCounter(int index, int value){_counters[index] = value;}
                void addCounter(int value){_counters.push_back(value);}
                int getCounterChange(int index){return _countersChanges[index];}
                void setCounterChange(int index, int value){_countersChanges[index] = value;}
                void addCounterChange(int value){_countersChanges.push_back(value);}
                void addCounterOperator(std::string op){_countersOperators.push_back(op);}
                void addCounter(std::string op, int counterValue, int counterChange);
                bool getIgnoredValue(){return _ignoredValue;}
                void setIgnoredValue(bool value){_ignoredValue = value;}
                int checkCounters(int counters[]);
                int updateCounters(int counters[], int lastReversals[]);
                void print();
                size_t getId(){return _id;}
        };

        class StateSave
        {
            private:
                Node *_state;
                std::vector<int> _counters;

            public:
                StateSave(Node *state, int counters[], size_t numberOfCounters);
                size_t getNodeID() const {return _state->getID();}
                int checkCounters(std::vector<int> counters);
                void addCounter(int value){_counters.push_back(value);}
                bool operator==(const StateSave& other);
        };

    public:
        const static int
         DEPTH_FIRST = 0,
         BREADTH_FIRST = 1,
         DEPTH_FIRST_DYNAMIC = 2,
         DEPTH_FIRST_DYNAMIC_STATES_SAVE = 3,
         FROM_END = 3,
         BREADTH_FIRST_FROM_END = 4,
         DEPTH_FIRST_FROM_END = 5;
        Graph(){};
        Graph(std::string fileName);
        ~Graph();

        Node* getHead(){return _head;}
        int addNode(Node* node);
        static Graph* readFile(std::string fileName);
        void uglyPrint();
        int wordEntry(std::string word);
        int wordEntryWithCounters(std::string word);
        Node* processNode(Node *actualNode, char letter, int counters[], int &numberOfReversals, int lastReversals[]);
        bool voidTest(int type = 0);
        bool voidTestLoopDFS(Node *actualNode, int counters[], int numberOfReversals, int lastReversals[], std::string word);
        bool voidTestFullDFS();
        bool voidTestFullBFS();
        bool voidTestDynamicDFS(int type);
        bool voidTestFromEnd(int type);
        Graph makeInvertGraph();
        void invertGraph();
        bool voidTestLoopDFS_withStatesSave(Node *actualNode, int counters[], int numberOfReversals, int lastReversals[], std::string word, std::vector<StateSave> statesSave);
        void print();
};


#endif // GRAPH_H
