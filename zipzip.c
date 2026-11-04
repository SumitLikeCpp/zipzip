#include <zipzip.h>

// sort(list,func)
List *zipsort(List *list,int32 size, sortfun *func){
    int32 len,n;
    List *new;
    sortres res;
    bool changed;

    new = listcopy(list);
    len = list->length;

    for(n = 0;n<(len - 2);n++){
        res = func(new->data[n],new->data[(n+1)]);
        if(res == LessThan)
            swap(new->data[n],new->data[(n+1)]);
    }

    changed = (listeq(list,new)) ? 
            false;
        true;

}

void amtswap(amtentry *e1,amtentry *e2){
    amtentry tmp;
    if(!e1 || !e2) return;

    tmp = *e1;
    *e1 = *e2;
    *e2 = tmp;

}

bool increase(amtlist **list){
    amtlist *p;
    int64 size;
    int32 len;

    if(!*list || ((*list)->capacity >= Maxcap))
        return false;
    
    len = (*list)->capacity + Blocksize;
    size = (sizeof(struct s_amtlist)+(sizeof(amtentry)*len));
    p = (amtlist *)ralloc(*list, size);
    if(!p)
        return false;
    else{
        p->capacity = len;
        *list = p;
    }
        
    
    return true;

}

bool amtadd(amtlist **list,amtentry e){
    int32 idx;

    if(!list) return false;

    if((*list)->length == (*list)->capacity){
        if((*list)->capacity >= Maxcap)
            return false;
        else if(!increase(list))
            return false;
    }

    idx = (*list)->length;
    (*list)->data[idx] = e;
    (*list)->length++;

    return true;

}

void showamtlist(int8* identifier,amtlist *list){
    int32 n;

    if(!identifier || !list)
        return;
    
    printf("(amtlist *)%s = {\n  ", $c identifier);
    printf("capacity = %d\n  ", $i list->capacity);
    printf("length  = %d\n }", $i list->length);
    printf("\n");

    if(list->length){
        for(n=1; (n<= list->length) && (n<6);n++){
            show(list->data[n]);
        }
        if(list->length > 5)
            printf("   [..]\n");
    }

    return;
}

void showamtentry(int8 *identifier, amtentry e){
    if(!identifier) return;

    printf("    %s={0x%08x,%u}\n",
        $c identifier, $i e.block, $i e.amt);

    return;
}

amtlist *mkamtlist(){
    int32 size;
    amtlist *p;

    size = (sizeof(struct s_amtlist)+(sizeof(amtentry)*Blocksize));
    p = (amtlist *)alloc(size);
    if(!p)
        return (amtlist *)0;
    zero($1 p,size);

    p->length = 0;
    p->capacity = Blocksize;

    return p;
}

void zero(int8 *dst,int32 size){
    int8 *p;
    int32 n;

    for(p=dst,n=size;n;p++,n--)
        *p = 0;
    
    return;
}

int main(){
    amtlist *list;
    int32 n;

    list = mkamtlist();
    if(!list)
        return 1;
    show(list);

    for(n = 0;n<0x11000;n++)
        amtadd(&list,(amtentry){0x1000000+n, n});
    swap(&list->data[1],&list->data[3]);
    
    show(list);
    destroy(list);
    
    return 0;
}