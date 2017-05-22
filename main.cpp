#include <iostream>
#include "Graph.h"

int main()
{
    // Tests ctor from file
    // automaton0.txt accepte tous les mots qui finissent pas C (alphabet = {A,B,C})
    Graph newGraph("automaton3.txt");
    newGraph.uglyPrint();
    newGraph.print();


    // Tests words entries
    /*
    std::string word;
    while(word != "exit")
    {
        std::cout << "enter your word: ";
        std::cin >> word;
        (newGraph.wordEntryWithCounters(word)) ? std::cout<<"accepted"<<std::endl : std::cout<<"refused"<<std::endl;
    }
    */


    // Tests for void tests
    std::cout << "\n Void test: " << std::endl;
    (newGraph.voidTest(Graph::DEPTH_FIRST_FROM_END)) ? std::cout << "accepts at least one word" << std::endl : std::cout << "no word accepted" << std::endl;

    return 0;
}

