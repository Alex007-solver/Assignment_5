# Advanced Artificial Intelligence & System Design Portfolio

Welcome to the comprehensive repository of core Artificial Intelligence algorithms, system architectures, and probabilistic graphical models. This project bridges the gap between classic symbolic AI (Expert Systems, Search Tree Algorithms) and modern data-driven architectures (Knowledge Graphs, Bayesian Inference, and Generative AI wrappers).

---

##  Table of Contents
1. [Game-Playing & Search Tree Algorithms (C)](#1-game-playing--search-tree-algorithms-c)
2. [AI Travel Planner: Knowledge-Based Expert System](#2-ai-travel-planner-knowledge-based-expert-system)
3. [Knowledge Graphs Architecture & Tooling](#3-knowledge-graphs-architecture--tooling)
4. [Probabilistic Graphical Models: Bayesian Networks](#4-probabilistic-graphical-models-bayesian-networks)
5. [Compilation and Execution Guide](#5-compilation-and-execution-guide)

---

## 1. Game-Playing & Search Tree Algorithms (C)
This module contains four separate implementations of classic adversarial search and reinforcement strategy paradigms, evaluated using a Tic-Tac-Toe state space environment.

* **Minimax (`minimax.c`):** A pure, exhaustive recursive backtracking algorithm simulating an optimal opponent by maximizing winning utility and minimizing opponent advantage.
* **Alpha-Beta Pruning (`alphabeta.c`):** An optimized execution variant of Minimax that structurally cuts off branches ($\alpha \ge \beta$) that will never be reached by rational play, dramatically decreasing tree traversal steps.
* **Heuristic Alpha-Beta (`heuristic_ab.c`):** A depth-limited iteration of Alpha-Beta that drops deep calculations after a fixed cut-off depth and substitutes the exact final score with a heuristic state-evaluation calculator (evaluating critical control sectors like centers and corners).
* **Monte-Carlo Tree Search (`mcts.c`):** A statistical, non-deterministic search algorithm that expands an active node tree dynamically via four core phases: Selection (using UCT formulas), Expansion, Simulation (random rollouts), and Backpropagation.

---

## 2. AI Travel Planner: Knowledge-Based Expert System
A complete technical system design detailing how to architect a factual, hallucination-free AI travel agent by substituting standard standalone Large Language Models with a **Knowledge-Based Expert System** framework.

### Architectural Blueprint
* **User Interaction Layer:** Captures unstructured customer requests and applies an NLP Intent Parser to structure the input into strict variables (Destination, Duration, Budget, Preferences).
* **Knowledge Integration Layer:** Integrates pre-existing, domain-expert semantic assets (Wine Ontologies, Gastronomy Bases, Tourist Attraction Geolocations) without building them from scratch.
* **Recommendation Engine:** Executes symbolic reasoning and graph-traversal queries (via Cypher or SPARQL) to generate highly personalized match lists.
* **Cost Assessment Layer:** Functions as a hard-constraint accountant, computing dynamic pricing limits and adjusting selections automatically if total costs breach bounds.
* **Itinerary Generation Layer:** Models geographical spatial grouping by solving a variation of the Traveling Salesperson Problem (TSP) to group activities smoothly into multi-day blocks.
* **GenAI Presentation Layer:** Pipes the final, deterministic data tables into a generative model template to wrap the verified output into a clean, conversational itinerary narrative.

---

## 3. Knowledge Graphs Architecture & Tooling
An in-depth evaluation of enterprise semantic structures and data pipeline processing methodologies.

### The Construction Pipeline
1.  **Data Ingestion:** Sourcing multimodal assets (Structured SQL, semi-structured JSON, unstructured PDFs).
2.  **Named Entity Recognition (NER):** Isolating explicit domain targets using deep semantic extraction models.
3.  **Relation Extraction:** Transforming text linkages into declarative semantic triples ($\text{Subject} \xrightarrow{\text{Predicate}} \text{Object}$).
4.  **Entity Resolution:** Normalizing and merging textual duplicates into unique, unsegmented node identifiers.
5.  **Graph Storage:** Writing relational webs securely into index-optimized graph storage engines.

### Key Platforms Evaluated
* **Graph DB Storage:** Neo4j, Amazon Neptune, Memgraph.
* **AI Graph Orchestrators:** Microsoft GraphRAG, LlamaIndex PropertyGraph, LangChain Transformers.
* **Semantic Ontologies:** Stanford's Protégé, Stardog.

---

## 4. Probabilistic Graphical Models: Bayesian Networks
An operational model exploring diagnostic reasoning under high uncertainty, featuring a practical implementation of a **Server Downtime Diagnostic System**.


### Network Structure

### Core Operations & Causal Modifications
The engine leverages joint probability factorization to calculate dynamic shifts across variables when partial data is introduced. It demonstrates **Explaining Away (Causal Pattern Modification)**: when an alarm is triggered but a secondary test confirms that the physical hardware is fine, the network logically alters the weights to isolate a `Cyberattack` as the most probable remaining cause for the infrastructure collapse.

---

## 5. Compilation and Execution Guide

### Compiling the C Algorithms
Ensure you have a standard GCC compiler configured in your system environment.

```bash
# Compile Standard Minimax
gcc -o minimax_test minimax.c

# Compile Alpha-Beta Pruning
gcc -o alphabeta_test alphabeta.c

# Compile Heuristic Depth-Limited Search
gcc -o heuristic_test heuristic_ab.c

# Compile Monte-Carlo Tree Search (Requires explicit math library linking)
gcc -o mcts_test mcts.c -lm
