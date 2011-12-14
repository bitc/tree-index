#ifndef FILE_MULTI_TRIE_HPP_
#define FILE_MULTI_TRIE_HPP_

#include <string>
#include <vector>

#include "Misc.hpp"
#include "MultiTrie.hpp"

// Looks up terms directly from the database file
// Use WriteMultiTrie to create the database file
class FileMultiTrie
{
    public:
        static void WriteMultiTrie(const MultiTrie& multiTrie, const std::string& file);

        FileMultiTrie(const std::string& file);
        ~FileMultiTrie();

        std::vector<DocId> getDocIds(const UString& term) const;

        std::vector<UString> getTerms(const UString& prefix) const;

        static void DebugDumpFile(const std::string& file);

    private:
        static size_t NodeBlockSize(const MultiTrie::Node& node);
        static void WriteNode(const MultiTrie::Node& node, FILE* fp);

};

#endif
