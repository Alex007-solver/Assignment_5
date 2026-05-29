# AI-Based Travel Planner Architecture

The system must:
- Capture user preferences and constraints
- Integrate existing domain knowledge bases (Ontologies)
- Generate highly personalized recommendations
- Assess and optimize trip costs
- Compile a day by day itinerary

## This Architecture contains the following:
- [User Interaction Layer](#user-interaction-layer)
- [Knowledge Integration Layer](#knowledge-integration-layer)
- [Recommendation Engine](#recommendation-engine)
- [Cost Assessment Layer](#cost-assessment-layer)
- [Itinerary Generation Layer](#itinerary-generation-layer)

## User Interaction Layer
This layer handles all user communication and data collection. It serves as the bridge between natural human language and our structured database. 

It captures:
- Desired destination or region (e.g., Tuscany, Paris)
- Travel dates and duration (e.g., 3-day trip)
- Budget constraints (e.g., under $1000)
- Personal interests (e.g., adventure, history, culinary, wine tasting)
- Dietary restrictions (e.g., vegan, gluten-free)

An **Intent Parser** and **Entity Extractor** (powered by an LLM) translate this messy, conversational user input into a clean, structured JSON query object containing these explicit rules and constraints.

## Knowledge Integration Layer
The most important layer for reusing existing domain knowledge. Instead of building databases from scratch, this layer connects to established ontologies using Semantic Web technologies (RDF/OWL) or a unified Graph Database (like Neo4j). 

It integrates:
- **Wine Ontology:** Maps geographical regions to grape varieties, vineyards, flavor profiles, and expert food pairings (e.g., Chianti Classico $\rightarrow$ matches $\rightarrow$ Tuscany).
- **Tourist Places Ontology:** Categorizes physical locations by historical significance, geolocation coordinates, opening hours, and entry fees.
- **Food Recommendation:** Links local cuisines, popular restaurants, and specific dietary profiles to regional gastronomy records.

**Knowledge Integration Rule:** These separate knowledge bases are linked together via shared semantic edges. For example, a restaurant node from the Food KB is connected to a specific vineyard node from the Wine KB via a `SERVES_PAIRING` edge, anchoring both to a shared location node.

## Recommendation Engine
This layer filters the massive amount of knowledge base data down to what the user actually wants, operating as the core of our Expert System.

The flow:

A **Semantic Matcher** uses graph query languages (like SPARQL or Cypher) to pull matching real-world nodes from the graph. GenAI is used here to parse complex or ambiguous user requests (e.g., "I want a relaxing trip with good red wine and old castles") and map those fuzzy concepts directly to the strictly structured ontology nodes. A recommendation algorithm then ranks these valid options from 1 to 100 based on user preference matching.

## Cost Assessment Layer
This layer ensures the recommendations do not exceed the user's budget, functioning as a strict constraint verification engine. 

It handles:
- Dynamic pricing lookups for real-time expenses (flights, hotels via external APIs)
- Activity cost estimation calculated directly from fixed data in the Tourist Places ontology
- Trade-off calculations performed by an inference engine (e.g., if total projected costs exceed $1000, it automatically drops the lowest-ranked high-cost items and queries the Knowledge Graph for cheaper semantic equivalents, like swapping a luxury restaurant for an authentic local bistro)

## Itinerary Generation Layer
This layer takes the final approved recommendations and builds the chronological trip plan, converting abstract destinations into a realistic physical route. 

It handles:
- Travel time and physical distance calculations between geographic coordinates
- Logical grouping of activities via a spatiotemporal routing algorithm that solves a constrained variation of the Traveling Salesperson Problem (TSP) (e.g., ensuring all activities on Day 1 are clustered in the North part of the city to minimize transit times)
- Formatting the finalized data points into a synchronized time-slot layout

---

# Implementation Using GenAI
To make the AI Travel Planner act more natural and operate as a personalized agent, we use an LLM API for the final itinerary presentation. 

Hence the machine pipeline becomes:

The system takes the mathematically optimized, rigid JSON data from the itinerary generation layer and pipes it into a prompt template. The prompt is formatted to make the GenAI model speak like a human travel agent:

*"Act as a professional travel agent. Present this itinerary data in a friendly, exciting tone, highlighting why the selected wine tours match their preference for bold reds. Keep the constraints completely accurate to the provided data."*


# AI-Based Travel Planner: Step-by-Step Functional Walkthrough


---

## 1. User Interaction Layer (The Input)

The entry point of the system handles human communication and converts natural language desires into rigid, deterministic logic rules.

* **The Scenario:** A user inputs a raw textual query:  
  *"I want a 2-day romantic weekend trip to Tuscany. I love bold red wines and my budget is $300."*
* **The Structural Extraction:** A background Natural Language Understanding (NLU) component parses this unstructured prompt, extracting explicit target entities and constraints to build a structured parameters object:
  * **Location:** `Tuscany`
  * **Duration:** `2 Days`
  * **Preferences:** `[Bold Red Wine, Romantic]`
  * **Budget Constraint:** `$300`

---

## 2. Knowledge Integration Layer (The Wine Ontology)

Instead of executing a fuzzy web search or querying an ungrounded database, the system acts as a true Expert System by consulting a pre-built **Knowledge Graph (KG)**. This graph contains explicit, verified domain facts represented as nodes (entities) and edges (relational paths).

[Image of a knowledge graph schema linking wine regions, grape varieties, and tourist destinations]

The graph is pre-populated with highly specific, domain-expert assertions:
* A node called **"Sangiovese"** is connected to a property node called **"Bold Red"** via a `HAS_STYLE` edge.
* A specific wine class node called **"Chianti Classico"** is connected to the **"Sangiovese"** grape node via a `MADE_FROM` edge.
* A specific physical destination node called **"Castello di Ama Vineyard"** is connected to **"Chianti Classico"** via a `PRODUCES` edge.
* That exact same vineyard node is anchored geographically to the **"Tuscany"** region node via a `LOCATED_IN` edge.

---

## 3. Recommendation Engine (The Matching)

The system reasons through the established semantic pathways to align the user's explicit taste preferences with verified physical locations.

* **Graph Traversal:** The engine identifies the root concepts from the user's intent: `Bold Red` wine style and `Tuscany` geography.
* **Path Deductions:** It traces paths along the graph nodes backward to discover all logical intersections:  
  $$	ext{Bold Red} \longrightarrow 	ext{Sangiovese} \longrightarrow 	ext{Chianti Classico} \longrightarrow 	ext{Castello di Ama Vineyard}$$
* **Candidate Pool Generation:** The engine filters out unrelated items and forms a targeted pool of valid candidate experiences (vineyards, boutique wine bars, and authentic regional restaurants) that mathematically fit the specific taste profile.

---

## 4. Cost Assessment Layer (The Financial Filter)

This layer executes the strict constraint checking essential to an industrial expert system, operating like an absolute, automated accountant.

* **Factual Expense Lookups:** The system inspects each candidate node inside the verified pool to pull its real-world costs (e.g., fixed tasting fees, entry tickets, average menu prices).
* **The Resource Allocation Strategy:** It sequentially adds the costs of top-rated experiences. 
* **The Trade-Off Rule:** If the system attempts to book a premium, world-renowned vineyard tour costing **$200**, it immediately computes that only **$100** remains to cover the rest of the 2-day trip. Rather than breaking the budget, the inference engine automatically crawls alternative branches in the Knowledge Graph for a cheaper semantic equivalent—such as an authentic, highly-rated local wine bar that serves the *exact same* vintage or style of Chianti for a flat tasting fee of only **$40**.

---

## 5. Itinerary Generation Layer (The Map & Clock)

Once the system settles on an optimized selection of budget-compliant, high-matching venues, it coordinates physical logistics, time-blocks, and routing.

* **Spatiotemporal Calculation:** The system checks the explicit latitude and longitude coordinates attached to each chosen node in the graph.
* **Geographic Clustering:** It clusters nearby places together so the traveler spends time enjoying the region rather than driving inefficiently back and forth across vast distances.
* **Chronological Assignment:** It splits the items into realistic day-long packages that conform to waking hours:
  * **Day 1:** Concentrates on clustered wine bars and trattorias located in the northern valley.
  * **Day 2:** Transitions to open-air estate tours and tastings across the southern vineyards.

---

## 6. The GenAI Presentation Layer (The Output)

At this terminal stage, the planner has engineered a mathematically perfect, budget-validated, geographically optimized timeline. However, this data is still raw structural information (such as a rigid JSON file filled with coordinates, decimal ratings, and financial variables).

* **The Generative Handoff:** The system pipes this locked, hallucination-free itinerary data straight into a Large Language Model (GenAI) along with strict agent parameters:  
  *"Act as a professional, welcoming personal travel agent. Take this exact structured timeline data and translate it into a friendly, narrative trip plan. Highlight to the user that Castello di Ama was curated specifically to satisfy their taste for bold, high-tannin reds."*
* **The Conversational Translation:** The GenAI model reads the immutable data boundaries and transforms the raw numbers into a fluent, engaging narrative itinerary that perfectly mimics the conversational eloquence of a human guide while remaining 100% accurate to the real-world constraints.