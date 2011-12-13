#include "Index.hpp"

Index::Index(const std::string& directory)
{
    // TODO ...
}

Index::~Index()
{
    // TODO ...
}

void Index::add(const UString& term, DocId docId)
{
    // TODO ...
}

std::vector<DocId> Index::getDocIds(const UString& term) const
{
    // TODO ...
    return std::vector<DocId>();
}

std::vector<UString> Index::getTerms(const UString& prefix) const
{
    // TODO ...
    return std::vector<UString>();
}

void Index::flush()
{
    // TODO ...
}
