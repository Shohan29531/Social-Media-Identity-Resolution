#include<bits/stdc++.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

#define ALPHABETS 26
#define CASE 'a'
#define MAX_WORD_SIZE 25

//global json object to store the output which will be written to file later.
json json_output;
//Index for inserting dictionary words
int dictionary_words=0;
int start_with_capital=0;
int upper_case_letters=0;
//The output file-stream, global because more than one method uses this to write
//into the output file
ofstream output;
//A simple class that holds a string and its frequencies
class word_ext_and_freq;
//A class holding special words and frequencies
class special_and_freq;
//Word extensions. loaded from an input file.
//vector<word_ext_and_freq> word_extensions;
//Special chars and punctuations, loaded from an input file
vector<special_and_freq> special_chars;
//Emojis, Currently loaded from input file
//change and do using regex
vector<word_ext_and_freq> emojis;
//All the hashtags and their frequencies
vector<word_ext_and_freq> hashtags;
//Mention in twitter is detected only by a @ character
vector<word_ext_and_freq> mentions;
//URLs and their frequencies
vector<word_ext_and_freq> all_urls;
//Di-grams
vector<word_ext_and_freq> di_grams;
//tri-grams
vector<word_ext_and_freq> tri_grams;
//tri-grams
vector<word_ext_and_freq> stop_words;
//

vector<word_ext_and_freq> all_capital_words;
vector<word_ext_and_freq> alphanumeric_words;
vector<word_ext_and_freq> numeric_words;
vector<word_ext_and_freq> word_extensions;

//

struct Node
{
    struct Node * parent;
    struct Node * children[ALPHABETS];
    vector<int> occurrences;
};

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
void LexicographicalPrint(struct Node * trieTree, vector<char> word,int &_unique, int &total)
{
    int i;
    bool noChild = true;

    if (trieTree->occurrences.size() != 0)
    {
        // Condition trie_tree->occurrences.size() != 0,
        // is a neccessary and sufficient condition to
        // tell if a node is associated with a word or not

        if(trieTree->occurrences.size() >= 2)
        {
            _unique++;
            vector<char>::iterator charItr = word.begin();
            while (charItr != word.end())
            {
                printf("%c", *charItr);
                output<<*charItr;
                ++charItr;

            }
            total+=(trieTree->occurrences.size()-1);
            cout<<"-->"<<trieTree->occurrences.size()-1<<endl;
            output<<"-->"<<trieTree->occurrences.size()-1<<endl;
        }
    }

    for (i = 0; i < ALPHABETS; ++i)
    {
        if (trieTree->children[i] != NULL)
        {
            noChild = false;
            word.push_back(CASE + i);   // Select a child

            // and explore everything associated with the cild
            LexicographicalPrint(trieTree->children[i], word,_unique,total);
            word.pop_back();
            // Remove the alphabet as we dealt
            // everything associated with it
        }
    }

    word.pop_back();
}



//print functions to do the printing
void print(vector<word_ext_and_freq> v);
void print(vector<special_and_freq> v);

//Returns total words along with total distinct words
void diversity_counter(vector<word_ext_and_freq> v, int &_unique, int &total);
//Split a sentence with a delimiter specified
void split(const  string &s, char delim,  vector< string> &elems)
{
    stringstream ss;
    ss.str(s);
    string item;
    while ( getline(ss, item, delim))
    {
        elems.push_back(item);
    }
}

//helps with the splitting task listed just above
vector< string> split(const  string &s, char delim)
{
    vector< string> elems;
    split(s, delim, elems);
    return elems;
}


class word_ext_and_freq
{
public:
    string word_ext;
    int freq;
    word_ext_and_freq(string s, int i)
    {
        word_ext=s;
        freq=i;
    }
};

class special_and_freq
{
public:
    char special;
    int freq;
    special_and_freq(char c, int i)
    {
        special=c;
        freq=i;
    }
};

//convert a string to all lower case
string All_lower_case(string s)
{
    for(int i=0; i<s.size(); i++)
    {
        if((s[i]>='A' && s[i]<='Z') || (s[i]>='a' && s[i]<='z')) s[i]=tolower(s[i]);
    }
    return s;
}

