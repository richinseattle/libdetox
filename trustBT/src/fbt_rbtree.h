#ifndef FBT_RBTREE_H
#define FBT_RBTREE_H

#include <inttypes.h>

typedef void* rb_key;
enum rb_color {
    COLOR_RED,
    COLOR_BLACK
};

// struct rb_node {
//     rb_key addr_begin;
//     rb_key addr_end;
//     struct rb_node *parent;
//     struct rb_node *left;
//     struct rb_node *right;
//     enum rb_color color;
//     uint8_t flags;              /* 3 bits rwx flags */
//     uint16_t lib_index;         /* index in the table of linked libraries */
//     const char* section_name;         /* pointer to the section name string */
// };

struct rb_node {
    rb_key addr_begin;
    rb_key addr_end;
    struct rb_node *parent;
    struct rb_node *left;
    struct rb_node *right;
    enum rb_color color;
};


struct rb_node *rb_query(struct rb_node *root, rb_key addr);
struct rb_node *rb_insert(struct rb_node *root,
                          struct rb_node *node)
                          __attribute__ ((warn_unused_result));
void rb_clear(struct rb_node *root);

#endif /* FBT_RBTREE_H */