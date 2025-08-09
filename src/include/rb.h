#pragma once

#include <stdint.h>
#include <stddef.h>

enum rb_color { BLACK, RED };

struct rb_node {
    struct rb_node* parent;

    struct rb_node* left;
    struct rb_node* right;
    enum rb_color color;
};
typedef struct rb_node rb_node_t;

struct rb_tree {
    rb_node_t* root;
};
typedef struct rb_tree rb_tree_t;

rb_tree_t rb_init() {
    rb_tree_t tree;
    tree.root = NULL;

    return tree;
}

void rb_insert(rb_node_t* parent, rb_node_t* node, rb_node_t** link) {
    node->color = RED;

    node->parent = parent;
    node->left = NULL;
    node->right = NULL;
    *link = node;
}

rb_node_t* rb_next(rb_node_t* node) {
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
        return node;
    }

    while (node->parent != NULL && node == node->parent->right) {
        node = node->parent;
    }
    return node->parent;
}

rb_node_t* rb_prev(rb_node_t* node) {
    if (node->left != NULL) {
        node = node->left;
        while (node->right != NULL) {
            node = node->right;
        }
        return node;
    }

    while (node->parent != NULL && node == node->parent->left) {
        node = node->parent;
    }
    return node->parent;
}

static void rb_left_rotate(rb_tree_t* tree, rb_node_t* x) {
    rb_node_t* y = x->right;
    x->right = y->left;
    if (y->left != NULL) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

static void rb_right_rotate(rb_tree_t* tree, rb_node_t* x) {
    rb_node_t* y = x->left;
    x->left = y->right;
    if (y->right != NULL) {
        y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == NULL) {
        tree->root = y;
    }
    else if (x == x->parent->right) {
        x->parent->right = y;
    }
    else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

static void rb_transplant(rb_tree_t* tree, rb_node_t* u, rb_node_t* v) {
    if (u->parent == NULL) {
        tree->root = v;
    }
    else if (u == u->parent->left) {
        u->parent->left = v;
    }
    else {
        u->parent->right = v;
    }
    if (v != NULL) {
        v->parent = u->parent;
    }
}

static rb_node_t* rb_minimum(rb_node_t* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

static void rb_remove_fixup(rb_tree_t* tree, rb_node_t* x) {
    while (x != tree->root && (x == NULL || x->color == BLACK)) {
        if (x == x->parent->left) {
            rb_node_t* w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rb_left_rotate(tree, x->parent);
                w = x->parent->right;
            }
            if ((w->left == NULL || w->left->color == BLACK) &&
                (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL) w->left->color = BLACK;
                    w->color = RED;
                    rb_right_rotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL) w->right->color = BLACK;
                rb_left_rotate(tree, x->parent);
                x = tree->root;
            }
        }
        else {
            rb_node_t* w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rb_right_rotate(tree, x->parent);
                w = x->parent->left;
            }
            if ((w->right == NULL || w->right->color == BLACK) &&
                (w->left == NULL || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            }
            else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL) w->right->color = BLACK;
                    w->color = RED;
                    rb_left_rotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != NULL) w->left->color = BLACK;
                rb_right_rotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    if (x != NULL)
        x->color = BLACK;
}

void rb_remove(rb_tree_t* tree, rb_node_t* node) {
    rb_node_t* y = node;
    rb_node_t* x;
    enum rb_color y_original_color = y->color;

    if (node->left == NULL) {
        x = node->right;
        rb_transplant(tree, node, node->right);
    }
    else if (node->right == NULL) {
        x = node->left;
        rb_transplant(tree, node, node->left);
    }
    else {
        y = rb_minimum(node->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == node) {
            if (x != NULL) x->parent = y;
        }
        else {
            rb_transplant(tree, y, y->right);
            y->right = node->right;
            y->right->parent = y;
        }
        rb_transplant(tree, node, y);
        y->left = node->left;
        y->left->parent = y;
        y->color = node->color;
    }

    if (y_original_color == BLACK) {
        rb_remove_fixup(tree, x);
    }
}

void rb_balance(rb_tree_t* tree, rb_node_t* node) {
    rb_node_t* parent = node->parent;

    if (parent->color == BLACK) return;

    while (node != tree->root && node->parent->color == RED) {
        if (node->parent == node->parent->parent->left) {
            rb_node_t* u = node->parent->parent->right;
            if (u->color == RED) {
                node->parent->color = BLACK;
                u->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->right) {
                    node = node->parent;
                    rb_left_rotate(tree, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rb_right_rotate(tree, node->parent->parent);
            }
        }
        else {
            rb_node_t* u = node->parent->parent->left;
            if (u->color == RED) {
                node->parent->color = BLACK;
                u->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else {
                if (node == node->parent->left) {
                    node = node->parent;
                    rb_right_rotate(tree, node);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                rb_left_rotate(tree, node->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}