#include <queue>

#include "iostream"
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace std;

int x,y;
int cases[1010][1010];
struct Node
{
  string str;
  int cnt;
  bool operator<(const Node&other)const
  {
    return cnt<other.cnt;
  }
};
priority_queue<Node> vis;
int main(){
  cin>>x>>y;
  for (int i=0;i<x;i++)
    for (int j=0;j<y;j++)
      cin>>cases[i][j];

  for (int i=0;i<x;i++)
  {
    unordered_set<string> tempSet;
    for (auto visStr:vis)
    {
      tempSet.insert(visStr);
      for (int k=0;k<y;k++)
      {
        if (cases[i][k]) visStr[k]='1';
      }
      tempSet.insert(visStr);
    }
  }

}