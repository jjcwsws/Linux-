#include <iostream>
#include <queue>

int main()
{
    std::queue<int> q;
    q.push(10);
    q.push(10);
    q.push(10);
    q.push(10);
    q.push(10);

    while(!q.empty())
    {
        std::cout << q.front() << std::endl;
        q.pop();
    }
}