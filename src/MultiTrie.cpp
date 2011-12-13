#include <algorithm>
#include <iostream>

#include "MultiTrie.hpp"

UString createUString(const std::string& str)
{
    UString result;
    for(std::string::const_iterator i = str.begin(); i != str.end(); ++i)
        result.push_back(*i);
    //std::copy(str.begin(), str.end(), result.begin());
    return result;
}

MultiTrie::MultiTrie()
{
}

MultiTrie::~MultiTrie()
{
}

void MultiTrie::add(const UString& term, DocId docId)
{
    Node* n = &root;
    UString::const_iterator i = term.begin();

    // Step 1, Traverse down as far as possible in the current tree structure

    Node* n2;
    while((n2 = n->getChild(*i)) != 0)
    {
        n = n2;
        ++i;
        if(i == term.end())
        {
            // The full key already exists in the tree. Just add the docId to
            // the current node
            // TODO Don't add the docId if it already exists
            n->values.push_back(docId);
            return;
        }
    }

    // Step 2: Build a chain of intermediate nodes

    while(i != term.end())
    {
        boost::shared_ptr<Node> childNode(new Node);
        n->children.push_back(Child(*i, childNode));
        n = childNode.get();
        ++i;
    }

    // Step 3: Add the value to the leaf node
    n->values.push_back(docId);
}

std::vector<DocId> MultiTrie::getDocIds(const UString& term) const
{
    const Node* n = &root;
    UString::const_iterator i = term.begin();

    while(i != term.end())
    {
        n = n->getChild(*i);
        if(n == 0)
            return std::vector<DocId>();
        ++i;
    }

    return n->values;
}

void MultiTrie::debugDump() const
{
    debugDump(&root, std::string());
}

void MultiTrie::debugDump(const Node* n, const std::string& prefix)
{
    std::cout << prefix << ": ";
    for(std::vector<DocId>::const_iterator i = n->values.begin(); i != n->values.end(); ++i)
    {
        std::cout << *i << " ";
    }
    std::cout << std::endl;

    for(std::vector<Child>::const_iterator i = n->children.begin(); i != n->children.end(); ++i)
    {
        std::string prefix2 = prefix;
        prefix2.push_back(i->c);
        debugDump(i->child.get(), prefix2);
    }
}
