// Copyright @ 2020

#ifndef SRC_NODE_HPP_
#define SRC_NODE_HPP_

struct Node{
    int x;
    int y;

    int value;
    bool visited;
    bool is_mine;
    bool is_flag;

    int num_neighbours;
    Node **neighbours;
};

#endif  // SRC_NODE_HPP_
