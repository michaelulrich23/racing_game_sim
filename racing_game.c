#include <stdio.h>
int max(int a, int b);

#define R_MAX 2147483647
static long long unsigned int SEED = 0x1;
void srnd(int seed) { SEED = seed; }

int rnd(int from, int to) {
    SEED = SEED * 16807 % R_MAX;
    return from + (int) SEED % (to - from + 1);
}
//-----------------------------------------------------------------------

int main(){
    int s, t, n;
    int game = 1, player = -1, opponent = -1, player_turn = 1, oldPosPlayer = -1, oldPosOpp = -1;
    int dices, dice1, dice2, distance, onBoard = 0, justSpawned = 0, oppOnBoard = 0;
    int round = 1, special = 0, elimination = 0, teleport = 0, winner;
    scanf("%d %d %d", &s, &t, &n);
    if(s <= 0 || n < 10 || n > 100 || (t*2 > n/2)) // !( s > 0 && n >= 10 && n <= 100 && (t*2 > n/2))
        return 1;
    int tunnels[t*2], visits[n];
    srnd(s);
    for(int i = 0; i<t*2 ; i=i+2) { //nesmie byť index 0 a menej, index n-1 a viac, rovnaky index
        scanf("%d %d", &tunnels[i], &tunnels[i + 1]);
        if(tunnels[i] <= 0 || tunnels[i] >= n-1 || tunnels[i+1] <= 0 || tunnels[i+1] >= n-1 || (tunnels[i] == tunnels[i+1]))
            return 2;/*
        for (int j = 0; j<t*2; j=j+2) { //rovnaky index vchodu
            if (tunnels[i] == tunnels[j] && t!=1 && j!=i)
                return 2;
        }
        for (int k = 1; k < t*2; k=k+2) { //vychod nesmie byt vchodom ineho tunela
            if(tunnels[i] == tunnels[k])
                return 2;
        }*/
    }
    for (int i = 0; i < t*2; i=i+2) {
        for (int j = 0; j<t*2; j=j+2) { //rovnaky index vchodu
            if (tunnels[i] == tunnels[j] && t!=1 && j!=i)
                return 2;
        }
        for (int k = 1; k < t*2; k=k+2) { //vychod nesmie byt vchodom ineho tunela
            if(tunnels[i] == tunnels[k])
                return 2;
        }
    }
    printf("TUNNELS:");
    if(t > 0){
        for (int i = 0; i < t*2-1; i=i+2){
            for (int j = 0; j < t*2-1; j=j+2){
                if (tunnels[j] > tunnels[i]){
                    int tmp = tunnels[i], tmp2 = tunnels[i+1];
                    tunnels[i] = tunnels[j];
                    tunnels[i+1] = tunnels[j+1];
                    tunnels[j] = tmp;
                    tunnels[j+1] = tmp2;
                }
            }
        }
        for (int i = 0; i < t*2-1; i=i+2) {
            printf(" [%d,%d]", tunnels[i], tunnels[i+1]);
        }
    }
    for (int i = 0; i < n; i++) {
        visits[i] = 0;
    }

    while(game){
        if(player_turn){
            dice1 = rnd(1, 6);
            dice2 = rnd(1, 6);
            dices = dice1 + dice2;
            if(dices > 7 && oldPosPlayer == -1){ //start do hry
                player = dices - 7;
                onBoard = 1;
                justSpawned = 1;
                if(t!=0){ for (int i = 0; i < t*2-1; i=i+2) if(player == tunnels[i]) break; else if(i == t*2-2 && player != tunnels[i]) visits[player]++;}
                else visits[player]++;
            }
            if(onBoard == 1 && justSpawned != 1){
                if(((dices == 12 && (oldPosPlayer < opponent)) || (dices == 2 && (oldPosPlayer > opponent))) && oppOnBoard == 1){ //special1 special2
                    oldPosPlayer = oldPosPlayer + opponent;
                    opponent = oldPosPlayer - opponent;
                    player = oldPosPlayer - opponent;
                    oldPosOpp = opponent;
                    oldPosPlayer = opponent;
                    visits[player]++;
                    visits[oldPosOpp]++;
                    special = 1;
                }
                else{ //posun
                    distance = max(dice1, dice2);
                    player = oldPosPlayer + distance;
                    if(t!=0){ for (int i = 0; i < t*2-1; i=i+2) if(player == tunnels[i]) break; else if(i == t*2-2 && player != tunnels[i]) visits[player]++;}
                    else if(player<n) visits[player]++;
                }
                if(player >= n){
                    game = 0;
                    winner = 1;
                }
            }
            for (int i = 0; i < t*2-1; i=i+2) { //teleport
                if(tunnels[i] == player){
                    player = tunnels[i+1];
                    teleport = 1;
                    visits[player]++;
                }
            }
            if(player == opponent && player != -1){ //eliminacia
                oldPosOpp = -1;
                opponent = -1;
                oppOnBoard = 0;
                elimination = 1;
            }
            printf("\n[%d,1] [%d] [%d,%d] [%d]", round, oldPosPlayer, dice1, dice2, player);
            if(special) printf(" S");
            else if(teleport && elimination) printf(" T E");
            else if(teleport) printf(" T");
            else if(elimination) printf(" E");
            justSpawned = 0;
            player_turn = 0;
            oldPosPlayer = player;
            round++;
            special = 0;
            teleport = 0;
            elimination = 0;
        }
        else{
            dice1 = rnd(1, 6);
            dice2 = rnd(1, 6);
            dices = dice1 + dice2;
            if(dices > 7 && oldPosOpp == -1){ //start do hry
                opponent = dices - 7;
                oppOnBoard = 1;
                justSpawned = 1;
                if(t!=0){for (int i = 0; i < t*2-1; i=i+2) if(opponent == tunnels[i]) break; else if(i == t*2-2 && opponent != tunnels[i]) visits[opponent]++;}
                else visits[opponent]++;
            }
            if(oppOnBoard == 1 && justSpawned != 1){
                if(((dices == 12 && (oldPosOpp < player)) || (dices == 2 && (oldPosOpp > player))) && onBoard == 1){ //special1 special2
                    oldPosOpp = oldPosOpp + player;
                    player = oldPosOpp - player;
                    opponent = oldPosOpp - player;
                    oldPosOpp = player;
                    oldPosPlayer = player;
                    visits[oldPosPlayer]++;
                    visits[opponent]++;
                    special = 1;
                }
                else{ //posun
                    distance = max(dice1, dice2);
                    opponent = oldPosOpp + distance;
                    if(t!=0){for (int i = 0; i < t*2-1; i=i+2) if(opponent == tunnels[i]) break; else if(i == t*2-2 && opponent != tunnels[i]) visits[opponent]++;}
                    else if(opponent<n) visits[opponent]++;
                }
                if(opponent >= n){
                    game = 0;
                    winner = 2;
                }
            }
            for (int i = 0; i < t*2-1; i=i+2) { //teleport
                if(tunnels[i] == opponent){
                    opponent = tunnels[i+1];
                    teleport = 1;
                    visits[opponent]++;
                }
            }
            if(opponent == player && opponent != -1){ //eliminacia
                oldPosPlayer = -1;
                player = -1;
                onBoard = 0;
                elimination = 1;
            }
            printf("\n[%d,2] [%d] [%d,%d] [%d]", round, oldPosOpp, dice1, dice2, opponent);
            if(special) printf(" S");
            else if(teleport && elimination) printf(" T E");
            else if(teleport) printf(" T");
            else if(elimination) printf(" E");
            justSpawned = 0;
            player_turn = 1;
            oldPosOpp = opponent;
            round++;
            special = 0;
            teleport = 0;
            elimination = 0;
        }
    }
    printf("\nWINNER: %d\nVISITS:", winner);
    for (int i = 0; i < n; i++) {
        printf(" %d", visits[i]);
    }

    return 0;
}

int max(int a, int b){
    if(a>b) return a;
    else return b;
}