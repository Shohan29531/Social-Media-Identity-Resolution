#include<bits/stdc++.h>
#include <cstdio>
#include <cstdlib>
#include <vector>

#define ALPHABETS 26
#define CASE 'a'
#define MAX_WORD_SIZE 25

using namespace std;

struct Node
{
    struct Node * parent;
    struct Node * children[ALPHABETS];
    vector<int> occurrences;
};

bool Dictionary_word(string s)
{
    for(int i=0; i<s.size(); i++)
    {
        if((s[i]>='A' && s[i]<='Z') || (s[i]>='a' && s[i]<='z')) continue;
        else return false;
    }
    return true;
}
string All_lower_case(string s)
{
    for(int i=0; i<s.size(); i++)
    {
        s[i]=tolower(s[i]);
    }
    return s;
}

// Inserts a word 'text' into the Trie Tree
// 'trieTree' and marks it's occurence as 'index'.
void InsertWord(struct Node * trieTree, char * word, int index)
{
    struct Node * traverse = trieTree;

    while (*word != '\0')       // Until there is something to process
    {
        if (traverse->children[*word - CASE] == NULL)
        {
            // There is no node in 'trieTree' corresponding to this alphabet

            // Allocate using calloc(), so that components are initialised
            traverse->children[*word - CASE] = (struct Node *) calloc(1, sizeof(struct Node));
            traverse->children[*word - CASE]->parent = traverse;  // Assigning parent
        }

        traverse = traverse->children[*word - CASE];
        ++word; // The next alphabet
    }

    traverse->occurrences.push_back(index);      // Mark the occurence of the word
}

// Searches for the occurence of a word in 'trieTree',
// if not found, returns NULL,
// if found, returns poniter pointing to the
// last node of the word in the 'trieTree'
// Complexity -> O(length_of_word_to_be_searched)
struct Node * SearchWord(struct Node * treeNode, char * word)
{
    // Function is very similar to insert() function
    while (*word != '\0')
    {
        if (treeNode->children[*word - CASE] != NULL)
        {
            treeNode = treeNode->children[*word - CASE];
            ++word;
        }
        else
        {
            break;
        }
    }

    if (*word == '\0' && treeNode->occurrences.size() != 0)
    {
        // Word found
        return treeNode;
    }
    else
    {
        // Word not found
        return NULL;
    }
}

// Searches the word first, if not found, does nothing
// if found, deletes the nodes corresponding to the word
void RemoveWord(struct Node * trieTree, char * word)
{
    struct Node * trieNode = SearchWord(trieTree, word);

    if (trieNode == NULL)
    {
        // Word not found
        return;
    }

    trieNode->occurrences.pop_back();    // Deleting the occurence

    // 'noChild' indicates if the node is a leaf node
    bool noChild = true;

    int childCount = 0;
    // 'childCount' has the number of children the current node
    // has which actually tells us if the node is associated with
    // another word .This will happen if 'childCount' != 0.
    int i;

    // Checking children of current node
    for (i = 0; i < ALPHABETS; ++i)
    {
        if (trieNode->children[i] != NULL)
        {
            noChild = false;
            ++childCount;
        }
    }

    if (!noChild)
    {
        // The node has children, which means that the word whose
        // occurrence was just removed is a Suffix-Word
        // So, logically no more nodes have to be deleted
        return;
    }

    struct Node * parentNode;     // variable to assist in traversal

    while (trieNode->occurrences.size() == 0 && trieNode->parent != NULL && childCount == 0)
    {
        // trieNode->occurrences.size() -> tells if the node is associated with another word
        //
        // trieNode->parent != NULL -> is the base case sort-of condition, we simply ran
        // out of nodes to be deleted, as we reached the root
        //
        // childCount -> does the same thing as explained in the beginning, to every node
        // we reach

        childCount = 0;
        parentNode = trieNode->parent;

        for (i = 0; i < ALPHABETS; ++i)
        {
            if (parentNode->children[i] != NULL)
            {
                if (trieNode == parentNode->children[i])
                {
                    // the child node from which we reached
                    // the parent, this is to be deleted
                    parentNode->children[i] = NULL;
                    free(trieNode);
                    trieNode = parentNode;
                }
                else
                {
                    ++childCount;
                }
            }
        }
    }
}

// Prints the 'trieTree' in a Pre-Order or a DFS manner
// which automatically results in a Lexicographical Order
void LexicographicalPrint(struct Node * trieTree, vector<char> word)
{
    if(trieTree->occurrences.size()==0) return;
    int i;
    bool noChild = true;

    if (trieTree->occurrences.size() != 0)
    {
        // Condition trie_tree->occurrences.size() != 0,
        // is a neccessary and sufficient condition to
        // tell if a node is associated with a word or not

        vector<char>::iterator charItr = word.begin();

        while (charItr != word.end())
        {
            printf("%c", *charItr);
            ++charItr;
        }
        printf(" -> @ index -> ");

        vector<int>::iterator counter = trieTree->occurrences.begin();
        // This is to print the occurences of the word

        while (counter != trieTree->occurrences.end())
        {
            printf("%d, ", *counter);
            ++counter;
        }

        printf("\n");
    }

    for (i = 0; i < ALPHABETS; ++i)
    {
        if (trieTree->children[i] != NULL)
        {
            noChild = false;
            word.push_back(CASE + i);   // Select a child

            // and explore everything associated with the cild
            LexicographicalPrint(trieTree->children[i], word);
            word.pop_back();
            // Remove the alphabet as we dealt
            // everything associated with it
        }
    }

    word.pop_back();
}

int main()
{
    int n, i=0;
    vector<char> printUtil;       // Utility variable to print tree
    ifstream file;
    file.open("temp.txt");

    // Creating the Trie Tree using calloc
    // so that the components are initialised
    struct Node * trieTree = (struct Node *) calloc(1, sizeof(struct Node));
    string w;
    while(file>>w)
    {
        if(!Dictionary_word(w)) continue;
        w=All_lower_case(w);
        InsertWord(trieTree,(char*)w.c_str(), i++);
    }

    printf("Done\n");   // Just to make the output more readable

    string se = "mad";
    if(SearchWord(trieTree,(char*)se.c_str())==NULL) cout<<"not found"<<endl;

    LexicographicalPrint(trieTree,printUtil);


    return 0;
}