//a dictionary word tester function
//all chars must be alphabets
bool Dictionary_word(string s)
{
    for(int i=0; i<s.size(); i++)
    {
        if((s[i]>='A' && s[i]<='Z') || (s[i]>='a' && s[i]<='z')) continue;
        else return false;
    }
    return true;
}

//Return the number of occurrences of a substring 'sub' in a string 's'
int Substring_occurences(string s, string sub)
{
    int n=sub.size();
    int occur=0;
    int index=s.find(sub);
    while(1)
    {
        if(index!=string::npos)
        {
            occur++;
            index=s.find(sub,index+1);
        }
        else break;
    }
    return occur;
}

void diversity_counter(vector<word_ext_and_freq> v, int &_unique, int &total)
{
    _unique=0;
    total=0;

    for(int i=0; i<v.size(); i++)
    {
        if(v[i].freq>0)
        {
            _unique++;
            total+=v[i].freq;
        }
    }

}


//DO IT USING REGEX
//emojis and their frequencies
//if not separated by at least one space, candidate emojis are not considered
void Emoji_analysis(string s, int &total)
{

    vector<string> tokens = split(s,' ');
    total=0;
    for(int i=0; i<tokens.size(); i++)
    {
        for(int j=0; j<emojis.size(); j++)
        {
            if(emojis[j].word_ext==tokens[i])
            {
                emojis[j].freq++;
                total++;
                break;
            }
        }
    }
}

//URL finder and counter, done using regex library
int number_of_urls(string s)
{
    regex word_regex("((ftp|http|https)://)?(www\\.)[^\\s]*|((ftp|http|https)://)(www\\.)?[^\\s]*");


    auto words_begin = sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end = sregex_iterator();
    for (sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();

        bool is_new=true;

        for(int k=0; k<all_urls.size(); k++)
        {
            if(match_str==all_urls[k].word_ext)
            {
                all_urls[k].freq++;
                is_new=false;
            }
        }
        if(is_new)
        {
            word_ext_and_freq* temp=new word_ext_and_freq(match_str,1);
            all_urls.push_back(*temp);
        }

        //cout << "  " << match_str << '\n';

    }

    return distance(words_begin, words_end);
}



//find the punctuations in a sentence
int puncs(string s)
{
    vector<string> punctuations;
    regex word_regex("[[:punct:]]+");
    auto words_begin =
        sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end =  sregex_iterator();
    for ( sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();

        punctuations.push_back(match_str);
    }
    bool removed=false;
    for(int i=0; i<punctuations.size(); i++)
    {
        if(removed)
        {
            i--;
            removed=false;
        }
        for(int j=0; j<emojis.size(); j++)
        {
            if(punctuations[i]==emojis[j].word_ext)
            {
                punctuations.erase(punctuations.begin()+i);
                removed=true;
            }
        }
    }
    return punctuations.size();
}


//number of words in a sentence
//hashtags and their frequencies
vector<string> Words_in_a_sentence(string s,int &punc)
{
    //dummy
    punc=0;

    vector<string> words;
    regex word_regex("([@#])?[a-zA-Z0-9_']+");
    auto words_begin =
        sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end =  sregex_iterator();
    for ( sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();

        words.push_back(match_str);
        //cout<<match_str<<endl;
    }
    return words;
}


