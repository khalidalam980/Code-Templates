# Data Structures and Methods for Query Types

| Query Type                        | Data Structure/Method             | Time Complexity (Query) | Time Complexity (Update) | Space Complexity |
|------------------------------------|-----------------------------------|-------------------------|--------------------------|------------------|
| **Static Sum Query**               | Prefix Sum                        | O(1)                    | O(n) **(preprocess)**         | O(n)             |
| **Static Min/Max Query**           | Sparse Table                      | O(1)                    | O(n log n)                | O(n log n)       |
| **Dynamic Sum Query (Point Updates)** | **Fenwick Tree** / Segment Tree     | O(log n)                | O(log n)                  | O(n)             |
| **Dynamic Min/Max Query (Point Updates)** | Segment Tree                    | O(log n)                | O(log n)                  | O(n)             |
| **Range Update with Point Query**  | **Difference Array**                  | O(n)                    | O(1)                      | O(n)             |
| **Range Update with Range Query**  | Segment Tree (**Lazy Propagation**)   | O(log n)                | O(log n)                  | O(n)             |
| **2D Range Sum Query**             | 2D Fenwick Tree / Prefix Sum      | O(log^2 n)              | O(log^2 n)                | O(n^2)           |
| **Dynamic Range GCD Query**        | Segment Tree                      | O(log n)                | O(log n)                  | O(n)             |
| **Range Mode Query**               | Mo’s Algorithm                    | O((n+q) sqrt(n))        | O((n+q) sqrt(n))          | O(n)             |
