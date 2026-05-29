#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define INF 999999
#define EMPTY 0
#define PLAYER_X 1
#define PLAYER_O -1

// A simple state representation
typedef struct {
    int board[9];
    int turn; // Whose turn is it? 1 (X) or -1 (O)
} GameState;

// Helper to check who won. Returns 1 for X, -1 for O, 0 for nobody (yet)
int check_winner(GameState *state) {
    int lines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // horizontal
        {0,3,6}, {1,4,7}, {2,5,8}, // vertical
        {0,4,8}, {2,4,6}           // diagonal
    };
    
    for (int i = 0; i < 8; i++) {
        int a = lines[i][0], b = lines[i][1], c = lines[i][2];
        if (state->board[a] != EMPTY && 
            state->board[a] == state->board[b] && 
            state->board[a] == state->board[c]) {
            return state->board[a];
        }
    }
    return EMPTY;
}

// Check if the board is full or someone won
bool is_game_over(GameState *state) {
    if (check_winner(state) != EMPTY) return true;
    for (int i = 0; i < 9; i++) {
        if (state->board[i] == EMPTY) return false;
    }
    return true;
}

// Fills an array with available spots and returns how many there are
int get_moves(GameState *state, int moves[]) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (state->board[i] == EMPTY) {
            moves[count++] = i;
        }
    }
    return count;
}

// Quick positional heuristic if we hit our depth limit
int eval_board(GameState *state) {
    int winner = check_winner(state);
    if (winner != EMPTY) return winner * 100; // prioritize winning above all

    int score = 0;
    // Center is usually good
    if (state->board[4] == PLAYER_X) score += 5;
    if (state->board[4] == PLAYER_O) score -= 5;
    
    // Corners are okay too
    int corners[] = {0, 2, 6, 8};
    for (int i = 0; i < 4; i++) {
        if (state->board[corners[i]] == PLAYER_X) score += 1;
        if (state->board[corners[i]] == PLAYER_O) score -= 1;
    }
    return score;
}

// Copy state and apply a move
GameState apply_move(GameState state, int move) {
    GameState next = state;
    next.board[move] = state.turn;
    next.turn = -state.turn; // flip turn
    return next;
}

