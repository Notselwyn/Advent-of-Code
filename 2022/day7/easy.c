// starting with $ means command
// cd XYZ means **enter recursion**
// cd .. means **return recursion**
// cd / means **reset recursion**
// ls means **add**
//
//
// INPUT: command history
// ALGO: recursively retrieve size of all directories
// OUTPUT: sum of directories with total size of at most 100.000

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


typedef struct fs_entry_ {
    const char* name;  // directories should end in / e.g. dir/
    unsigned long size;
    unsigned char type;  // file: 1, dir: 2
    struct fs_entry_* parent;
    struct fs_entry_** children;
    unsigned int children_capacity;
    unsigned int children_len;
} fs_entry;

fs_entry* fs_entry_init(const char* name, unsigned long size, unsigned char type)
{
    if (name == NULL || type == 0)
    {
        puts("ERR fs_entry_init(): invalid arguments");
        exit(1);
    }

    fs_entry* fentry = calloc(sizeof(fs_entry), 1);
    if (!fentry)
    {
        puts("ERR fs_entry_init(): malloc failed");
        exit(1);
    }

    fentry->name = name;
    fentry->size = size;
    fentry->type = type;

    return fentry;
}


fs_entry* fs_entry_child_append(fs_entry* parent, fs_entry* child)
{
    if (parent == NULL || child == NULL)
    {
        puts("ERR fs_entry_child_append(): arguments are invalid");
        exit(1);
    }

    if (parent->type != 2)
    {
        printf("ERR fs_entry_child_append(): parent->type: %d\n", parent->type);
        exit(1);
    }
    
    // add parent property for increasing size
    child->parent = parent;
    for (fs_entry* curr = child->parent; curr != NULL; curr = curr->parent)
        curr->size += child->size;

    if (parent->children_len == parent->children_capacity)
    {
        parent->children_capacity = parent->children_capacity ? \
                                    parent->children_capacity * 2 : 8;

        parent->children = realloc(parent->children, parent->children_capacity*sizeof(fs_entry*));
    }

    parent->children[parent->children_len++] = child;
    return parent;
}


fs_entry* fs_entry_child_find_name(fs_entry* parent, const char* name)
{
    for (int i=0; i < parent->children_len; i++)
        if (!strcmp(parent->children[i]->name, name))
            return parent->children[i];

    printf("ERR: failed to find child of %p with name '%s'\n", parent, name);
    exit(1);
}


int sum_get(fs_entry* fs_root)
{
    int sum = 0;
    for (int i = 0; i < fs_root->children_len; i++)
    {
        fs_entry* child = fs_root->children[i];
        if (child->type == 1)  // we aren't interested in files
        {
            printf("detected file (name: %s)\n", child->name);
            continue;
        }
        
        printf("detected directory (name: %s)\n", child->name);
        if (child->size <= 100000)
            sum += child->size;

        sum += sum_get(child);
    }

    return sum;
}


int main()
{
    FILE* file = fopen("input.txt", "r");
    if (!file)
    {
        puts("ERR: fopen()");
        exit(1);
    }

    char line[128];


    // PARSING STATES:
    // - 0: idle
    // - 1: directory listing
    int state = 0;
    fs_entry* fs_root = fs_entry_init("/", 0, 2);
    fs_entry* fs_cwd = fs_root;
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
        if (line[0] == '$') // command
        {
            state = 0;
            if (!strncmp(line, "$ ls", 4)) // list directory content
                state = 1;
            else if (!strncmp(line, "$ cd ", 5)) {
                char dirname[128];
                sscanf(line, "$ cd %s[^\n]", dirname);

                if (!strcmp(dirname, "/"))
                    fs_cwd = fs_root;
                else if (!strcmp(dirname, ".."))
                    fs_cwd = fs_cwd->parent;
                else 
                    fs_cwd = fs_entry_child_find_name(fs_cwd, dirname);
            }
        } else if (state == 1) { // is parsing directory content
            char filename[128];
            unsigned long size = 0;
            unsigned char type = 0;

            if (!strncmp(line, "dir ", 4))
            {
                type = 2;
                sscanf(line, "dir %s[^\n]", filename);
            } else { 
                type = 1;
                sscanf(line, "%lu %s[^\n]", &size, filename);
            } 

            if (strlen(filename) < 1)
            {
                printf("ERR: invalid size or filename (filename: '%s')\n", filename);
                exit(1);
            }
            
            // use heap because we need to use it outside of main()
            char* filename_heap = malloc(strlen(filename)+5);
            strcpy(filename_heap, filename);

            fs_entry* fs_child = fs_entry_init(filename_heap, size, type);
            fs_entry_child_append(fs_cwd, fs_child);
        }
    }

    printf("root size: %lu\n", fs_root->size);
    
    
    // STATES
    // - 0: keep nesting
    // - 1: return to parent
    
    int sum = sum_get(fs_root);
    printf("sum of directories whose size is smaller than 100.000: %d\n", sum);
}

