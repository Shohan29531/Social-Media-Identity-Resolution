#include<bits/stdc++.h>

using namespace std;

//calculate the cosine similarity of two vectors
double cosine_similarity_score(vector<double> string1, vector<double> string2)
{
    int size=string1.size();
    double numerator=0;
    double denominator_1=0;
    double denominator_2=0;
    for(int i=0; i<size; i++)
    {
        numerator+=string1[i]*string2[i];
        denominator_1+=string1[i]*string1[i];
        denominator_2+=string2[i]*string2[i];
    }
    if(denominator_1 && denominator_2) return numerator/(sqrt(denominator_1)*sqrt(denominator_2));
    //return an invalid value in case of an empty string
    else return -1;
}

//calculate mean
double mean(vector<double> numbers)
{
    double mean=0;
    int size=numbers.size();
    if(size==0) return -1.0;

    for(int i=0; i<size; i++)
    {
        mean+=numbers[i];
    }

    return (double)(mean/(double)size);
}


//calculate standard deviation

double standard_deviation(vector<double> numbers)
{
    double mean=mean(numbers);
    double SD=0;
    int size=numbers.size();
    for(int i=0; i<size; i++){
        SD+=pow(numbers[i]-mean,2);
    }
    SD/=(double)size;
    SD=sqrt(SD);

    return SD;
}
