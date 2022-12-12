// dynamic array
typedef struct dynarr_t {
    void** content;
    size_t nmemb;
    size_t typesize;
    size_t capacity;
} dynarr_t;


dynarr_t* dynarr_init(unsigned int typesize)
{
    dynarr_t* dnar;
   
    dnar = malloc(sizeof(dynarr_t));
    memset(dnar, '\0', typesize);

    dnar->typesize = typesize;
    
    return dnar;
}


dynarr_t* dynarr_append(dynarr_t* dnar, void* entry)
{
    if (dnar->nmemb == dnar->capacity)
    {
        dnar->capacity = dnar->capacity ? dnar->capacity * 2 : 8;
        dnar->content = realloc(dnar->content, dnar->capacity * dnar->typesize);
        if (!dnar->content)
        {
            puts("ERR: realloc failed");
            exit(1);
        }
    }

    dnar->content[dnar->nmemb++] = entry;
    return dnar;
}


#define DYNARR_INDEX(dnar, index) (((dynarr_t*)dnar)->content[index])
#define DYNARR_LEN(dnar) (((dynarr_t*)dnar)->nmemb)
