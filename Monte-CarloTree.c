#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define EMPTY 0
#define PLAYER_X 1   // Maximizer
#define PLAYER_O -1  // Minimizer

#define INF 99999.0

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
int check_winner(GameState *state) {
    int lines[8][3] = {
        {0,1,2}, {3,4,5}, {6,7,8}, // Rows
        {0,3,6}, {1,4,7}, {2,5,8}, // Cols
        {0,4,8}, {2,4,6}           // Diagonals
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

// Checks if the board is full or someone won
bool is_game_over(GameState *state) {
    if (check_winner(state) != EMPTY) return true;
    for (int i = 0; i < 9; i++) {
        if (state->board[i] == EMPTY) return false;
    }
    return true; // Draw
}

// Fills an array with available spot indices and returns how many there are
int get_legal_moves(GameState *state, int moves[]) {
    int count = 0;
    for (int i = 0; i < 9; i++) {
        if (state->board[i] == EMPTY) {
            moves[count++] = i;
        }
    }
    return count;
}

// Creates a new state with a move applied
GameState apply_move(GameState state, int move) {
    GameState next = state;
    next.board[move] = state.turn;
    next.turn = (state.turn == PLAYER_X) ? PLAYER_O : PLAYER_X;
    return next;
}

// ==========================================
// MCTS TREE NODE STRUCTURE
// ==========================================
typedef struct Node {
    GameState state;
    struct Node *parent;
    struct Node **children;
    int num_children;
    
    int move_taken; // The move that led to this node
    double wins;    // +1 for X win, -1 for O win, 0 for draw
    int visits;     // How many times this node was part of a simulation
    
    int untried[9];
    int num_untried;
} Node;

// Allocates memory for a new node
Node* create_node(GameState state, Node *parent, int move) {
    Node *n = (Node*)malloc(sizeof(Node));
    n->state = state;
    n->parent = parent;
    n->children = NULL;
    n->num_children = 0;
    n->move_taken = move;
    n->wins = 0.0;
    n->visits = 0;
    n->num_untried = get_legal_moves(&state, n->untried);
    return n;
}

// Recursively frees the memory used by the tree
void free_tree(Node *n) {
    if (!n) return;
    for (int i = 0; i < n->num_children; i++) {
        free_tree(n->children[i]);
    }
    if (n->children != NULL) free(n->children);
    free(n);
}

// ==========================================
// THE 4 PHASES OF MCTS
// ==========================================

// Phase 1: Selection (Using Upper Confidence Bound for Trees - UCT)
Node* select_best_uct_child(Node *node) {
    Node *best_child = NULL;
    double max_uct = -INF;
    
    for (int i = 0; i < node->num_children; i++) {
        Node *child = node->children[i];
        
        // If a child has never been visited, it must be selected immediately
        if (child->visits == 0) return child;

        // Exploitation: Average win rate
        double exploitation = child->wins / (double)child->visits;
        
        // If it is Player O's turn at the current node, they want to pick the child 
        // with the most negative score (since -1 is a win for O). We invert it for the UCT formula.
        if (node->state.turn == PLAYER_O) {
            exploitation = -exploitation;
        }
        
        // Exploration: Favors nodes with fewer visits
        double exploration = 1.414 * sqrt(log((double)node->visits) / (double)child->visits);
        
        double uct_value = exploitation + exploration;

        if (uct_value > max_uct) {
            max_uct = uct_value;
            best_child = child;
        }
    }
    return best_child;
}

// Phase 2: Expansion
Node* expand_tree(Node *node) {
    // Pop an untried move from the list
    int move = node->untried[--node->num_untried];
    
    // Create the new child state and node
    GameState next_state = apply_move(node->state, move);
    Node *child = create_node(next_state, node, move);
    
    // Add child to the parent's dynamic array
    node->num_children++;
    node->children = (Node**)realloc(node->children, node->num_children * sizeof(Node*));
    node->children[node->num_children - 1] = child;
    
    return child;
}

// Phase 3: Simulation (Random Playout)
double simulate_random_game(GameState state) {
    int moves[9];
    // Play completely random moves until the game ends
    while (!is_game_over(&state)) {
        int n_moves = get_legal_moves(&state, moves);
        int random_index = rand() % n_moves;
        state = apply_move(state, moves[random_index]);
    }
    return (double)check_winner(&state);
}

// Phase 4: Backpropagation
void backpropagate(Node *node, double result) {
    // Walk back up to the root, updating stats along the way
    while (node != NULL) {
        node->visits++;
        node->wins += result;
        node = node->parent;
    }
}

// ==========================================
// THE MAIN MCTS LOOP
// ==========================================
int mcts(GameState start_state, int iterations) {
    Node *root = create_node(start_state, NULL, -1);

    for (int i = 0; i < iterations; i++) {
        Node *current_node = root;
        
        // 1. Selection
        // Traverse down the tree as long as there are no untried moves
        while (current_node->num_untried == 0 && current_node->num_children > 0) {
            current_node = select_best_uct_child(current_node);
        }
        
        // 2. Expansion
        if (current_node->num_untried > 0) {
            current_node = expand_tree(current_node);
        }
        
        // 3. Simulation
        double simulation_result = simulate_random_game(current_node->state);
        
        // 4. Backpropagation
        backpropagate(current_node, simulation_result);
    }

    // Once iterations are done, pick the move that was visited the most
    // (High visits indicate the UCT algorithm found it consistently reliable)
    int best_move = -1;
    int max_visits = -1;
    
    for (int i = 0; i < root->num_children; i++) {
        if (root->children[i]->visits > max_visits) {
            max_visits = root->children[i]->visits;
            best_move = root->children[i]->move_taken;
        }
    }

    free_tree(root); // Clean up memory to prevent leaks
    return best_move;
}

// ==========================================
// MAIN DRIVER & TEST CASE
// ==========================================
int main() {
    // Seed the random number generator for our simulations
    srand(time(NULL));

    // Setup a specific test board:
    GameState test_game = {
        .board = {
             PLAYER_X,  PLAYER_X,  EMPTY,
             PLAYER_O,  PLAYER_O,  EMPTY,
             EMPTY,     EMPTY,     EMPTY
        },
        .turn = PLAYER_X // It is X's turn
    };

    printf("Starting Monte-Carlo Tree Search (MCTS) Test...\n");
    printf("Initial Board State:");
    print_board(test_game);
    printf("It is Player X's turn. Running 5000 random simulations...\n\n");

    // Run MCTS with 5,000 iterations
    int best_move = mcts(test_game, 5000);

    printf("MCTS Simulations Complete!\n");
    printf("---------------------------\n");
    printf("Best Move Index: %d\n", best_move);

    if (best_move != -1) {
        printf("\nBoard after AI plays move %d:", best_move);
        print_board(apply_move(test_game, best_move));
    }

    return 0;
}