bool valid_character(char c)
{
    if(!((c>='A' && c<='Z' )||(c>='a' && c<='z')||(c>='0' && c<='9')))
    {
        for(int k=0; k<special_chars.size(); k++)
        {
            if(c==special_chars[k].special)
            {
                return true;
            }
        }
        if(c==' ') return true;
        return false;
    }
    else
    {
        return true;
    }
}
// Analyze a post by counting the number of sentences in it
// number of upper case letters
// number of quotations
// number of special characters, digits and their frequencies
vector<string> Sentences_in_a_post(string s)
{
    for(int i=0; i<s.size(); i++)
    {
        for(int k=0; k<special_chars.size(); k++)
        {
            if(s[i]==special_chars[k].special)
            {
                special_chars[k].freq++;
                break;
            }
        }
        if(s[i]>='A' && s[i]<='Z') upper_case_letters++;
    }
    vector<string> sentences;

    regex word_regex("([\\s]*)?[^{\\.!\\?}]+[!\\?\\.]+");
    auto words_begin =
        sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end =  sregex_iterator();

    for ( sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();

        sentences.push_back(match_str);
    }

    for(int i=0; i<sentences.size(); i++)
    {
        //front checked
        for(int j=0; j<sentences[i].size(); j++)
        {
            if(sentences[i][j]!=' ' && sentences[i][j]!='\t' && sentences[i][j]!='\n') break;
            else
            {
                sentences[i]=sentences[i].substr(1);
            }
        }
        //back checked
        for(int j=sentences[i].size()-1; j>=0; j--)
        {
            if(sentences[i][j]=='?' || sentences[i][j]=='.'||sentences[i][j]=='!')
            {
                if(sentences[i][j-1]!='?' && sentences[i][j-1]!='.' && sentences[i][j-1]!='!')
                {
                    break;
                }
                else
                {
                    sentences[i]=sentences[i].substr(0,sentences[i].size()-1);
                }
            }
        }
    }


    for(int i=0; i<sentences.size(); i++)
    {
        if(sentences[i][0]>='A' && sentences[i][0]<='Z') start_with_capital++;
    }

    //punctuation count analysis
    /*for(int i=0; i<sentences.size(); i++)
    {
        cout<<sentences[i]<<"->"<<puncs(sentences[i])<<endl;
        output<<sentences[i]<<"->"<<puncs(sentences[i])<<endl;
    }*/

    return sentences;
}
//count the number of hash_tags(#) in a post
int Number_of_hashtags(string s)
{
    int count=0;
    for(int i=0; i<s.size(); i++)
    {
        if(s[i]=='#') count++;
    }
    return count;
}

//test whether a word is formed using all capital letters
bool All_capital_test(string s)
{
    for(int i=0; i<s.size(); i++)
    {
        if(s[i]<'A' || s[i]>'Z') return false;
    }
    return true;
}

bool numeric_test(string s)
{
    for(int i=0; i<s.size(); i++)
    {
        if((s[i]<'0' || s[i]>'9')) return false;
    }
    return true;
}

//count quotations
int quotation_count(string s)
{
    regex word_regex("\"([^\"]*)\"");
    auto words_begin = sregex_iterator(s.begin(), s.end(), word_regex);
    auto words_end =  sregex_iterator();
    int count=0;
    for ( sregex_iterator i = words_begin; i != words_end; ++i)
    {
        smatch match = *i;
        string match_str = match.str();
        //cout<<match_str<<endl;
        //output<<match_str<<endl;
        count++;
    }
    return count;
}

