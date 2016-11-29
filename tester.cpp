#include<bits/stdc++.h>
#include "library.h"
#include "json.hpp"
using namespace std;


void Run_simulation_from_json_file(string input_file, string output_file)
{
    json p;
    ifstream disqus_comments(input_file);
    disqus_comments>>p;
    string s="";

    for(int i=0; i<p["comments"].size(); i++)
    {
        s+=".";
        s+=p["comments"][i]["post"];
    }
    s+=".";
    disqus_comments.close();
    Run_using_string_output_as_json(s,output_file);
}




int main()
{
    Run_simulation_from_json_file("json_input/temp.json","json_output/test_output_temp.txt");


    ofstream myfile;
    myfile.open ("json_output/test_output_temp2.json",ios::binary);

    json j2 =
    {
        {"pi", 3.141},
        {"happy", true},
        {"name", "Niels"},
        {"nothing", nullptr},
        {
            "answer", {
                {"everything", 42}
            }
        },
        {"list", {1, 0, 2}},
        {
            "object", {
                {"currency", "USD"},
                {"value", 42.99}
            }
        }
    };

    myfile<<j2;


    json j;
    j.push_back("foo");
    j.push_back(1);
    j.push_back(true);

// iterate the array
    for (json::iterator it = j.begin(); it != j.end(); ++it)
    {
        std::cout << *it << '\n';
    }


    return 0;
}
