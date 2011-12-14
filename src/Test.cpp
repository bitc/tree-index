#include <iostream>

#include "Index.hpp"
#include "MultiTrie.hpp"
#include "FileMultiTrie.hpp"

void testStdInput();

int main()
{
    MultiTrie trie;

    trie.add("dog", 1);
    trie.add("cat", 2);
    trie.add("dog", 3);
    trie.add("doggy", 1);
    trie.debugDump();

    std::cout << std::endl;

    std::cout << trie.getDocIds(createUString("dog"))[0] << std::endl;
    std::cout << trie.getDocIds(createUString("dog"))[1] << std::endl;
    std::cout << trie.getDocIds(createUString("doggy"))[0] << std::endl;

    std::cout << std::endl;

    FileMultiTrie::WriteMultiTrie(trie, "test.trie");
    FileMultiTrie::DebugDumpFile("test.trie");

    FileMultiTrie fileTrie("test.trie");

    std::cout << std::endl;

    std::cout << fileTrie.getDocIds(createUString("dog"))[0] << std::endl;
    std::cout << fileTrie.getDocIds(createUString("dog"))[1] << std::endl;
    std::cout << fileTrie.getDocIds(createUString("doggy"))[0] << std::endl;

    testStdInput();
}

// Pipe a file such as /usr/share/dict/words into stdin
void testStdInput()
{
    MultiTrie trie;

    std::cout << "Reading stdin..." << std::endl;

    std::vector<std::string> allWords;

    std::string word;
    while(true)
    {
        std::getline(std::cin, word);
        if(std::cin.eof())
            break;
        trie.add(word, 42);
        allWords.push_back(word);
    }

    std::cout << "Writing to file..." << std::endl;

    FileMultiTrie::WriteMultiTrie(trie, "words.trie");

    std::cout << "Checking Memory trie..." << std::endl;

    for(std::vector<std::string>::const_iterator i = allWords.begin(); i != allWords.end(); ++i)
    {
        std::vector<DocId> docs = trie.getDocIds(createUString(*i));
        if(docs.size() != 1 || docs[0] != 42)
        {
            std::cout << "ERROR: " << *i << std::endl;
        }
    }

    std::cout << "Checking File trie..." << std::endl;

    FileMultiTrie fileTrie("words.trie");

    for(std::vector<std::string>::const_iterator i = allWords.begin(); i != allWords.end(); ++i)
    {
        std::vector<DocId> docs = fileTrie.getDocIds(createUString(*i));
        if(docs.size() != 1 || docs[0] != 42)
        {
            std::cout << "ERROR: " << *i << std::endl;
        }
    }

    std::cout << "Done" << std::endl;
}
