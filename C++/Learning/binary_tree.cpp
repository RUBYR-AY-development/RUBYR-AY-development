/*
* simple binary tree i made (for practice)
* has infinite children and simple search func and delete
*/

#include <iostream>
#include <vector>

template <typename T> struct node
{
    T data;
    std::vector<node<T>*> children;

    void add_child(T data)
    {
        node<T>* child = new node<T>(data);
        this->children.push_back(child);
    }

    node(T data) { this->data = data; }
    ~node()
    {
        for (node<T>* x : children)
            delete x;
    }

    bool search(T request) // searches itself and children for it
    {
        if (this->data == request)
            return true;
        else
            for (node<T>* x : children)
                return x->search(request);
    }
};

int main()
{
    node<int>* root = new node<int>(1);
    root->add_child(2); // root->child1
    root->children[0]->add_child(3); // root->child1->child1

    std::cout << root->data << '\n' << root->children[0]->data << '\n';

    bool success = root->search(4);
    if (success)
        std::cout << "hooray";
    else
        std::cout << "saddening";

    delete root;
    return 0;
}
