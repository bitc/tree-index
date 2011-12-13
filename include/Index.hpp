#ifndef INDEX_HPP_
#define INDEX_HPP_

#include <string>
#include <vector>

#include "Misc.hpp"
#include "MultiTrie.hpp"

class Index
{
    public:
        Index(const std::string& directory);
        ~Index();

        void add(const UString& term, DocId docId);

        std::vector<DocId> getDocIds(const UString& term) const;

        std::vector<UString> getTerms(const UString& prefix) const;

        void flush();

    private:
        MultiTrie multiTrie;
};

#endif
