#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include "quadtree.h"
#include "pbPlots.h"
#include "supportLib.h"


////////////////Inicio das funções de particulas///////////////

Particle *start_Part(int number, float mass, float radius, Vec2 pos, Vec2 Ini_Vel){
    Particle *Paux;
    Paux = (Particle *) malloc(sizeof(Particle));
    if (Paux == NULL){
        return NULL;
    }
    else{
        Paux->number = number;
        Paux->mass = mass;
        Paux->radius = radius;
        Paux->pos = pos;
        Paux->vel = Ini_Vel;
        return Paux;
    }
}



void destroy_Particle(Particle *P){
    free(&P->number);
    free(&P->mass);
    free(&P->radius);
    free(&P->pos.x);
    free(&P->pos.y);
    free(&P->pos);
    free(&P->vel.x);
    free(&P->vel.y);
    free(&P->vel);
    free(P);
}



/////////////////Fim das funções de particulas/////////////////

///////////inicio das funções de lista linear simples//////////

ListNode *start_ListNode(Particle *P){
    ListNode *Naux;
    Naux = (ListNode *) malloc(sizeof(ListNode));
    if (Naux == NULL){
        return NULL;
    }
    else{
        Naux->p = P;
        Naux->next = NULL;
        return Naux;
    }

}


List *start_List(){

    List *Laux;
    Laux = (List *) malloc(sizeof(List));
    if (Laux == NULL){
        return NULL;
    }
    else{
        Laux->root = NULL;
        Laux->size = 0;
        return Laux;
    }
}



int insert_List(Particle *P, List *L){

    ListNode *Nsent;

    ListNode *Naux;
    Naux = start_ListNode(P);

    if (Naux == NULL){
        return 0;
    }

    else{

        if(L->size == 0){
                L->root = Naux;
        }
        else{
            Nsent = L->root;
            for (int a = 0; a < L->size - 1; a++){
                Nsent = Nsent->next;
            }
            Nsent->next = Naux;
        }

        L->size++;
        return 1;        

    }
}



ListNode *search_ListNode(Particle P,List *L){
    ListNode *Nsent;
    Nsent = L->root;

    for(int i = 0; i < L->size; i++){
        if(Nsent->p->number == P.number){
            break;
        }
        else{
            Nsent = Nsent->next;
        }
    }

    return Nsent;
}



void destroy_ListNode(ListNode *N){

    if(N != NULL){

        if(N->next != NULL){
            N->next = NULL;
        }

        N->p = NULL;

        free(N);
    }

}



int pop_List(Particle P, List *L){


    ListNode *Nsent;
    ListNode *Nsent2;
    Nsent = L->root;

    Nsent2 = search_ListNode(P, L);

    if(Nsent2 == NULL){
        return 0;
    }

    else{

        if(Nsent->p->number == P.number){
            L->root = Nsent2->next;
        }

        else{

            for(int i = 0; i < L->size; i++){
                if( Nsent->p->number == P.number){
                    break;
                }
                else{
                    continue;
                }
            }


            if(Nsent2->next != NULL){
                Nsent->next = Nsent2->next;        
            }
                


        }

        destroy_ListNode(Nsent2);
        L->size--;
        return 1;
    }
}

List *copy_List(List *L){
    List *Laux;
    Laux = start_List();
    if (Laux == NULL){
        return NULL;
    }
    else{
        ListNode *Nsent;
        Nsent = L->root;

        for(int i = 0; i < L->size; i++){
            insert_List(Nsent->p, Laux);
            Nsent = Nsent->next;
        }

        return Laux;
    }
}



void destroy_List(List *L){

    ListNode *Nsent;
    Nsent = L->root;

    while(L->size > 1){

        Nsent = L->root;

        for(int i = 0; i < L->size; i++){
            Nsent = Nsent->next;
        }
        
        destroy_ListNode(Nsent);
        L->size--;
    }

    destroy_ListNode(L->root);
    L->root = NULL;
    
    free(L->root);
    free(&L->size);
}



////////////Fim das funções de lista linear simples////////////

/////////////////Inicio das funções da quadtree////////////////



QuadNode *start_QuadNode(Vec2 Origin, Vec2 wid_hei){


    QuadNode *Naux;
    Naux = (QuadNode *) malloc(sizeof(QuadNode));
    if (Naux  == NULL){
        return(Naux);
    }
    else{

        Naux->Origin = Origin;
        Naux->width_height = wid_hei;
        Naux->dividido = 0;
        Naux->part_list = *start_List();
        

        Naux->downleft = NULL;
        Naux->downright = NULL;
        Naux->upleft = NULL;
        Naux->upright = NULL;


        return(Naux);

    }
}



Quad *start_Quad(QuadNode *N, List *L){

    Quad *Q;
    Q = (Quad *) malloc(sizeof(Quad));
    if (Q == NULL){
        return(NULL);
    }
    else{
        
        Q->size = 0;
        Q->Raiz = N;
        Q->List_Part = L;

        return(Q);
    }

}



