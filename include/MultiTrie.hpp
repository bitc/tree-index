#ifndef MULTI_TRIE_HPP_
#define MULTI_TRIE_HPP_

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

typedef unsigned int UChar;
typedef std::basic_string<UChar> UString;
typedef unsigned long DocId;

UString createUString(const std::string& str);

class MultiTrie
{
    private:
        struct Node;

        struct Child
        {
            Child(UChar c, boost::shared_ptr<Node> child) : c(c), child(child) {}
            UChar c;
            boost::shared_ptr<Node> child;
        };

        struct Node
        {
            std::vector<DocId> values;
            std::vector<Child> children;

            Node* getChild(UChar c)
            {
                for(std::vector<Child>::const_iterator i = children.begin(); i != children.end(); ++i)
                    if(i->c == c)
                        return i->child.get();
                return 0;
            }

            const Node* getChild(UChar c) const
            {
                for(std::vector<Child>::const_iterator i = children.begin(); i != children.end(); ++i)
                    if(i->c == c)
                        return i->child.get();
                return 0;
            }
        };

    public:
        MultiTrie();
        ~MultiTrie();

        void add(const UString& term, DocId docId);

        void add(const std::string& term, DocId docId)
        {
            add(createUString(term), docId);
        }

        std::vector<DocId> getDocIds(const UString& term) const;

        void debugDump() const;

    private:
        static void debugDump(const Node* n, const std::string& prefix);

        Node root;
};

#endif

