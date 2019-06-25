

#include "tree.h"

NTree* root = NULL;


int nid = 1;
int MAX = 0; /* maximum length of the of an array for setting ids, equals to maximum number of nodes in the tree, so it is ensured to have enough space */



Data* createData(Expression* left, Expression* right, char* ruletag) {
    Data* data = malloc(sizeof(Data));
    data->left = left;
    data->right = right;
    data->ruletag = ruletag;
    data->counter = 0;

    nullExpressions(); // nulling Expressions for further use 
    
    return data;
}

void createRoot(Data* data) {
    root = malloc(sizeof(NTree));
    root->data = data;
    root->child = NULL;
    root->brother = NULL;
    root->id = 0;
    
}

void createNode(NTree* parent, Data* data) {
    MAX++;
    if (root == NULL) {
        createRoot(data);
        return;
    }

    NTree* newNode = malloc(sizeof(NTree));
    if (parent->child == NULL) {
        parent->child = newNode;
    }
    else {
        NTree* ptr = parent->child;
        while (ptr->brother != NULL) {
            ptr = ptr->brother;
        }
        ptr->brother = newNode;
    }
    newNode->data = data;
    newNode->id = 0;
    newNode->child = NULL;
    newNode->brother = NULL;

}

Expression* getLeft(NTree* node) {
    return node->data->left;
}

Expression* getRight(NTree* node) {
    return node->data->right;
}

void ids() {
    NTree** nodes = malloc(MAX*(sizeof(struct NTree*)));
    nodes[0] = root->child;
    NTree* bro = root->child->brother;
    int i = 1;
    while (bro) {
        nodes[i] = bro;
        i++;
        bro = bro->brother;
    }
    setIds(nodes, i);
}

void setIds(NTree* nodes[], int counter) {
    if (counter) {
        NTree** nlevel = malloc(MAX*sizeof(struct NTree*));
        int n = 0;
        int i=0;
        for (; i<counter; i++) {
            nodes[i]->id = nid;
            

            nid++;
            if (nodes[i]->child) {
                nlevel[n] = nodes[i]->child;
                n++;
                NTree* bro = nodes[i]->child->brother;
                while (bro) {
                    nlevel[n] = bro;
                    n++;
                    bro = bro->brother;
                }
            }
        }
        setIds(nlevel, n);
    }
}


void printNodes(NTree* node) {
    bool solved = false;
    printf("node[%d]===> ", node->id);
    if (node != root) {
        printf("%s ", node->data->ruletag);
    }
    else {
        printf("(root) ");
    }
    if (node->data->left || node->data->right) {
        printf("--- ");
        printExpressions(getLeft(node), getRight(node));
    }
    if (node->data->counter) {
        int shift;
        if (!strcmp(node->data->mappings[node->data->counter-1], "solved")) {
            solved = !solved;
            if (node->data->mappings[1]) {
                printf("--- SOLVED --- [ ");
            }
            else {
                printf("--- SOLVED");
            }
            shift = 1;
        }
        else if (!strcmp(node->data->mappings[node->data->counter-1], "fail")) {
            solved = !solved;
            printf("--- FAIL");
            shift = 1;
        }
        else {
            printf(" --- [ ");
            shift = 0;
        }
        int i=0;
        for (; i<node->data->counter - shift; i++) {
            printf("%s", node->data->mappings[i]);
            if (i+1 != node->data->counter - shift) {
                printf(" / ");
            }
            else {
                printf(" ]");
            }
        }
    }
    printf("\n");
    if (node->child) {
        printNodes(node->child);
    }
    if (node->brother) {
        printNodes(node->brother);
    }
    if (!node->child && !node->brother) {
        if (solved) {
            printf("\n");
        }
        else {
            printf("!!!!!!HAVE NOT BEEN SOLVED!!!!!!\n");
        }
    }
}
