void check_Pos(Particle *P, QuadNode *Q){

    if(P->pos.x <= Q->width_height.x 
       && P->pos.y <= Q->width_height.y 
       && P->pos.x >= Q->Origin.x 
       && P->pos.y >= Q->Origin.y 
       && search_ListNode(*P, &Q->part_list) == NULL){

        insert_List(P, &Q->part_list);
    }

    else if( search_ListNode(*P, &Q->part_list) != NULL 
            && P->pos.x > Q->width_height.x 
            || P->pos.y > Q->width_height.y 
            || P->pos.x < Q->Origin.x 
            || P->pos.y < Q->Origin.y ){

        pop_List(*P, &Q->part_list);
    }
    
}




void quadNode_Destroy(QuadNode *Q){

    if(Q->dividido){
        quadNode_Destroy(Q->downleft);
        quadNode_Destroy(Q->downright);
        quadNode_Destroy(Q->upleft);
        quadNode_Destroy(Q->upright);

        Q->dividido = 0;
    }
    else{

        Q->downleft = NULL;
        Q->downright = NULL;
        Q->upleft = NULL;
        Q->upright = NULL;
        destroy_List(&Q->part_list);


        free(Q->downleft);
        free(Q->downright);
        free(Q->upleft);
        free(Q->upright);

        free(&Q->width_height.x);
        free(&Q->width_height.y);
        free(&Q->width_height);
        free(&Q->Origin);

        free(&Q->dividido);

    }
}



void quad_Check(QuadNode *Q){


    if (Q->part_list.size > max_points){
        if(!Q->dividido){

            Vec2 downleft_o, downright_o, upleft_o, upright_o;
            Vec2 downleft, downright, upleft, upright;


            downleft_o.x = Q->Origin.x;
            downleft_o.y = Q->Origin.y;
            downleft.x = (Q->width_height.x - downleft_o.x)/2 + downleft_o.x;
            downleft.y = (Q->width_height.y - downleft_o.y)/2 + downleft_o.y;

            downright_o.x = (Q->width_height.x - Q->Origin.x)/2 + Q->Origin.x;
            downright_o.y = Q->Origin.y;
            downright.x = Q->width_height.x;
            downright.y = (Q->width_height.y - Q->Origin.y)/2 + downright_o.y;

            upleft_o.x = Q->Origin.x;
            upleft_o.y = (Q->width_height.y - Q->Origin.y)/2 + Q->Origin.y;
            upleft.x = (Q->width_height.x - Q->Origin.x)/2 + upleft_o.x;
            upleft.y = Q->width_height.y;

            upright_o.x = (Q->width_height.x - Q->Origin.x)/2 + downleft_o.x;
            upright_o.y = (Q->width_height.y - Q->Origin.y)/2 + downleft_o.y;
            upright.x = Q->width_height.x;
            upright.y = Q->width_height.y; 

            Q->downleft = start_QuadNode(downleft_o ,downleft);
            Q->downright = start_QuadNode(downright_o ,downright);
            Q->upleft = start_QuadNode(upleft_o ,upleft);
            Q->upright = start_QuadNode(upright_o ,upright);

            ListNode *Nsent;
            ListNode *Nsent2;
            Nsent = Q->part_list.root;
            int Num_Part_Orig = Q->part_list.size; 

            for(int i = 0; i < Num_Part_Orig; i++){

                check_Pos(Nsent->p, Q->downleft);
                check_Pos(Nsent->p, Q->downright);
                check_Pos(Nsent->p, Q->upleft);
                check_Pos(Nsent->p, Q->upright);

                if(Nsent->next != NULL){
                    Nsent2 = Nsent->next;
                }

                pop_List(*Nsent->p, &Q->part_list);

                Nsent = Nsent2;            
            }

            Q->dividido = 1;


        }

        quad_Check(Q->downleft);
        quad_Check(Q->downright);
        quad_Check(Q->upleft);
        quad_Check(Q->upright);

    }

    else{
        if(Q->dividido){
            quadNode_Destroy(Q->downleft);
            quadNode_Destroy(Q->downright);
            quadNode_Destroy(Q->upleft);
            quadNode_Destroy(Q->upright);
        }
    }  
}



void destroy_Quad(Quad *Q){

    ListNode *Nsent;
    Nsent = Q->List_Part->root;
    for(int i = 0; i < Q->List_Part->size; i++){
        destroy_Particle(Nsent->p);
        Nsent = Nsent->next;
    }

    quadNode_Destroy(Q->Raiz);
    destroy_List(Q->List_Part);
    
    free(Q->Raiz);
    free(Q->List_Part);
    free(&Q->size);

}



//////////////////Fim das funções da quadtree//////////////////

////////////Inicio das funções de plot da quadtree/////////////