//Alphanumeric character detection
bool Alphanumeric_test(string s)
{
    bool alpha=false;
    bool numeric =false;
    for(int i=0; i<s.size(); i++)
    {
        if((s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z') || (s[i]>='0' && s[i]<='9'))
        {
            if( (s[i]>='a' && s[i]<='z') || (s[i]>='A' && s[i]<='Z') ) alpha=true;
            else if((s[i]>='0' && s[i]<='9')) numeric=true;

        }
        else
            return false;
    }

    return alpha&numeric;
}



bool is_stop_word(string s)
{
    for(int i=0;i<stop_words.size();i++){
        if(stop_words[i].word_ext==s) return true;
    }
    return false;
}
//compute frequency of dictionary words
//compute frequency of word extensions
//compute lexical diversity
//compute the number of sentences beginning with a capital letter
//
void Complete_word_analysis(string s,Node * trieTree)
{
    vector<string> sentences = Sentences_in_a_post(s);
    int total_words=0;
    int first_letter_capital=0;
    //URL analysis
    int urls=number_of_urls(s);

    for(int i=0; i<sentences.size(); i++)
    {
        int punc;

        vector<string> words = Words_in_a_sentence(sentences[i],punc);
        //cout<<endl;
        //cout<<sentences[i]<<"."<<"-->"<<punc<<endl;

        total_words+=words.size();

        if(words.size()==0) continue;


        if(words[0][0]>='A' && words[0][0]<='Z') first_letter_capital++;

        //cout<<words.size()<<endl;
        for(int j=0; j<words.size(); j++)
        {
            if(is_stop_word(words[j])) continue;
            //numeric test
            if(numeric_test(words[j]))
            {
                bool found=false;
                for(int m=0; m<numeric_words.size(); m++)
                {
                    if(words[j]==numeric_words[m].word_ext)
                    {
                        numeric_words[m].freq++;
                        found=true;
                        break;
                    }
                }

                if(!found)
                {
                    word_ext_and_freq *t = new word_ext_and_freq(words[j],1);
                    numeric_words.push_back(*t);
                }
                continue;
            }



            //alphanumeric test
            if(Alphanumeric_test(words[j]))
            {
                bool found=false;
                for(int m=0; m<alphanumeric_words.size(); m++)
                {
                    if(words[j]==alphanumeric_words[m].word_ext)
                    {
                        alphanumeric_words[m].freq++;
                        found=true;
                        break;
                    }
                }

                if(!found)
                {
                    word_ext_and_freq *t = new word_ext_and_freq(words[j],1);
                    alphanumeric_words.push_back(*t);
                }
                continue;
            }

            //hashtags
            if(words[j][0]=='#')
            {
                bool ff = false;
                for(int k=0; k<hashtags.size(); k++)
                {
                    if(words[j]==hashtags[k].word_ext)
                    {
                        hashtags[k].freq++;
                        ff=true;
                        break;
                    }

                }
                if(!ff)
                {
                    word_ext_and_freq *t = new word_ext_and_freq(words[j],1);
                    hashtags.push_back(*t);
                }
                continue;
            }


            //mentions
            if(words[j][0]=='@')
            {
                bool ff = false;
                for(int k=0; k<mentions.size(); k++)
                {
                    if(words[j]==mentions[k].word_ext)
                    {
                        mentions[k].freq++;
                        ff=true;
                        break;
                    }

                }
                if(!ff)
                {
                    word_ext_and_freq *t = new word_ext_and_freq(words[j],1);
                    mentions.push_back(*t);
                }
                continue;
            }

            //all capital test
            if(All_capital_test(words[j]))
            {
                bool found=false;
                for(int m=0; m<all_capital_words.size(); m++)
                {
                    if(words[j]==all_capital_words[m].word_ext)
                    {
                        all_capital_words[m].freq++;
                        found=true;
                        break;
                    }
                }
                if(!found)
                {
                    word_ext_and_freq *t = new word_ext_and_freq(words[j],1);
                    all_capital_words.push_back(*t);
                }
            }
            //handling tri-grams
            // cout<<words[j]<<endl;
            if(words.size()>=3 && j<=words.size()-3)
            {
                if(Dictionary_word(words[j]) && Dictionary_word(words[j+1]) && Dictionary_word(words[j+2]) )
                {
                    string temp="";
                    temp+=All_lower_case(words[j]);
                    temp+=" ";
                    temp+=All_lower_case(words[j+1]);
                    temp+=" ";
                    temp+=All_lower_case(words[j+2]);
                    //cout<<temp<<endl;
                    bool found=false;
                    for(int x=0; x<tri_grams.size(); x++)
                    {
                        if(tri_grams[x].word_ext==temp)
                        {
                            tri_grams[x].freq++;
                            found=true;
                        }
                    }
                    if(!found)
                    {
                        word_ext_and_freq *t = new word_ext_and_freq(temp,1);
                        //cout<<temp<<endl;
                        tri_grams.push_back(*t);

                    }
                }
            }

            //handling Di-grams
            if(words.size()>=2 &&j<=words.size()-2)
            {
                if(Dictionary_word(words[j]) && Dictionary_word(words[j+1]))
                {
                    string temp="";
                    temp+=All_lower_case(words[j]);
                    temp+=" ";
                    temp+=All_lower_case(words[j+1]);
                    bool found=false;
                    //cout<<temp<<endl;
                    for(int x=0; x<di_grams.size(); x++)
                    {
                        if(di_grams[x].word_ext==temp)
                        {
                            di_grams[x].freq++;
                            found=true;
                        }
                    }
                    if(!found)
                    {
                        word_ext_and_freq *t = new word_ext_and_freq(temp,1);
                        di_grams.push_back(*t);
                    }
                }
            }
            //Dictionary words and word extensions
            string se=words[j];
            if(Dictionary_word(words[j]))
            {
                se = All_lower_case(words[j]);
                if(SearchWord(trieTree,(char*)se.c_str())==NULL)
                {
                    //If not found in the dictionary
                    //it's a word extension
                    bool found=false;
                    for(int x=0; x<word_extensions.size(); x++)
                    {
                        if(word_extensions[x].word_ext==se)
                        {
                            word_extensions[x].freq++;
                            found=true;
                        }
                    }
                    if(!found)
                    {
                        word_ext_and_freq *t = new word_ext_and_freq(se,1);
                        word_extensions.push_back(*t);
                    }
                }
                else
                {
                    //If found in the dictionary, just increase the freq
                    //it's a dictionary word
                    InsertWord(trieTree,(char*)se.c_str(),dictionary_words++);
                }
            }
            else
            {
                bool found=false;
                for(int x=0; x<word_extensions.size(); x++)
                {
                    if(word_extensions[x].word_ext==se)
                    {
                        word_extensions[x].freq++;
                        found=true;
                    }
                }
                if(!found)
                {
                    word_ext_and_freq *t = new word_ext_and_freq(se,1);
                    word_extensions.push_back(*t);
                }
            }
        }
    }

    // wrong currently, correct this
    // add all the unique ones
    //double lexical_diversity= (double)(dictionary_words.size()+we)/total_words;

    int total_emoji;
    Emoji_analysis(s,total_emoji);

    int _unique=0;
    int total=0;
    int q=quotation_count(s);

    //Job done, now print everything
    vector<char> printUtil;
    cout<<"             Dictionary Words with frequency:"<<endl;
    output<<"             Dictionary Words with frequency:"<<endl;
    LexicographicalPrint(trieTree,printUtil,_unique,total);

    int u,t;
    //
    diversity_counter(all_capital_words,u,t);
    _unique+=u;
    total+=t;
    //
    diversity_counter(alphanumeric_words,u,t);
    _unique+=u;
    total+=t;
    //
    diversity_counter(numeric_words,u,t);
    _unique+=u;
    total+=t;
    //
    diversity_counter(hashtags,u,t);
    _unique+=u;
    total+=t;
    //
    diversity_counter(mentions,u,t);
    _unique+=u;
    total+=t;
    //
    diversity_counter(word_extensions,u,t);
    _unique+=u;
    total+=t;
    //
    double Lexical_diversity=(double)_unique/(double)total;


    cout<<"             All Capital Words with frequency:"<<endl;
    output<<"             All Capital Words with frequency:"<<endl;
    print(all_capital_words);
    cout<<"             Alphanumeric Words with frequency:"<<endl;
    output<<"             Alphanumeric Words with frequency:"<<endl;
    print(alphanumeric_words);
    cout<<"             Numeric Words with frequency:"<<endl;
    output<<"             Numeric Words with frequency:"<<endl;
    print(numeric_words);
    cout<<"             Word Extensions with frequency:"<<endl;
    output<<"             Word Extensions with frequency:"<<endl;
    print(word_extensions);
    cout<<"             Special Characters with frequency:"<<endl;
    output<<"             Special Characters with frequency:"<<endl;
    print(special_chars);
    cout<<"             Emojis with frequency:"<<endl;
    output<<"             Emojis with frequency:"<<endl;
    print(emojis);
    cout<<"             Hash tag Collection with frequency:"<<endl;
    output<<"             Hash tag Collection with frequency:"<<endl;
    print(hashtags);
    cout<<"             Mentions with frequency:"<<endl;
    output<<"             Mentions with frequency:"<<endl;
    print(mentions);
    cout<<"             URLs with frequencies:"<<endl;
    output<<"             URLs with frequencies:"<<endl;
    print(all_urls);
    cout<<"             Di-grams frequencies:"<<endl;
    output<<"             Di-grams frequencies:"<<endl;
    print(di_grams);
    cout<<"             Tri-grams frequencies:"<<endl;
    output<<"             Tri-grams frequencies:"<<endl;
    print(tri_grams);
    cout<<"             Total sentences in the post : "<<sentences.size()<<endl;
    output<<"             Total sentences in the post : "<<sentences.size()<<endl;
    cout<<"             Total words in the post : "<<total<<endl;
    output<<"             Total words in the post : "<<total<<endl;

    cout<<"             Number of Hashtags : "<<Number_of_hashtags(s)<<endl;
    output<<"             Number of Hashtags : "<<Number_of_hashtags(s)<<endl;
    cout<<"             Number of Emojis : "<<total_emoji<<endl;
    output<<"             Number of Emojis : "<<total_emoji<<endl;
    cout<<"             Number of URLs : "<<urls<<endl;
    output<<"             Number of URLs : "<<urls<<endl;
    cout<<"             Number of quotations : "<<q<<endl;
    output<<"             Number of quotations : "<<q<<endl;
    cout<<"             Sentences starting with capital : "<<start_with_capital<<endl;
    output<<"             Sentences starting with capital : "<<start_with_capital<<endl;
    cout<<"             Total Upper case letters : "<<upper_case_letters<<endl;
    output<<"             Total Upper case letters : "<<upper_case_letters<<endl;

    cout<<"             Lexical_diversity : "<<Lexical_diversity<<endl;
    output<<"             Lexical_diversity : "<<Lexical_diversity<<endl;

    //cout<<total<<"  "<<_unique<<endl;
}

//Print function-1
void print(vector<word_ext_and_freq> v)
{
    cout<<endl<<endl;
    for(int i=0; i<v.size(); i++)
    {
        cout<<v[i].word_ext<<"--->"<<v[i].freq<<endl;
        output<<v[i].word_ext<<"--->"<<v[i].freq<<endl;
    }
    output<<endl<<endl;
}
//Print function-2
void print(vector<special_and_freq> v)
{
    cout<<endl<<endl;
    for(int i=0; i<v.size(); i++)
    {
        cout<<v[i].special<<"--->"<<v[i].freq<<endl;
        output<<v[i].special<<"--->"<<v[i].freq<<endl;
    }
    output<<endl<<endl;
}

void Run(string input_file, string output_file)
{
    //
    ifstream file("stopwords.txt");
    //All special characters in a file
    ifstream file2("All_specials.txt");
    //All emojis in a file
    ifstream file3("emojis.txt");
    ifstream file4;
    //All dictionary words in a file
    file4.open("words3.txt");

    string line3;
    string line2;
    string line;

    //Read
    while(getline(file, line))
    {
        istringstream iss(line);
        string token;
        while(getline(iss, token, '\n'))
        {
            token=All_lower_case(token);
            word_ext_and_freq* temp =new word_ext_and_freq(token,0);
            stop_words.push_back(*temp);
        }
    }


    //Read the special chars into the vector
    while(getline(file2, line2))
    {
        istringstream iss(line2);
        string token;
        while(getline(iss, token, ' '))
        {
            char *c = (char *)token.c_str();
            special_and_freq* temp =new special_and_freq(*c,0);
            special_chars.push_back(*temp);
        }
    }

    // Read the emojis into the vector
    while(getline(file3, line3))
    {
        istringstream iss(line3);
        string token;
        while(getline(iss, token, ' '))
        {
            word_ext_and_freq* temp =new word_ext_and_freq(token,0);
            emojis.push_back(*temp);
        }
    }
    //trie data structure for storing the dictionary
    struct Node * trieTree = (struct Node *) calloc(1, sizeof(struct Node));
    string w;

    //Load the dictionary from the file
    while(file4>>w)
    {
        if(!Dictionary_word(w)) continue;
        w=All_lower_case(w);
        if(SearchWord(trieTree,(char*)w.c_str())==NULL) InsertWord(trieTree,(char*)w.c_str(), dictionary_words++);
    }
    //input file
    ifstream t(input_file);
    stringstream buffer;
    buffer << t.rdbuf();
    string s = buffer.str();
    s+=".";

    output.open(output_file);
    Complete_word_analysis(s,trieTree);
    file2.close();
    file3.close();
    file4.close();
    t.close();
    output.close();
}

void Run_using_string(string input_string, string output_file)
{
    ifstream file("stopwords.txt");
    //All special characters in a file
    ifstream file2("All_specials.txt");
    //All emojis in a file
    ifstream file3("emojis.txt");
    ifstream file4;
    //All dictionary words in a file
    file4.open("words3.txt");

    string line3;
    string line2;
    string line;

    //Read
    while(getline(file, line))
    {
        istringstream iss(line);
        string token;
        while(getline(iss, token, '\n'))
        {
            token=All_lower_case(token);
            word_ext_and_freq* temp =new word_ext_and_freq(token,0);
            stop_words.push_back(*temp);
        }
    }

    //Read the special chars into the vector
    while(getline(file2, line2))
    {
        istringstream iss(line2);
        string token;
        while(getline(iss, token, ' '))
        {
            char *c = (char *)token.c_str();
            special_and_freq* temp =new special_and_freq(*c,0);
            special_chars.push_back(*temp);
        }
    }

    // Read the emojis into the vector
    while(getline(file3, line3))
    {
        istringstream iss(line3);
        string token;
        while(getline(iss, token, ' '))
        {
            word_ext_and_freq* temp =new word_ext_and_freq(token,0);
            emojis.push_back(*temp);
        }
    }
    //trie data structure for storing the dictionary
    struct Node * trieTree = (struct Node *) calloc(1, sizeof(struct Node));
    string w;

    //Load the dictionary from the file
    while(file4>>w)
    {
        if(!Dictionary_word(w)) continue;
        w=All_lower_case(w);
        if(SearchWord(trieTree,(char*)w.c_str())==NULL) InsertWord(trieTree,(char*)w.c_str(), dictionary_words++);
    }

    output.open(output_file);
    Complete_word_analysis(input_string,trieTree);
    file2.close();
    file3.close();
    file4.close();
    //t.close();
    output.close();
}


void Run_using_string_output_as_json(string input_string, string output_file)
{
    ifstream file("stopwords.txt");
    //All special characters in a file
    ifstream file2("All_specials.txt");
    //All emojis in a file
    ifstream file3("emojis.txt");
    ifstream file4;
    //All dictionary words in a file
    file4.open("words3.txt");

    string line3;
    string line2;
    string line;

    //Read
    while(getline(file, line))
    {
        istringstream iss(line);
        string token;
        while(getline(iss, token, '\n'))
        {
            token=All_lower_case(token);
            word_ext_and_freq* temp =new word_ext_and_freq(token,0);
            stop_words.push_back(*temp);
        }
    }
    //Read the special chars into the vector
    while(getline(file2, line2))
    {
        istringstream iss(line2);
        string token;
        while(getline(iss, token, ' '))
        {
            char *c = (char *)token.c_str();
            special_and_freq* temp =new special_and_freq(*c,0);
            special_chars.push_back(*temp);
        }
    }

    // Read the emojis into the vector
    while(getline(file3, line3))
    {
        istringstream iss(line3);
        string token;
        while(getline(iss, token, ' '))
        {
            word_ext_and_freq* temp =new word_ext_and_freq(token,0);
            emojis.push_back(*temp);
        }
    }
    //trie data structure for storing the dictionary
    struct Node * trieTree = (struct Node *) calloc(1, sizeof(struct Node));
    string w;

    //Load the dictionary from the file
    while(file4>>w)
    {
        if(!Dictionary_word(w)) continue;
        w=All_lower_case(w);
        if(SearchWord(trieTree,(char*)w.c_str())==NULL) InsertWord(trieTree,(char*)w.c_str(), dictionary_words++);
    }

    output.open(output_file);
    Complete_word_analysis(input_string,trieTree);
    file2.close();
    file3.close();
    file4.close();
    //t.close();
    output.close();
}
