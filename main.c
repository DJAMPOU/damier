#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include <stdio.h>
#include <stdlib.h>
#define len_p 10
#define cc 50
SDL_Window *window=NULL;
SDL_Renderer *rendere;
SDL_Texture *texture;
SDL_Texture *blan;
SDL_Texture *noir_vid;
SDL_Texture *pion_n;
SDL_Texture *pion_b;
SDL_Texture *noir_pos;
SDL_Texture *pion_b_select;
SDL_Texture *pion_n_select;
SDL_Texture *roi_n_select;
SDL_Texture *roi_n;
SDL_Texture *roi_b_select;
SDL_Texture *roi_b;
SDL_Texture *fond_1;
SDL_Texture *menu_1;

SDL_Rect rect[len_p][len_p];
int clik[2];
int nbr_pion_b =0;
int joueur = 2;
int advers = 1;
int joue =0;
int dpl;
int pion_jouable[80][6];// garde les pions jouables de la machine a instant
int nbr_pion_j;//garde le nombre de pion jouable
int jou[2];
int cot_event[2];
int plateau[len_p][len_p];
int level_simul = 1;
//int pion_jouable_s[20][10];
//int nbr_pion_j_s;
int multijoueur[2]={0, 0};
int multi_play = 0;
int ok=0;


