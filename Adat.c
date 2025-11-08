#include <stdio.h>
#include <stdlib.h>
#include "debugmalloc.h"
#include "Adat.h"

static int maxid = 0;

#pragma pack(1)
typedef struct boardelement {
    char allas[8][8];
    int id;
    int previd;

} BoardElement;

static void update_maxid(int id){
    if(id > maxid){
        maxid = id;
    }
}

static Board* search_board(int id, Board* curr);

Board* create_board(Board* previous){
    Board* new = (Board*)malloc(sizeof(Board));
    if(new != NULL){
        new ->prev = previous;
        new -> next = NULL;
        new -> numNext = 0;
        new -> allas[0][0] = 'A';
        if(previous != NULL){
        new -> previd = previous->id;
        }else new -> previd = -1;
        new -> id = maxid++;
    }
    return new;
}

static Board* create_board_from_element(BoardElement* element){
    Board* prev = NULL;
    Board* curr = NULL;
    if(element -> previd == -1){
        curr = create_board(NULL);
        head = curr;
    }else{//megkeressük hogy melyik board az ezt megelőző
        prev = search_board(element -> previd, head);
        if(prev == NULL) return NULL; //TODO ERROR KÓD
        curr = add_new_board(prev);
        if(curr == NULL) return NULL; //TODO ERROR
        memcpy(curr->allas, element->allas, sizeof(element->allas));
        curr -> id = element ->id;
        curr -> previd = element -> previd;
        update_maxid(curr ->id);
    }
    return curr;
}

Board* add_new_board(Board *curr){
    Board* nextboard = NULL;
    Board** new = realloc(curr -> next, (curr->numNext + 1)*(sizeof(Board)));

    if(new != NULL){
        curr->next = new;
        nextboard = create_board(curr);
        //
        if(nextboard != NULL){
            curr->next[curr->numNext] = nextboard;
            curr->numNext += 1; 
        }
    }
    return nextboard; //NULL, ha valami nem jo
}

void felszabaditas(Board* curr){
    maxid = 0;
    printf("%p %d %p\n", curr, curr->id, curr->next);
    if(curr->next != NULL){
        for(int i = 0; i <curr->numNext; i++){
            felszabaditas(curr->next[i]);
        }
        free(curr -> next);
    }
    free(curr);
    return;
}

static void saveboard(FILE* fp, Board* curr){
    BoardElement element;

    element.id = curr->id;

    memcpy(element.allas,curr->allas, sizeof(curr->allas));

    element.previd = curr->previd;

    fwrite(&element, sizeof(BoardElement), 1, fp);    
    
    if(curr->next != NULL){
        for(int i = 0; i <curr->numNext; i++){
            saveboard(fp, curr->next[i]);
        }
    }
    return;
}

void save_boards(char* filename){
    FILE* filepointer;
    filepointer = fopen(filename, "w");
    saveboard(filepointer, head);

    fclose(filepointer);
}

void load_boards(char* filename){
    BoardElement element;
    int size;
    FILE* filepointer;
    filepointer = fopen(filename, "r");
    //loadboard(filepointer, head);

    while(size = fread(&element, sizeof(element), 1, filepointer) > 0){
        printf("%d %d BOARD\n", element.id, element.previd);
        if(size != 1) break; //TODO HIBAKOD
        create_board_from_element(&element);
    }
    fclose(filepointer);
}

static Board* search_board(int id, Board* curr){
    Board* found = NULL;
    if(curr->id == id){
        return curr;
    }else{
        for(int i = 0; i < curr->numNext; i++){
            found = search_board(id, curr->next[i]);
            if(found) return found;
        }
    }
    return NULL;
}