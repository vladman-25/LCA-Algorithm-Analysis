#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;


int ran(int l, int r)
{
    return l + (rand() % (r - l + 1));
}

 
int main()
{
    srand(time(0));
    const int H = 100;
    const int K = 1000;
    const int TK = 10000;
    const int HK = 100000;
    const int KK = 1000000;

    int nrofnodes[20] = {H,K,K,K,K,TK,TK,TK,TK,TK,HK,HK,HK,HK,HK,H,H,H,5*TK,5*TK};
    int nrofqueries[20] = {H,HK,TK,K,H,KK,HK,TK,K,H,KK,HK,TK,K,H,K,TK,HK,5*TK,50};

    ofstream myfile;

    for(int iterator = 1; iterator <= 20; iterator++) {
        char buffer[50];
        sprintf(buffer,"in/test%d.in",iterator);
        myfile.open (buffer);
        myfile << nrofnodes[iterator-1] << " " << nrofqueries[iterator-1] <<endl;
        
        /// GENERATE N-1 EDGES /////////
//1
        int n = nrofnodes[iterator-1];
        int length = n - 2;
        int arr[length];
        for (int i = 0; i < length; i++)
        {
            arr[i] = ran(0, length + 1) + 1;
        }
//2
        int vertices = length + 2;
        int vertex_set[vertices];
    
        for (int i = 0; i < vertices; i++)
            vertex_set[i] = 0;
    
        for (int i = 0; i < vertices - 2; i++)
            vertex_set[arr[i] - 1] += 1;
    
        int j = 0;
    

        for (int i = 0; i < vertices - 2; i++)
        {
            for (j = 0; j < vertices; j++)
            {
    

                if (vertex_set[j] == 0)
                {
    

                    vertex_set[j] = -1;
                    myfile<<(j + 1) << " " << arr[i] <<endl;
    
                    vertex_set[arr[i] - 1]--;
    
                    break;
                }
            }
        }
    
        j = 0;
    

        for (int i = 0; i < vertices; i++)
        {
            if (vertex_set[i] == 0 && j == 0)
            {
    
                myfile <<(i + 1) << " ";
                j++;
            }
            else if (vertex_set[i] == 0 && j == 1)
                myfile << (i + 1);
        }

        for(int i = 0; i < nrofqueries[iterator-1]; i++) {
            int q1 = ran(0, nrofnodes[iterator-1] - 1) + 1;
            int q2 = ran(0, nrofnodes[iterator-1] - 1) + 1;
            myfile<< endl << q1 << " " << q2;
        }


        myfile.close();
    }



 
    return 0;
}