void initialise(){
    SDL_Surface *surface;
    SDL_Event event;
    int i, j, c=0, ok=0;
    SDL_Rect menu[5];

    //creation du neccessaire
    window = SDL_CreateWindow("damier", 200, 200, 700, 600, SDL_WINDOW_SHOWN);
    rendere = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(rendere, 144, 100, 100, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(rendere);
    texture = SDL_CreateTexture(rendere, -1, SDL_TEXTUREACCESS_TARGET, 900, 900);
    SDL_SetRenderTarget(rendere, texture);
    //creation des textures
    surface = SDL_LoadBMP("pion_n.bmp");
    pion_n = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("pion_b.bmp");
    pion_b = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("case_n.bmp");
    noir_vid = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("case_b.bmp");
    blan = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("pion_b_select.bmp");
    pion_b_select = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("pion_n_select.bmp");
    pion_n_select = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("noir_pos.bmp");
    noir_pos = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("rois_b.bmp");
    roi_b = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("rois_n.bmp");
    roi_n = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("rois_b_s.bmp");
    roi_b_select = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("rois_n_s.bmp");
    roi_n_select = SDL_CreateTextureFromSurface(rendere, surface);
    surface = SDL_LoadBMP("fond_1.bmp");
    fond_1 = SDL_CreateTextureFromSurface(rendere, surface);
    //menu
    menu[0].x = 250;
    menu[0].y = 150;
    menu[0].h = 100;
    menu[0].w = 150;
    surface = SDL_LoadBMP("play.bmp");
    menu_1 = SDL_CreateTextureFromSurface(rendere, surface);
    SDL_RenderCopy(rendere, fond_1, NULL, NULL);
    SDL_RenderCopy(rendere, menu_1, NULL, &menu[0]);
    SDL_RenderPresent(rendere);
    do{
        SDL_WaitEvent(&event);
    }while(event.type!=SDL_MOUSEBUTTONDOWN && ok!=1);
    for(i=0; i<len_p; i++){
        for(j=0; j<len_p; j++){
            if(i<4){
                if(c==0){
                    plateau[j][i]=0;
                    c=1;}else{plateau[j][i]=1; c=0;
                    }
            }
            if(i>=4 && i<6){
                if(c==0){
                    plateau[j][i]=0;
                    c=1;}else{plateau[j][i]=-1; c=0;
                    }
            }
            if(i>=6){
                if(c==0){
                    plateau[j][i]=0;
                    c=1;}else{plateau[j][i]=2; c=0;
                    }
            }
            rect[i][j].x=100+i*cc;
            rect[i][j].y=100+j*cc;
            rect[i][j].h=cc;
            rect[i][j].w=cc;
        }
            if(c==1)
            c=0;
        else
            c=1;

    }
     for(i=0; i<len_p; i++){
        for(j=0; j<len_p; j++){
            if(plateau[i][j]==0)
                SDL_RenderCopy(rendere, blan, NULL, &rect[i][j]);
            if(plateau[i][j]==-1)
                SDL_RenderCopy(rendere, noir_vid, NULL, &rect[i][j]);
            if(plateau[i][j]==1)
                SDL_RenderCopy(rendere, pion_n, NULL, &rect[i][j]);
            if(plateau[i][j]==2)
                SDL_RenderCopy(rendere, pion_b, NULL, &rect[i][j]);
        }
     }
     //plateau[len_p/2-2][len_p/2-1]=6;
     SDL_RenderPresent(rendere);
     //on garde le sens de placement du joueur
if(joueur == 1)
dpl = -1;
else dpl = +1;
}

void print_dispo(){
    int i, j;
    printf("\n\n");
    for(i=0; i<len_p; i++){
        for(j=0; j<len_p; j++){
            printf("%3d", plateau[j][i]);
        }
        printf("\n");
        }
}

void present_posibl(int x_d, int y_d, int i_p, int j_p, int n){
    int i;
    if(plateau[i_p+x_d][j_p+y_d]==-1 && plateau[i_p][j_p]==joueur){//avance simplement
        plateau[i_p+x_d][j_p+y_d] = 5;
    }
    if((plateau[i_p+x_d][j_p+y_d]==advers || plateau[i_p+x_d][j_p+y_d]==advers+5) && (plateau[i_p][j_p]==joueur || plateau[i_p][j_p]==joueur+5) && plateau[i_p+2*x_d][j_p+2*y_d]==-1){
            plateau[i_p+2*x_d][j_p+2*y_d]=5;
            if(n==1){
                present_posibl(1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 1);
                present_posibl(-1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 1);}
         if(n==2){// cest un roi
                present_posibl(-1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
                present_posibl(1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
                present_posibl(1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
                present_posibl(-1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
            }
    }
    if((plateau[i_p+x_d][j_p+y_d]==advers || plateau[i_p+x_d][j_p+y_d]==advers+5) && plateau[i_p][j_p]==5){//fait un strite
        if(plateau[i_p+2*x_d][j_p+2*y_d]==-1){
            plateau[i_p+2*x_d][j_p+2*y_d] = 5;
            if(n==1){
            present_posibl(1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 1);
            present_posibl(-1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 1);}
            if(n==2){// cest un roi
                present_posibl(-1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
                present_posibl(1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
                present_posibl(1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
                present_posibl(-1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2);
            }
    }

       // printf("ok\n");
    }
    if(n==2 && plateau[i_p+x_d][j_p+y_d]==-1 && plateau[i_p][j_p]==joueur+5){//mouvement royal
        for(i=1; i_p+i*x_d>=0 && i_p+i*x_d<len_p && j_p+i*y_d>=0 && j_p+i*y_d<len_p; i++){
            if(plateau[i_p+i*x_d][j_p+i*y_d]==-1){
                plateau[i_p+i*x_d][j_p+i*y_d]=5;
            }
            else{
                if((plateau[i_p+i*x_d][j_p+i*y_d]==advers || plateau[i_p+i*x_d][j_p+i*y_d]==advers+5)&&plateau[i_p+(i+1)*x_d][j_p+(i+1)*y_d]==-1){
                    plateau[i_p+(i+1)*x_d][j_p+(i+1)*y_d]=5;
                    present_posibl(1, 1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2);
                    present_posibl(1, -1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2);
                    present_posibl(-1, 1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2);
                    present_posibl(-1, -1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2);}
                break;
            }
        }
    }

}

void indice(SDL_Event event){
    int i, j;
    SDL_Point point;
    point.x = event.button.x;
    point.y = event.button.y;
        clik[0] = -1;
    clik[1] = -1;
    for(i=0; i<len_p; i++){
        for(j=0; j<len_p; j++){
            if(SDL_PointInRect(&point, &rect[i][j]) && plateau[i][j]!=0){
                clik[0] = i;
                clik[1] = j;
                break;
            }

        }
    }
}

void refrech(){
    int i, j;
    //SDL_RenderClear(rendere);
    for(i=0; i<len_p; i++){
        for(j=0; j<len_p; j++){
            if(plateau[i][j]==0)
                SDL_RenderCopy(rendere, blan, NULL, &rect[i][j]);
            if(plateau[i][j]==-1)
                SDL_RenderCopy(rendere, noir_vid, NULL, &rect[i][j]);
            if(plateau[i][j]==1)
                SDL_RenderCopy(rendere, pion_n, NULL, &rect[i][j]);
            if(plateau[i][j]==2)
                SDL_RenderCopy(rendere, pion_b, NULL, &rect[i][j]);
            if(plateau[i][j]==3)
                 SDL_RenderCopy(rendere, pion_n_select, NULL, &rect[i][j]);
            if(plateau[i][j]==4)
                 SDL_RenderCopy(rendere, pion_b_select, NULL, &rect[i][j]);
            if(plateau[i][j]==5)
                 SDL_RenderCopy(rendere, noir_pos, NULL, &rect[i][j]);
            if(plateau[i][j]==6)
                 SDL_RenderCopy(rendere, roi_n, NULL, &rect[i][j]);
            if(plateau[i][j]==7)
                 SDL_RenderCopy(rendere, roi_b, NULL, &rect[i][j]);
            if(plateau[i][j]==8)
                 SDL_RenderCopy(rendere, roi_n_select, NULL, &rect[i][j]);
            if(plateau[i][j]==9)
                 SDL_RenderCopy(rendere, roi_b_select, NULL, &rect[i][j]);
        }
     }
    SDL_RenderPresent(rendere);
}

int verif_pion(int x_d, int y_d, int i_p, int j_p, int n, int plateau_[len_p][len_p]){
     int i, cote=0;
    if(plateau_[i_p+x_d][j_p+y_d]==-1 && plateau_[i_p][j_p]==advers){//avance simplement
        plateau_[i_p+x_d][j_p+y_d] = 5;
        pion_jouable[nbr_pion_j][2] = i_p+x_d;
        pion_jouable[nbr_pion_j][3] = j_p+y_d;
        if(cote==0)
            cote=1;
    }
    if(plateau_[i_p+x_d][j_p+y_d]==joueur && (plateau_[i_p][j_p]==advers || plateau_[i_p][j_p]==advers+5) && plateau_[i_p+2*x_d][j_p+2*y_d]==-1){
            plateau_[i_p+2*x_d][j_p+2*y_d]=5;
            pion_jouable[nbr_pion_j][2] = i_p+2*x_d;
            pion_jouable[nbr_pion_j][3] = j_p+2*y_d;
            cote = cote+2;
            if(n==1){
                cote +=verif_pion(-1*dpl, +1*dpl, i_p+2*x_d, j_p+2*y_d, 1, plateau_);
                cote +=verif_pion(1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 1, plateau_);}
         if(n==2){// cest un roi
                cote +=verif_pion(-1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
                cote +=verif_pion(1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
                cote +=verif_pion(1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
                cote +=verif_pion(-1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
            }
    }
    if((plateau_[i_p+x_d][j_p+y_d]==joueur || plateau_[i_p+x_d][j_p+y_d]==joueur+5) && plateau_[i_p][j_p]==5){//fait un strite
        if(plateau_[i_p+2*x_d][j_p+2*y_d]==-1){
            plateau_[i_p+2*x_d][j_p+2*y_d] = 5;
            pion_jouable[nbr_pion_j][2] = i_p+2*x_d;
            pion_jouable[nbr_pion_j][3] = j_p+2*y_d;
            cote = cote+3;;
            if(n==1){
            cote +=verif_pion(-1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 1, plateau_);
            cote +=verif_pion(1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 1, plateau_);}
            if(n==2){// cest un roi
                cote +=verif_pion(-1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
                cote +=verif_pion(1*dpl, 1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
                cote +=verif_pion(1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
                cote +=verif_pion(-1*dpl, -1*dpl, i_p+2*x_d, j_p+2*y_d, 2, plateau_);
            }
    }

        //printf("ok\n");
    }
    if(n==2 && plateau_[i_p+x_d][j_p+y_d]==-1 && plateau_[i_p][j_p]==advers+5){//mouvement royal
        for(i=1; i_p+i*x_d>=0 && i_p+i*x_d<len_p && j_p+i*y_d>=0 && j_p+i*y_d<len_p; i++){
            if(plateau_[i_p+i*x_d][j_p+i*y_d]==-1){
                plateau_[i_p+i*x_d][j_p+i*y_d]=5;
                 pion_jouable[nbr_pion_j][2] = i_p+i*x_d;
            pion_jouable[nbr_pion_j][3] = j_p+i*y_d;
            }
            if(cote==0)
                cote=1;
            else{
                if((plateau_[i_p+i*x_d][j_p+i*y_d]==joueur || plateau_[i_p+i*x_d][j_p+i*y_d]==joueur+5)&&plateau_[i_p+(i+1)*x_d][j_p+(i+1)*y_d]==-1){
                    plateau_[i_p+(i+1)*x_d][j_p+(i+1)*y_d]=5;
                     pion_jouable[nbr_pion_j][2] = i_p+(i+1)*x_d;
                    pion_jouable[nbr_pion_j][3] = j_p+(i+1)*y_d;
                    cote++;
                    cote +=verif_pion(1, 1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2, plateau_);
                    cote +=verif_pion(1, -1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2, plateau_);
                    cote +=verif_pion(-1, 1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2, plateau_);
                    cote +=verif_pion(-1, -1, i_p+(i+1)*x_d, j_p+(i+1)*y_d, 2, plateau_);}
                break;
            }
        }
    }
    return cote;
}

int bouf(int x, int y){
    if(plateau[x][y]==5 && (plateau[x+1][y+1]==advers || plateau[x+1][y+1]==advers+5) && (plateau[x+2][y+2]==5 || plateau[x+2][y+2]==joueur+2 || plateau[x+2][y+2]==joueur+7)){
        plateau[x][y] = -1;
        plateau[x+1][y+1]=-1;
        bouf(x+2, y+2);
        return 1;
    }
    if(plateau[x][y]==5 && (plateau[x-1][y+1]==advers || plateau[x-1][y+1]==advers+5) && (plateau[x-2][y+2]==5 || plateau[x-2][y+2]==joueur+2 || plateau[x-2][y+2]==joueur+7)){
        plateau[x][y] = -1;
        plateau[x-1][y+1]=-1;
        bouf(x-2, y+2);
        return 1;
    }
    if(plateau[x][y]==5 && (plateau[x+1][y-1]==advers || plateau[x+1][y-1]==advers+5) && (plateau[x+2][y-2]==5 || plateau[x+2][y-2]==joueur+2 || plateau[x+2][y-2]==joueur+7)){
        plateau[x][y] = -1;
        plateau[x+1][y-1]=-1;
        bouf(x+2, y-2);
        return 1;
    }
    if(plateau[x][y]==5 && (plateau[x-1][y-1]==advers || plateau[x-1][y-1]==advers+5) && (plateau[x-2][y-2]==5 || plateau[x-2][y-2]==joueur+2 || plateau[x-2][y-2]==joueur+7)){
        plateau[x][y] = -1;
        plateau[x-1][y-1]=-1;
        bouf(x-2, y-2);
        return 1;
    }
    if(plateau[x][y]==5 && plateau[x+1][y+1]==5){
        plateau[x][y] = -1;
        bouf(x+1, y+1);
        return 1;
    }
    if(plateau[x][y]==5 && plateau[x-1][y+1]==5){
        plateau[x][y] = -1;
        bouf(x-1, y+1);
        return 1;
    }
    if(plateau[x][y]==5 && plateau[x+1][y-1]==5){
        plateau[x][y] = -1;
        bouf(x+1, y-1);
        return 1;
    }
    if(plateau[x][y]==5 && plateau[x-1][y-1]==5){
        plateau[x][y] = -1;
        bouf(x-1, y-1);
        return 1;
    }
    return 0;

}

void auto_play(){
    int i, j, n;
    int plat_fict[len_p][len_p];
    nbr_pion_j=0;

     for(i=0; i<len_p; i++)
                for(j=0; j<len_p; j++){
                    plat_fict[i][j]=plateau[i][j];
                    }


    for(i=0; i<len_p; i++){
        for(j=0; j<len_p; j++){
            if(plat_fict[i][j]==advers)
                n=1;
            else if(plat_fict[i][j]==advers+5)
                n=2;
            if(plat_fict[i][j]==advers || plat_fict[i][j]==advers+5){
                pion_jouable[nbr_pion_j][0] = i;
                pion_jouable[nbr_pion_j][1] = j;
                pion_jouable[nbr_pion_j][4]=verif_pion(-1*dpl, 1*dpl, i, j, n, plat_fict);
                pion_jouable[nbr_pion_j][5] = n;
                nbr_pion_j++;
                pion_jouable[nbr_pion_j][0] = i;
                pion_jouable[nbr_pion_j][1] = j;
                pion_jouable[nbr_pion_j][4]=verif_pion(1*dpl, 1*dpl, i, j, n, plat_fict);
                pion_jouable[nbr_pion_j][5] = n;
                nbr_pion_j++;
                if(plat_fict[i][j]==advers+5){
                pion_jouable[nbr_pion_j][0] = i;
                pion_jouable[nbr_pion_j][1] = j;
                pion_jouable[nbr_pion_j][4]=verif_pion(1*dpl, -1*dpl, i, j, n, plat_fict);
                pion_jouable[nbr_pion_j][5] = n;
                nbr_pion_j++;
                pion_jouable[nbr_pion_j][0] = i;
                pion_jouable[nbr_pion_j][1] = j;
                pion_jouable[nbr_pion_j][4]=verif_pion(-1*dpl, -1*dpl, i, j, n, plat_fict);
                pion_jouable[nbr_pion_j][5] = n;
                nbr_pion_j++;
                }
            }
        }
    }
    //recherche de la meilleur bouf
    n=0;
    j=0;
    for(i=0; i<nbr_pion_j; i++){
        if(pion_jouable[i][4]>=n){
            n=pion_jouable[i][4];
            j=i;
        }
    }
    printf("okoh %d\n", n);
    //play
    i=joueur;
    joueur = advers;
    advers = i;
    dpl = (-1)*dpl;
    i = pion_jouable[j][5];
    if(i==1){
            present_posibl(-1*dpl, -1*dpl, pion_jouable[j][0], pion_jouable[j][1], 1);
            present_posibl(1*dpl, -1*dpl, pion_jouable[j][0], pion_jouable[j][1], 1);
            plateau[pion_jouable[j][0]][pion_jouable[j][1]] = joueur +2;
            }
                if(i==2){
            present_posibl(-1, -1, pion_jouable[j][0], pion_jouable[j][1], 2);
            present_posibl(1, -1, pion_jouable[j][0], pion_jouable[j][1], 2);
            present_posibl(1, 1, pion_jouable[j][0], pion_jouable[j][1], 2);
            present_posibl(-1, 1, pion_jouable[j][0], pion_jouable[j][1], 2);
            plateau[pion_jouable[j][0]][pion_jouable[j][1]] = joueur +7;
                }
                    refrech();
                    SDL_Delay(1000);
             n=bouf(pion_jouable[j][2], pion_jouable[j][3]);
        if(i==1)
            plateau[pion_jouable[j][2]][pion_jouable[j][3]] = joueur;
        else if(i==2)
            plateau[pion_jouable[j][2]][pion_jouable[j][3]] = joueur +5;

           plateau[pion_jouable[j][0]][pion_jouable[j][1]] =-1;
           SDL_Delay(50);
}

int move(){
    SDL_Event event;
    int i, j, n;
    int x, y;
    i = clik[0];
    j= clik[1];
    do{
        SDL_WaitEvent(&event);
    }while(event.type!=SDL_MOUSEBUTTONDOWN);
    indice(event);
    if(clik[0]!=-1){
        x = clik[0];
        y = clik[1];
        if(plateau[x][y]==5){
            n=bouf(x, y);
            joue = 1;
            refrech();
        }else return 0;
        if(plateau[i][j]==joueur+2)
            plateau[x][y]=joueur;
        else if(plateau[i][j]==joueur+7)
            plateau[x][y]=joueur+5;
                    if(n==1)
            plateau[i][j]=-1;
    }else return 0;

}

void end_coup(){
    int i, j;
    for(j=0; j<len_p; j++)
    for(i=0; i<len_p; i++){
        if(plateau[i][0]==2)
            plateau[i][0]=7;
        if(plateau[i][len_p-1]==1)
            plateau[i][len_p-1]=6;
            if(plateau[j][i]==5)
                plateau[j][i]=-1;
            if(joue==1){
        if(plateau[j][i]==joueur+2 || plateau[j][i]==joueur+7)
                plateau[j][i]=-1;
                }
                else{
                    if(plateau[j][i]==joueur+2)
                plateau[j][i]=joueur;
                    if(plateau[j][i]==joueur+7)
                plateau[j][i]=joueur+5;
                }
    }
}

int main(int argc, char *argv[]){
    SDL_bool quit=SDL_FALSE;
    SDL_Event event;
    int i=0;
    int u, v;
    int inter;
    int posi;
    if(0!=SDL_Init(SDL_INIT_VIDEO)){
        return EXIT_FAILURE;
    }
    initialise();
     print_dispo();
    while(!quit){
        SDL_WaitEvent(&event);
        if(event.type==SDL_QUIT)
            quit = SDL_TRUE;
        if(event.type==SDL_MOUSEBUTTONDOWN){
                indice(event);
                if(clik[0]!=-1){
                if(plateau[clik[0]][clik[1]]==joueur){
            present_posibl(-1*dpl, -1*dpl, clik[0], clik[1], 1);
            present_posibl(1*dpl, -1*dpl, clik[0], clik[1], 1);
            plateau[clik[0]][clik[1]] = joueur +2;
            }
                if(plateau[clik[0]][clik[1]]==joueur+5){
            present_posibl(-1, -1, clik[0], clik[1], 2);
            present_posibl(1, -1, clik[0], clik[1], 2);
            present_posibl(1, 1, clik[0], clik[1], 2);
            present_posibl(-1, 1, clik[0], clik[1], 2);
            plateau[clik[0]][clik[1]] = joueur +7;
                }
            refrech();
                joue = 0;
                i=move();
                end_coup();
                refrech();

                if(multi_play==0 && joue==1){

                                        auto_play();
                                        end_coup();
                                        SDL_Delay(50);
                                        refrech();
                                        inter=joueur;
                                        joueur=advers;
                                        advers=inter;
                                        dpl = dpl*-1;
                }
                if(multi_play==1 && joue==1){
                    inter=joueur;
                    joueur=advers;
                    advers=inter;
                    dpl = dpl*-1;
                }
                }
            print_dispo();
        }
        SDL_Delay(50);
    }
    return EXIT_SUCCESS;
}

