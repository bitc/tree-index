#include "FileMultiTrie.hpp"

#include <iostream>

#include <stdio.h>
#include <string.h>

static const char FILE_HEADER[] = "TREE-INDEX0";

typedef uint32_t NumValuesHeader;
typedef uint32_t ValueElem;
typedef uint16_t NumChildrenHeader;
typedef uint32_t Offset;

size_t FileMultiTrie::NodeBlockSize(const MultiTrie::Node& node)
{
    size_t size = 0;
    size += sizeof(NumValuesHeader);
    size += node.values.size() * sizeof(ValueElem);
    size += sizeof(NumChildrenHeader);
    size += node.children.size() * (sizeof(UChar) + sizeof(Offset));
    return size;
}

void FileMultiTrie::WriteMultiTrie(const MultiTrie& multiTrie, const std::string& file)
{
    // TODO handle IO errors

    FILE* fp = fopen(file.c_str(), "wb");

    fwrite(FILE_HEADER, sizeof(FILE_HEADER), 1, fp);

    WriteNode(multiTrie.root, fp);

    fclose(fp);
}

void FileMultiTrie::WriteNode(const MultiTrie::Node& node, FILE* fp)
{
    // TODO handle IO errors

    // Write Value header
    NumValuesHeader numValues = node.values.size();
    fwrite(&numValues, sizeof(numValues), 1, fp);

    // Write Values:
    for(std::vector<DocId>::const_iterator i = node.values.begin(); i != node.values.end(); ++i)
    {
        ValueElem v = *i;
        fwrite(&v, sizeof(v), 1, fp);
    }

    // Write Children header
    NumChildrenHeader numChildren = node.children.size();
    fwrite(&numChildren, sizeof(numChildren), 1, fp);

    Offset offset = (node.children.size() - 1) * (sizeof(UChar) + sizeof(Offset));
    for(std::vector<MultiTrie::Child>::const_iterator i = node.children.begin(); i != node.children.end(); ++i)
    {
        UChar c = i->c;
        fwrite(&c, sizeof(c), 1, fp);
        fwrite(&offset, sizeof(offset), 1, fp);
        offset += NodeBlockSize(*i->child);
    }

    for(std::vector<MultiTrie::Child>::const_iterator i = node.children.begin(); i != node.children.end(); ++i)
    {
        WriteNode(*i->child, fp);
    }
}

static bool ValidHeader(FILE* fp)
{
    char header[sizeof(FILE_HEADER)];
    fread(header, sizeof(header), 1, fp);
    return strncmp(header, FILE_HEADER, sizeof(header)) == 0;
}

void FileMultiTrie::DebugDumpFile(const std::string& file)
{
    // TODO handle IO errors

    FILE* fp = fopen(file.c_str(), "rb");

    if(!ValidHeader(fp))
    {
        std::cout << "Corrupt database: Invalid header" << std::endl;
        return;
    }

    while(true)
    {
        // Read Value header
        NumValuesHeader numValues;
        if(fread(&numValues, sizeof(numValues), 1, fp) < 1)
        {
            return;
        }
        std::cout << "Num Values: " << numValues << std::endl;
        for(size_t i = 0; i < numValues; ++i)
        {
            ValueElem v;
            fread(&v, sizeof(v), 1, fp);
            std::cout << v << std::endl;
        }

        NumChildrenHeader numChildren;
        fread(&numChildren, sizeof(numChildren), 1, fp);
        std::cout << "Num Children: " << numChildren << std::endl;
        for(size_t i = 0; i < numChildren; ++i)
        {
            UChar c;
            fread(&c, sizeof(c), 1, fp);
            std::cout << c << " (" << char(c) << ")" << std::endl;
            Offset offset;
            fread(&offset, sizeof(offset), 1, fp);
            std::cout << offset << std::endl;
        }
    }

    fclose(fp);
}


FileMultiTrie::FileMultiTrie(const std::string& file)
{
    // TODO handle IO errors

    fp = fopen(file.c_str(), "rb");

    if(!ValidHeader(fp))
    {
        // TODO Should throw an exception instead
        std::cout << "Corrupt database: Invalid header" << std::endl;
    }
}

FileMultiTrie::~FileMultiTrie()
{
    fclose(fp);
}

std::vector<DocId> FileMultiTrie::getDocIds(const UString& term) const
{
    // TODO handle IO errors and malformed files

    fseek(fp, sizeof(FILE_HEADER), SEEK_SET);

    UString::const_iterator t = term.begin();
    while(t != term.end())
    {
        NumValuesHeader numValues;
        fread(&numValues, sizeof(numValues), 1, fp);
        fseek(fp, numValues * sizeof(ValueElem), SEEK_CUR);

        NumChildrenHeader numChildren;
        fread(&numChildren, sizeof(numChildren), 1, fp);

        bool foundChild = false;
        for(size_t i = 0; i < numChildren && !foundChild; ++i)
        {
            UChar c;
            fread(&c, sizeof(c), 1, fp);
            Offset offset;
            fread(&offset, sizeof(offset), 1, fp);
            if(c == *t)
            {
                ++t;
                fseek(fp, offset, SEEK_CUR);
                foundChild = true;
            }
        }
        if(!foundChild)
            return std::vector<DocId>();
    }

    NumValuesHeader numValues;
    fread(&numValues, sizeof(numValues), 1, fp);
    std::vector<DocId> result;
    result.reserve(numValues);
    for(size_t i = 0; i < numValues; ++i)
    {
        ValueElem v;
        fread(&v, sizeof(v), 1, fp);
        result.push_back(v);
    }
    return result;
}

std::vector<UString> FileMultiTrie::getTerms(const UString& prefix) const
{
    // TODO ...

    return std::vector<UString>();
}
