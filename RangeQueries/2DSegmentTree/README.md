# 2D Segment Tree

A C++ implementation of a **2D Segment Tree** for performing rectangle-sum queries on a 2D matrix.

This implementation is useful for understanding how a segment tree can be extended from **1 dimension to 2 dimensions**.

---

## 1. Problem

Given an `n × m` matrix, support queries of the form:

```text
(x1, y1) → (x2, y2)
```

and return the sum of all values inside that rectangle.

For example, given:

```text
0 1 0 0
1 0 1 1
1 1 0 0
1 1 1 1
```

the query:

```text
(2, 2) → (3, 4)
```

covers:

```text
0 1 1
1 0 0
```

so the answer is:

```text
3
```

---

# 2. Core Idea

A 1D segment tree divides one dimension into intervals.

A 2D segment tree does this **twice**:

```text
                 X Segment Tree
                       |
             +---------+---------+
             |                   |
          X interval          X interval
             |                   |
           Y Tree              Y Tree
          /     \             /     \
       Y range Y range     Y range Y range
```

In other words:

> **Every X-node contains a complete Y-segment tree.**

Therefore:

```cpp
seg[node_x][node_y]
```

stores the sum of the rectangle represented by:

```text
X interval of node_x
        ×
Y interval of node_y
```

This is the most important concept to remember.

---

# 3. Understanding the Indices

There are two types of variables in the implementation.

### Segment-tree node indices

```cpp
node_x
node_y
```

These represent positions inside the `seg` array.

For example:

```cpp
seg[node_x][node_y]
```

### Matrix coordinates

```cpp
low_x, high_x
low_y, high_y
```

These represent actual row/column ranges of the matrix.

For example:

```text
low_x = 2
high_x = 5

→ rows [2, 5]
```

### Important

Do not confuse:

```text
node_y  → segment-tree index
low_y   → matrix coordinate
```

This distinction is especially important while building the tree.

---

# 4. Structure of `seg`

The implementation uses:

```cpp
vector<vector<ll>> seg;
```

and allocates:

```cpp
seg.resize(4*n+1, vector<ll>(4*m+1, 0));
```

Why `4*n` and `4*m`?

A normal segment tree requires approximately `4*n` nodes.

Since we have a segment tree in both dimensions:

```text
X tree → 4n
Y tree → 4m
```

so we allocate approximately:

```text
4n × 4m
```

values.

---

# 5. Building the Tree

There are two build functions:

```cpp
build_x()
build_y()
```

## `build_x()`

This builds the outer segment tree over rows.

Conceptually:

```text
build_x()
    |
    +-- left X subtree
    |
    +-- right X subtree
    |
    +-- build Y tree for current X-node
```

For an X-leaf:

```cpp
if(low_x == high_x)
```

we have found one row, so we build the complete Y-tree for that row.

---

## `build_y()`

`build_y()` builds a normal segment tree over columns for a fixed X-node.

At the deepest level:

```cpp
if(low_y == high_y)
```

there is only one column.

If X is also a leaf:

```cpp
if(low_x == high_x)
```

then we have reached exactly one matrix cell:

```cpp
mat[low_x][low_y]
```

So:

```cpp
seg[node_x][node_y] = mat[low_x][low_y];
```

---

## The important part of `build_y()`

When X is **not** a leaf:

```cpp
seg[node_x][node_y] =
    seg[2*node_x+1][node_y] +
    seg[2*node_x+2][node_y];
```

This means:

> For the same Y-node, combine the corresponding values from the left and right X children.

Notice that we use:

```cpp
[node_y]
```

and **not**:

```cpp
[low_y]
```

because `node_y` is the Y-segment-tree node index.

---

# 6. Query

A rectangle query is:

```text
[x1, x2] × [y1, y2]
```

The query happens in two stages.

```text
query_x()
    |
    +-- find relevant X intervals
             |
             +-- query_y()
```

---

## `query_x()`

`query_x()` handles the row/X dimension.

It has the standard three cases.

### Case 1: No overlap

```cpp
if(x2 < low_x || high_x < x1)
    return 0;
```

The current X interval does not contribute anything.

---

### Case 2: Complete overlap

```cpp
if(x1 <= low_x && high_x <= x2)
```

The entire X interval is inside the query.

We don't need to go deeper in X.

Instead:

```cpp
query_y(...)
```

is called on this X-node.

This is the key step that makes the structure 2D.

---

### Case 3: Partial overlap

The current X interval only partially intersects the query.

So split it:

```cpp
left  = query_x(left child)
right = query_x(right child)
```

and return:

```cpp
left + right
```

---

# 7. `query_y()`

