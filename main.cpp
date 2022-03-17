#include <iostream>
#include <vector>
#include <map>
#include <bits/stdc++.h>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
///20170090-20180308-20180131
using namespace std;

int calculteFitness(string chromosome, int number,
                    vector <pair<int,int>>knapSack)
{
    int fitness=0;
    for(int j=0; j<number; j++)
    {
        int x=(int)chromosome[j]-'0';
        fitness=fitness+(x*knapSack[j].first);
    }
    return fitness;
}

int calculteValue(string chromosome, int number,
                    vector <pair<int,int>>knapSack)
{
    int fitness=0;
    for(int j=0; j<number; j++)
    {
        int x=(int)chromosome[j]-'0';
        fitness=fitness+(x*knapSack[j].second);
    }
    return fitness;
}

string subString(string str, int start,int endd)
{
    string finall="";
    for(int i=start;i<endd;i++)
        finall+=str[i];
    return finall;
}
string replaceString(string first, string second ,int start,int endd)
{
    string finall="";
    for(int i=0;i<start;i++)
        finall+=first[i];
    finall+=second;
    for(int i=start+second.length();i<endd;i++)
        finall+=first[i];
    return finall;
}

set<int> HighestFitInGen;

int main()
{

    vector<int> cases;
    srand(time(NULL));
    int terminate;

    int t;
    cin>>t;
    while(t--)
    {

        int number,weight,a,b,fitness=0;
        cin>>number>>weight;
        terminate=number*1.5;
        vector<pair<int,int>>knapSack; ///weight- benefit
        for(int i=0; i<number; i++)
        {
            cin>>a>>b;
            knapSack.push_back(pair<int,int>(a,b));
        }
        ///get generation
        int populationNumber;
        populationNumber=rand()%number+(number/2);
        //cout<<"popnum"<<populationNumber<<endl;
        vector<pair<string,int>>generation;
        for(int i=0; i<populationNumber; i++)
        {
            string chromosome="";
            int y;
            for(int j=0; j<number; j++)
            {
                y=rand()%5;
             //   cout<<"DDDD"<<y<<endl;
                if(y>2)
                    chromosome=chromosome+'1';
                else
                    chromosome=chromosome+'0';
            }
            fitness=calculteFitness(chromosome,number,knapSack);
            //cout<<"Fit: "<<fitness<<endl;
            if(fitness<=weight)
                generation.push_back(pair<string,int>(chromosome,fitness));
            else
                i--;
        }

        ///print generation
        /*for(int i=0; i<populationNumber; i++)
        {
            cout<<"i+1  "<<generation[i].first<<"  "<<generation[i].second<<endl;
        }*/
        while(terminate--)
        {

            int commulative[populationNumber];
            string chromosome1=generation[0].first;
            commulative[0]=generation[0].second;
            for (int i=1; i<populationNumber; i++)
            {
                // string chromosome=generation[i].first;
                commulative[i]=commulative[i-1]+generation[i].second;
            }
            ///random number to choose 2 chromosome for selection

            vector <pair<string,string>> offspring;
            string firstgen="";
            string secondgen="";

            for(int i=0; i<populationNumber/2; i++)
            {
                firstgen=generation[i].first;
                secondgen=generation[populationNumber-i-1].first;
                //cout<<"Size "<<firstgen.length()<<"   "<<secondgen.length()<<endl;
                offspring.push_back(pair<string,string>(firstgen,secondgen));
            }

            int xc=rand()%(number+1-1)+1;
            cout<<"xc:: "<<xc<<endl;
            double pc=0.6;

            ///perform crossover
            vector <string> newOffSpring;
            for(int i=0; i<offspring.size(); i++)
            {
                double rc=rand()%1;
                if (rc<=pc)
                {
                    string part1,part2,first,second;
                    first=offspring[i].first;
                    second=offspring[i].second;
                    //cout<<"Size"<<first.length()<<"   "<<second.length()<<endl;
                    part1=first.substr(xc,first.length()-1);
                    part2=second.substr(xc,second.length()-1);
                    first=first.replace(xc,first.length()-xc,part2);
                    second=second.replace(xc,second.length()-xc,part1);

                    /*first=replaceString(first, part2, xc, first.length()-xc);
                    second=replaceString(second, part1, xc, second.length()-xc);*/

                    offspring[i].first=first;
                    offspring[i].second=second;
                    //cout<<"xc is "<<xc<<endl;
                    //cout<<offspring[i].first<<" "<<offspring[i].second<<endl;

                }
                newOffSpring.push_back(offspring[i].first);
                newOffSpring.push_back(offspring[i].second);
            }


            ///mutation
            string offspringChrom;
            int c=number;
            for(int i=0; i<newOffSpring.size(); i++)
            {
                offspringChrom=newOffSpring[i];
                for(int j=0; j<offspringChrom.size(); j++)
                {
                    int randum= rand()%4;
                    if(randum>2)
                    {
                        if(offspringChrom[j]=='0')
                            offspringChrom[j]='1';
                        else
                            offspringChrom[j]='0';
                    }

                }
                ///new fitness
                int newfit=calculteFitness(offspringChrom,number,knapSack);
                //cout<<"NewFittttttttttt "<<newfit<<endl;
                if(newfit>weight&&c==0)
                {
                   // cout<<"before: "<<*(newOffSpring.begin()+i)<<endl;
                    newOffSpring.erase(newOffSpring.begin()+i);
                    //cout<<"After: "<<*(newOffSpring.begin()+i)<<endl;
                    i--;
                    c=number;
                }
                else if(newfit>weight&&c>0)
                {
                    newOffSpring[i]=offspringChrom;
                    i--;
                    c--;
                }
                else
                {
                        c=number;
                        newOffSpring[i]=offspringChrom;
                }
            }
            //for(int i=0;i<newOffSpring.size();i++)
                //cout<<"ss: "<<newOffSpring[i]<<endl;
            ///get highest fit in one gen
            set<int> bestFit;
            for(int i=0; i<newOffSpring.size(); i++)
            {
                //cout<<"Newofspring   "<<calculteValue(newOffSpring[i],number,knapSack)<<endl;
                bestFit.insert(calculteValue(newOffSpring[i],number,knapSack));
            }
            set <int>::iterator it;
            //it++;
            int s=*bestFit.rbegin();
            HighestFitInGen.insert(s);


           /* for(it=bestFit.begin();it!=bestFit.end();it++)
                cout<<*it<<endl;*/

            generation.clear();
            ///newgenfitness
            for(int i=0; i<newOffSpring.size(); i++)
            {
                generation.push_back(pair<string,int>(newOffSpring[i],calculteFitness(newOffSpring[i],number,knapSack)));
            }
            bestFit.clear();
            offspring.clear();
            newOffSpring.clear();

///endWhile
        }
            knapSack.clear();
            terminate=number*1.5;
            int s=*HighestFitInGen.rbegin();
            HighestFitInGen.clear();
            cases.push_back(s);




    }
    for(int i=0;i<cases.size();i++)
        cout<<"Case: "<<i+1<<" "<<cases[i]<<endl;

    return 0;
}
