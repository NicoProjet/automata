#include <iostream>
#include "Graph.h"

int main()
{

    // Tests ctor from file
    // automaton0.txt accepte tous les mots qui finissent pas C (alphabet = {A,B,C})

    Graph newGraph("automaton2.txt");
    newGraph.uglyPrint();


    // Tests words entries
    std::string word;
    while(1)
    {
        std::cout << "enter your word: ";
        std::cin >> word;
        (newGraph.wordEntryWithCounters(word)) ? std::cout<<"accepted"<<std::endl : std::cout<<"refused"<<std::endl;
    }


    return 0;
}
