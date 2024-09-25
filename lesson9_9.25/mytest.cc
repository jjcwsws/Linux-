#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    for(int i = 0; environ[i]; i++)
    {
        printf("env[%d]: %s\n", i, environ[i]);
    }
  //  for(int i = 0; argv[i]; i++)
  //  {
  //      printf("argv[%d]: %s\n", i, argv[i]);
  //  }

  //  cout << "hello C++" << endl;
  //  cout << "hello C++" << endl;
  //  cout << "hello C++" << endl;
  //  cout << "hello C++" << endl;
  //  cout << "hello C++" << endl;
  //  cout << "hello C++" << endl;
  //  cout << "hello C++" << endl;
  //  cout << "hello C++" << endl;
    cout << "hello C++" << endl;
    return 0;
}
