#include <stdio.h>
#include <stdbool.h>

#define EMPTY 0
#define PLAYER_X 1   // Maximizer
#define PLAYER_O -1  // Minimizer

// Represents the Tic-Tac-Toe board
typedef struct {
    int board[9];
    int turn;
} GameState;

// Visual helper to print the board
void print_board(GameState state) {
    printf("\n");
    for (int i = 0; i < 9; i++) {
        if (state.board[i] == PLAYER_X) printf(" X ");
        else if (state.board[i] == PLAYER_O) printf(" O ");
        else printf(" _ ");
        
        if ((i + 1) % 3 == 0) printf("\n");
        else printf("|");
    }
    printf("\n");
}

// Checks if someone has won. Returns 1 for X, -1 for O, 0 for nobody
int check_winner(GameState state) {
    int lines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // Rows
        {0,3,6}, {1,4,7}, {2,5,8}, // Cols
        {0,4,8}, {2,4,6}           // Diagonals
    };
    
    for (int i = 0; i < 8; i++) {
        int a = lines[i][0], b = lines[i][1], c = lines[i][2];
        if (state.board[a] != EMPTY && 
            state.board[a] == state.board[b] && 
            state.board[a] == state.board[c]) {
            return state.board[a];
        }
    }
    return EMPTY;
}

// Checks if the board is full or someone won
bool is_game_over(GameState state) {
    if (check_winner(state) != EMPTY) return true;
    for (int i = 0; i < 9; i++) {
        if (state.board[i] == EMPTY) return false;
    }
    return true; // Draw
}

// Fills an array with available spot indices and returns how many there are
int get_legal_moves(GameState state, int moves[]) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (state.board[i] == EMPTY) {
            moves[count++] = i;
        }
    }
    return count;
}

// Creates a new state with a move applied
GameState apply_move(GameState state, int move) {
    GameState next_state = state;
    next_state.board[move] = state.turn;
    next_state.turn = (state.turn == PLAYER_X) ? PLAYER_O : PLAYER_X;
    return next_state;
}

// ==========================================
// THE MINIMAX ALGORITHM
// ==========================================
int minimax(GameState state, bool is_max, int *best_move) {
    // BASE CASE: If the game is over, return the score (+1 for X win, -1 for O win, 0 for draw)
    if (is_game_over(state)) {
        return check_winner(state);
    }

    int moves[9];
    int num_moves = get_legal_moves(state, moves);
    int top_move = -1;

    // MAXIMIZER (Player X)
    if (is_max) {
        int best_score = -9999; // Start with the lowest possible score
        for (int i = 0; i < num_moves; i++) {
            GameState child_state = apply_move(state, moves[i]);
            int dummy_move;
            int score = minimax(child_state, false, &dummy_move); // Call recursively for O
            
            if (score > best_score) {
                best_score = score;
                top_move = moves[i];
            }
        }
        if (best_move) *best_move = top_move;
        return best_score;
    } 
    // MINIMIZER (Player O)
    else {
        int best_score = 9999; // Start with the highest possible score
        for (int i = 0; i < num_moves; i++) {
            GameState child_state = apply_move(state, moves[i]);
            int dummy_move;
            int score = minimax(child_state, true, &dummy_move); // Call recursively for X
            
            if (score < best_score) {
                best_score = score;
                top_move = moves[i];
            }
        }
        if (best_move) *best_move = top_move;
        return best_score;
    }
}

// ==========================================
// MAIN DRIVER & TEST CASE
// ==========================================
int main() {
    // Setup a specific test board:
    // Indices:
    // 0 | 1 | 2
    // 3 | 4 | 5
    // 6 | 7 | 8
    
    GameState test_game = {
        .board = {
             PLAYER_X,  PLAYER_X,  EMPTY,
             PLAYER_O,  PLAYER_O,  EMPTY,
             EMPTY,     EMPTY,     EMPTY
        },
        .turn = PLAYER_X // It is X's turn
    };

    printf("Starting Minimax Test...\n");
    printf("Initial Board State:");
    print_board(test_game);
    printf("It is Player X's turn. Minimax is calculating the best move...\n\n");

    int best_move = -1;
    int score = minimax(test_game, true, &best_move);

    printf("Minimax Evaluation Complete!\n");
    printf("---------------------------\n");
    printf("Best Move Index: %d\n", best_move);
    
    if (score == 1) printf("Expected Outcome: Player X is guaranteed to WIN.\n");
    else if (score == -1) printf("Expected Outcome: Player X will LOSE.\n");
    else printf("Expected Outcome: The game will be a DRAW.\n");

    // Show the board after the AI makes its choice
    if (best_move != -1) {
        printf("\nBoard after AI plays move %d:", best_move);
        print_board(apply_move(test_game, best_move));
    }

    return 0;
}