Once `query_x()` finds an X interval that is completely inside `[x1, x2]`, `query_y()` handles the Y dimension.

It follows the exact same three cases.

### No overlap

```cpp
if(y2 < low_y || high_y < y1)
    return 0;
```

### Complete overlap

```cpp
if(y1 <= low_y && high_y <= y2)
    return seg[node_x][node_y];
```

### Partial overlap

Search both Y children:

```cpp
left + right
```

---

# 8. Why There Is No Leaf Check in Query

You may notice that `query_x()` and `query_y()` don't explicitly check:

```cpp
if(low == high)
```

This is intentional.

The two cases before it already handle a leaf.

For a leaf, only two possibilities exist:

```text
outside query
    → No overlap

inside query
    → Complete overlap
```

A leaf cannot have a partial overlap.

Therefore:

```text
No overlap
Complete overlap
Partial overlap
```

is sufficient.

---

# 9. Example of a Query

Suppose we query:

```text
rows    [1, 3]
columns [2, 4]
```

The X-tree first decomposes:

```text
[1, 3]
```

into a small number of X-tree nodes.

For every completely covered X-node:

```text
        X-node
           |
           ↓
       query_y()
           |
      +----+----+
      |         |
    Y range   Y range
```

The Y-tree then finds the required columns.

The final result is the sum of all selected rectangles.

---

# 10. Update

The code also contains:

```cpp
update_x()
update_y()
```

although the `main()` currently only performs queries.

An update changes one cell:

```text
(x, y)
```

by:

```text
val
```

For example:

```cpp
update_x(..., x, y, +1, ...);
```

adds `1`.

And:

```cpp
update_x(..., x, y, -1, ...);
```

subtracts `1`.

---

## Update flow

The update travels down the X-tree:

```text
root X
   ↓
X child
   ↓
X child
   ↓
row x
```

At every X-node on this path, its Y-tree must also be updated.

So:

```text
update_x()
    ↓
update_y()
```

This is why the update takes:

```text
O(log² n)
```

---

# 11. Complexity

For an `n × m` matrix:

### Build

The X-tree has `O(n)` nodes.

Each X-node has a Y-tree containing `O(m)` nodes.

Therefore the number of stored values is approximately:

```text
O(n × m)
```

With this recursive implementation, construction involves the corresponding segment-tree traversal and is commonly described as:

```text
O(nm log m)
```

For a square `n × n` matrix:

```text
O(n² log n)
```

---

### Query

The X dimension takes:

```text
O(log n)
```

and for each relevant X-node, the Y query takes:

```text
O(log m)
```

Therefore:

```text
O(log n × log m)
```

For an `n × n` matrix:

```text
O(log² n)
```

---

### Update

Same reasoning:

```text
X traversal → O(log n)
Y traversal → O(log m)
```

Therefore:

```text
O(log n × log m)
```

or:

```text
O(log² n)
```

for a square matrix.

---

# 12. Example Input

```text
4 3
.*..
*.**
**..
****
2 2 3 4
3 1 3 1
1 1 2 2
```

The matrix is:

```text
. * . .
* . * *
* * . .
* * * *
```

The queries produce:

```text
5
1
2
```

---

# 13. Important Mental Model

When revisiting this code, remember this:

```text
                 2D QUERY
                    |
             +------+------+
             |             |
          X range       X range
             |             |
          X-node         X-node
             |             |
          Y query       Y query
             |             |
          Y range       Y range
             |             |
           answer        answer
             \             /
              +-----------+
                    |
                  total
```

The simplest way to remember the implementation is:

> **Segment tree on X, and inside every X-node, another segment tree on Y.**

---

# 14. Three Rules to Remember

### Rule 1

```cpp
seg[node_x][node_y]
```

represents a **rectangle**.

---

### Rule 2

For a query:

```text
First decompose X.
Then query Y inside each completely covered X-node.
```

---

### Rule 3

Keep these separate in your head:

```text
node_x / node_y
    → segment-tree indices

low_x / high_x / low_y / high_y
    → actual coordinate ranges
```

Most bugs in this implementation come from mixing these two concepts.

---

# 15. Current Implementation

The complete implementation is contained in the C++ source file accompanying this README.

It currently supports:

* 2D segment-tree construction
* Rectangle sum queries
* Point updates through `update_x()` / `update_y()`
* `O(log² n)` rectangle queries
* `O(log² n)` point updates

The `main()` currently demonstrates the **rectangle query** functionality.

```

This README is deliberately centered around the **mental model**, so when you revisit the code later, the `node_x/node_y` vs. `low_x/low_y` distinction and the **X → Y traversal** should be easy to reconstruct.
```
