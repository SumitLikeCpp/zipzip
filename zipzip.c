#include <zipzip.h>

int8 amtsort(entry e1_,entry e2_){
    amtentry *e1,*e2;

    e1 = (amtentry *)&e1_;
    e2 = (amtentry *)&e2_;

    if(e1->amt > e2->amt)
        return MoreThan;
    else if(e1->amt == e2->amt)
        return Equal;
    return 
        LessThan;
}

amtlist *amtlistcopy(amtlist *list){
    amtlist *new;
    int32 n;

    new = mkamtlist();
    if(!new){
        return (amtlist *)0;
    }
    if(!list->length)
        return new;
    
    for(n=0;n<list->length;n++)
        amtadd(&new, list->data[n]);
    
    return new;
}

bool amtlisteq(amtlist *l1,amtlist *l2){
    bool res;
    int32 n;

    if(!l1 || !l2)
        return false;
    
    if(l1->length != l2->length)
        return false;

    res = true;
    for(n=0;n<l1->length;n++)
        if((l1->data[n].amt == l2->data[n].amt)
            && (l1->data[n].block == l2->data[n].block));
        else{
            res = false;
            break;
        }

    return res;
}

// sort(list,func)
List *zipsort(List *list_,sortfunc func){
    int32 len,n;
    amtlist *new,*list;
    int8 res;
    bool changed;

    if(!func)
        return (List *)0;

    list = (amtlist *)list_;

    if(list->length < 2)
        return (List *)0;

    new = listcopy(list);
    if(!new){
        return (List *)0;
    }
    len = list->length;

    for(n = 0;n<(len - 1);n++){
        res = func((entry)new->data[n],(entry)new->data[(n+1)]);
        if(res == MoreThan)
            swap(&new->data[n],&new->data[(n+1)]);
    }

    changed = (listeq(list,new)) ? 
            false:
        true;

    if(changed){
        free(list);
        return (List *)new;
    }
    else{
        free(new);
        return (List *)0;
    }

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
        for(n=0; (n<list->length) && (n<5);n++){
            show(list->data[n]);
        }
        if(list->length > 4)
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
    List *new;
    amtlist *new_;
    int32 n;

    list = mkamtlist();
    if(!list)
        return 1;
    show(list);

    for(n = 0;n<0x11000;n++)
        amtadd(&list,(amtentry){0x1000000+n, n});
    swap(&list->data[1],&list->data[3]);
    show(list);

    new = zipsort((List *)list,&amtsort);
    new_ = (amtlist *)new;
    show(new_);

    new = zipsort((List *)new_,&amtsort);
    new_ = (amtlist *)new;
    show(new_);

    new = zipsort((List *)new_,&amtsort);
    if(!new){
        printf("(END)\n");
        return 0;
    }
    new_ = (amtlist *)new;
    show(new_);

    destroy(new_);
    
    return 0;
}