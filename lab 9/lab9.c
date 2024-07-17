# include <stdlib.h>
#include <stdio.h>

typedef struct Con{
    double weight;
    struct Node *node;
} Con;


typedef struct Node {
    int data;
    int num_neighbours;
    struct Con **connections;
    double dist_from_start;
    int visited;
} Node;

void create_node(int data, Node **p_node) {
    *p_node = malloc(sizeof(Node));
    (*p_node)->data = data;
    (*p_node)->connections = (void *)0;
    (*p_node)->num_neighbours = 0;
    (*p_node)->visited = 0;
    (*p_node)->dist_from_start = 10000000.0;
}

void add_neighbour(Node *node, Node *neighbour, double weight) {
    node->num_neighbours++;
    node->connections = realloc(node->connections, (node->num_neighbours) * sizeof(Con *));
    node->connections[node->num_neighbours - 1] = malloc(sizeof(Con));
    node->connections[node->num_neighbours - 1]->weight = weight;
    node->connections[node->num_neighbours - 1]->node = neighbour;
}



void add_to_visited(Node ***p_visited, int *p_n_visited, Node *node) {
    Node **visited = *p_visited;
    for(int i = 0; i < *p_n_visited; i++) {
        if (visited[i] == node) {
            return;
        }
    }
    (*p_n_visited)++;
    *p_visited = realloc(visited, (*p_n_visited) * sizeof(Node*));
    (*p_visited)[*p_n_visited - 1] = node;
}

int was_visited(Node **visited, int n_visited, Node *node) {
    for(int i = 0; i < n_visited; i++) {
        if (visited[i] == node) {
            return 1;
        }
    }
    return 0;
}


void dijkstra(Node *start, Node *end) {
    Node **visited = (void *)0; // array of nodes 
    int num_visited = 0;
    add_to_visited(&visited, &num_visited, start); // add start to visited 
    start->dist_from_start = 0.0;
    Node *current = start;

    while (current != end) {
        double min_dist = 1000000000.0;
        Node *min_node = (void *)0;
        for(int i = 0; i < num_visited; i++){
            Node *node = visited[i]; // look at all visited nodes, one at a time 
            for (int j = 0; j < node->num_neighbours; j++) { 
                Con *con = node->connections[j]; // look at each connection at the visited nodes 
                double dist = node->dist_from_start + con->weight;
                if (!was_visited(visited, num_visited, con->node) && dist < min_dist) { 
                    min_dist = dist;
                    min_node = con->node;
                }
            }
        }
        current = min_node;
        current->dist_from_start = min_dist;
        add_to_visited(&visited, &num_visited, current);
    }
}

int main(){
    Node *a;
    Node *b;
    Node *c;
    Node *d;
    Node *e;

    create_node(1, &a);
    create_node(2, &b);
    create_node(3, &c);
    create_node(4, &d);
    create_node(5, &e);

    add_neighbour(a, b, 5);
    add_neighbour(a, c, 2);
    add_neighbour(b, d, 4);
    add_neighbour(c, b, 1);
    add_neighbour(c, d, 1);
    add_neighbour(d, e, 1);

    dijkstra(a, e);

    printf("Shortest path size: %f\n", e->dist_from_start);

    free(a->connections);
    free(b->connections);
    free(c->connections);
    free(d->connections);
    free(e->connections);
    free(a);
    free(b);
    free(c);
    free(d);
    free(e);

    return 0;
}