// ==========================================
// 1. Standard Minimax
// ==========================================
int minimax(GameState state, bool is_max, int *best_move) {
    if (is_game_over(&state)) return check_winner(&state);

    int moves[9];
    int num_moves = get_moves(&state, moves);
    int top_move = -1;

    if (is_max) {
        int best_score = -INF;
        for (int i = 0; i < num_moves; i++) {
            int dummy;
            int score = minimax(apply_move(state, moves[i]), false, &dummy);
            if (score > best_score) {
                best_score = score;
                top_move = moves[i];
            }
        }
        if (best_move) *best_move = top_move;
        return best_score;
    } else {
        int best_score = INF;
        for (int i = 0; i < num_moves; i++) {
            int dummy;
            int score = minimax(apply_move(state, moves[i]), true, &dummy);
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
// 2. Alpha-Beta Pruning
// ==========================================
int alpha_beta(GameState state, int alpha, int beta, bool is_max, int *best_move) {
    if (is_game_over(&state)) return check_winner(&state);

    int moves[9];
    int num_moves = get_moves(&state, moves);
    int top_move = -1;

    if (is_max) {
        int best_score = -INF;
        for (int i = 0; i < num_moves; i++) {
            int dummy;
            int score = alpha_beta(apply_move(state, moves[i]), alpha, beta, false, &dummy);
            if (score > best_score) {
                best_score = score;
                top_move = moves[i];
            }
            if (best_score > alpha) alpha = best_score;
            if (beta <= alpha) break; // prune
        }
        if (best_move) *best_move = top_move;
        return best_score;
    } else {
        int best_score = INF;
        for (int i = 0; i < num_moves; i++) {
            int dummy;
            int score = alpha_beta(apply_move(state, moves[i]), alpha, beta, true, &dummy);
            if (score < best_score) {
                best_score = score;
                top_move = moves[i];
            }
            if (best_score < beta) beta = best_score;
            if (beta <= alpha) break; // prune
        }
        if (best_move) *best_move = top_move;
        return best_score;
    }
}

// ==========================================
// 3. Heuristic Alpha-Beta (Depth-limited)
// ==========================================
int heuristic_ab(GameState state, int depth, int alpha, int beta, bool is_max, int *best_move) {
    // Stop if we hit the depth limit or game over
    if (depth == 0 || is_game_over(&state)) return eval_board(&state);

    int moves[9];
    int num_moves = get_moves(&state, moves);
    int top_move = -1;

    if (is_max) {
        int best_score = -INF;
        for (int i = 0; i < num_moves; i++) {
            int dummy;
            int score = heuristic_ab(apply_move(state, moves[i]), depth - 1, alpha, beta, false, &dummy);
            if (score > best_score) {
                best_score = score;
                top_move = moves[i];
            }
            if (best_score > alpha) alpha = best_score;
            if (beta <= alpha) break;
        }
        if (best_move) *best_move = top_move;
        return best_score;
    } else {
        int best_score = INF;
        for (int i = 0; i < num_moves; i++) {
            int dummy;
            int score = heuristic_ab(apply_move(state, moves[i]), depth - 1, alpha, beta, true, &dummy);
            if (score < best_score) {
                best_score = score;
                top_move = moves[i];
            }
            if (best_score < beta) beta = best_score;
            if (beta <= alpha) break;
        }
        if (best_move) *best_move = top_move;
        return best_score;
    }
}

// ==========================================
// 4. Monte-Carlo Tree Search (MCTS)
// ==========================================
// Have to manage tree nodes manually in C. Fun stuff.
typedef struct Node {
    GameState state;
    struct Node *parent;
    struct Node **children;
    int num_children;
    
    int move_taken; 
    double wins;
    int visits;
    
    int untried[9];
    int num_untried;
} Node;

Node* new_node(GameState state, Node *parent, int move) {
    Node *n = malloc(sizeof(Node));
    n->state = state;
    n->parent = parent;
    n->children = NULL;
    n->num_children = 0;
    n->move_taken = move;
    n->wins = 0;
    n->visits = 0;
    n->num_untried = get_moves(&state, n->untried);
    return n;
}

void free_tree(Node *n) {
    if (!n) return;
    for (int i = 0; i < n->num_children; i++) {
        free_tree(n->children[i]);
    }
    free(n->children);
    free(n);
}

Node* best_uct(Node *n) {
    Node *best = NULL;
    double max_uct = -INF;
    
    for (int i = 0; i < n->num_children; i++) {
        Node *child = n->children[i];
        if (child->visits == 0) return child;

        double exploit = child->wins / child->visits;
        if (n->state.turn == PLAYER_O) exploit = -exploit; // O wants negative score
        
        double explore = 1.41 * sqrt(log(n->visits) / child->visits);
        double uct = exploit + explore;

        if (uct > max_uct) {
            max_uct = uct;
            best = child;
        }
    }
    return best;
}

double simulate(GameState state) {
    int moves[9];
    while (!is_game_over(&state)) {
        int n_moves = get_moves(&state, moves);
        int r = rand() % n_moves; // pick random move
        state = apply_move(state, moves[r]);
    }
    return (double)check_winner(&state);
}

int mcts(GameState start_state, int iter_limit) {
    Node *root = new_node(start_state, NULL, -1);

    for (int i = 0; i < iter_limit; i++) {
        Node *curr = root;
        
        // 1. Select
        while (curr->num_untried == 0 && curr->num_children > 0) {
            curr = best_uct(curr);
        }
        
        // 2. Expand
        if (curr->num_untried > 0) {
            int move = curr->untried[--curr->num_untried];
            Node *child = new_node(apply_move(curr->state, move), curr, move);
            
            curr->num_children++;
            curr->children = realloc(curr->children, curr->num_children * sizeof(Node*));
            curr->children[curr->num_children - 1] = child;
            curr = child;
        }
        
        // 3. Rollout / Simulate
        double result = simulate(curr->state);
        
        // 4. Backprop
        while (curr != NULL) {
            curr->visits++;
            curr->wins += result;
            curr = curr->parent;
        }
    }

    // Find the most visited node to return
    int best_move = -1, max_visits = -1;
    for (int i = 0; i < root->num_children; i++) {
        if (root->children[i]->visits > max_visits) {
            max_visits = root->children[i]->visits;
            best_move = root->children[i]->move_taken;
        }
    }

    free_tree(root); 
    return best_move;
}

// ==========================================
// Test Driver
// ==========================================
int main() {
    srand(time(NULL));

    // Setup a board where X can win immediately by playing at index 2
    // X | X | _
    // O | O | _
    // _ | _ | _
    GameState test = {
        .board = {
             1,  1,  0,
            -1, -1,  0,
             0,  0,  0
        },
        .turn = PLAYER_X
    };

    printf("Starting test suite...\n");
    printf("Expected best move is index 2 (top right corner)\n\n");

    int move, score;

    score = minimax(test, true, &move);
    printf("Minimax       -> Move: %d (Score: %d)\n", move, score);

    score = alpha_beta(test, -INF, INF, true, &move);
    printf("Alpha-Beta    -> Move: %d (Score: %d)\n", move, score);

    // Limit depth to 3
    score = heuristic_ab(test, 3, -INF, INF, true, &move);
    printf("Heur. A-B     -> Move: %d (Score: %d)\n", move, score);

    // Run 2000 simulations
    move = mcts(test, 2000);
    printf("MCTS          -> Move: %d\n", move);

    return 0;
}