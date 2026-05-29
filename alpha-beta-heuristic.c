#include <stdio.h>
#include <stdbool.h>

#define EMPTY 0
#define PLAYER_X 1   // Maximizer
#define PLAYER_O -1  // Minimizer

#define INF 99999

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
// THE HEURISTIC EVALUATION FUNCTION
// ==========================================
// This is used when the depth limit is reached before the game is over.
int evaluate_board(GameState state) {
    int winner = check_winner(state);
    
    // A guaranteed win is heavily weighted
    if (winner == PLAYER_X) return 100;
    if (winner == PLAYER_O) return -100;

    int score = 0;
    
    // Heuristic Rule 1: Center control is highly valuable
    if (state.board[4] == PLAYER_X) score += 5;
    else if (state.board[4] == PLAYER_O) score -= 5;
    
    // Heuristic Rule 2: Corners are slightly valuable
    int corners[] = {0, 2, 6, 8};
    for (int i = 0; i < 4; i++) {
        if (state.board[corners[i]] == PLAYER_X) score += 1;
        else if (state.board[corners[i]] == PLAYER_O) score -= 1;
    }
    
    return score;
}

// ==========================================
// HEURISTIC ALPHA-BETA ALGORITHM
// ==========================================
int heuristic_alpha_beta(GameState state, int depth, int alpha, int beta, bool is_max, int *best_move) {
    // NEW BASE CASE: Stop if the game is actually over, OR if we hit our depth limit
    if (depth == 0 || is_game_over(state)) {
        return evaluate_board(state); // Return heuristic guess instead of exact win/loss
    }

    int moves[9];
    int num_moves = get_legal_moves(state, moves);
    int top_move = -1;

    // MAXIMIZER (Player X)
    if (is_max) {
        int best_score = -INF;
        for (int i = 0; i < num_moves; i++) {
            GameState child_state = apply_move(state, moves[i]);
            int dummy_move;
            
            // Pass depth - 1 to the next level down
            int score = heuristic_alpha_beta(child_state, depth - 1, alpha, beta, false, &dummy_move);
            
            if (score > best_score) {
                best_score = score;
                top_move = moves[i];
            }
            
            if (best_score > alpha) alpha = best_score;
            if (beta <= alpha) break; // Pruning cut-off
        }
        if (best_move) *best_move = top_move;
        return best_score;
    } 
    // MINIMIZER (Player O)
    else {
        int best_score = INF;
        for (int i = 0; i < num_moves; i++) {
            GameState child_state = apply_move(state, moves[i]);
            int dummy_move;
            
            // Pass depth - 1 to the next level down
            int score = heuristic_alpha_beta(child_state, depth - 1, alpha, beta, true, &dummy_move);
            
            if (score < best_score) {
                best_score = score;
                top_move = moves[i];
            }
            
            if (best_score < beta) beta = best_score;
            if (beta <= alpha) break; // Pruning cut-off
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
    GameState test_game = {
        .board = {
             PLAYER_X,  PLAYER_X,  EMPTY,
             PLAYER_O,  PLAYER_O,  EMPTY,
             EMPTY,     EMPTY,     EMPTY
        },
        .turn = PLAYER_X // It is X's turn
    };

    printf("Starting Heuristic Alpha-Beta Test...\n");
    printf("Initial Board State:");
    print_board(test_game);
    printf("It is Player X's turn. Calculating move with a Depth Limit of 3...\n\n");

    int best_move = -1;
    int max_depth = 3; // The algorithm will only look 3 moves ahead
    
    int score = heuristic_alpha_beta(test_game, max_depth, -INF, INF, true, &best_move);

    printf("Heuristic Evaluation Complete!\n");
    printf("---------------------------\n");
    printf("Best Move Index: %d\n", best_move);
    printf("Heuristic Score: %d (Values near 100 mean X is winning)\n", score);

    if (best_move != -1) {
        printf("\nBoard after AI plays move %d:", best_move);
        print_board(apply_move(test_game, best_move));
    }

    return 0;
}