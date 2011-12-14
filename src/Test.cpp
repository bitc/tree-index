#include <iostream>

#include "Index.hpp"
#include "MultiTrie.hpp"
#include "FileMultiTrie.hpp"

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
}
