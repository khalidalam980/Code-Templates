#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class SegmentTree2D{
  private:
  // seg[node_x][node_y] = sum of the rectangle represented by these two nodes
    vector<vector<ll>> seg;
  public:
    SegmentTree2D(ll n, ll m){
        // Each X-node contains a complete Y-segment tree
        seg.resize(4*n+1, vector<ll>(4*m+1, 0));
    }
    
    // Build the Y-tree for a fixed X-node
    void build_y(ll node_x, ll low_x, ll high_x, ll node_y, ll low_y, ll high_y, vector<vector<ll>> &mat){
        if(low_y == high_y){
            // Both dimensions are leaves -> this is one matrix cell
            if(low_x == high_x) seg[node_x][node_y] = mat[low_x][low_y];
            // Same Y-node, combine the two X children
            else seg[node_x][node_y] = seg[2*node_x+1][node_y] + seg[2*node_x+2][node_y];
            return;
        }
        
        // Normal segment-tree split in the Y dimension
        ll mid = low_y + (high_y - low_y)/2;
        build_y(node_x, low_x, high_x, 2*node_y+1, low_y, mid, mat);
        build_y(node_x, low_x, high_x, 2*node_y+2, mid+1, high_y, mat);
        // Merge the two Y children
        seg[node_x][node_y] = seg[node_x][2*node_y+1] + seg[node_x][2*node_y+2];
        return;
    }
    
    // Build the outer (X) segment tree
    void build_x(ll node_x, ll low_x, ll high_x, vector<vector<ll>> &mat){
        if(low_x == high_x){ // X is fixed -> build a complete Y-tree for this row
            build_y(node_x, low_x, high_x, 0, 0, mat[0].size()-1, mat);
            return;
        }
        
        ll mid = low_x + (high_x - low_x)/2;
        build_x(2*node_x+1, low_x, mid, mat);
        build_x(2*node_x+2, mid+1, high_x, mat);
        
        // After X children are built, construct the Y-tree for this X-node
        build_y(node_x, low_x, high_x, 0, 0, mat[0].size()-1, mat);
        return;
    }
    
    // Update the Y-tree belonging to one X-node
    void update_y(ll node_x, ll low_x, ll high_x, ll node_y, ll low_y, ll high_y, ll x, ll y, ll val){
        if(low_y == high_y){
            // Reached the actual cell (x, y)
            if(low_x == high_x) seg[node_x][node_y] += val;
            // X is internal -> recalculate from X children
            else seg[node_x][node_y] = seg[2*node_x+1][node_y] + seg[2*node_x+2][node_y];
            return;
        }
        
        ll mid = low_y + (high_y - low_y)/2;
        // Follow only the Y path containing column y
        if(y<=mid) update_y(node_x, low_x, high_x, 2*node_y+1, low_y, mid, x, y, val);
        else update_y(node_x, low_x, high_x, 2*node_y+2, mid+1, high_y, x, y, val);
        
        // Recalculate ancestors after the update
        seg[node_x][node_y] = seg[node_x][2*node_y+1] + seg[node_x][2*node_y+2];
        return;
    }
    
    // Update cell (x, y) in the 2D tree
    void update_x(ll node_x, ll low_x, ll high_x, ll x, ll y, ll val, vector<vector<ll>> &mat){
        if(low_x == high_x){ // Reached row x -> update its Y-tree
            update_y(node_x, low_x, high_x, 0, 0, mat[0].size()-1, x, y, val);
            return;
        }
        ll mid = low_x + (high_x - low_x)/2;
        // Follow only the X path containing row x
        if(x<=mid) update_x(2*node_x+1, low_x, mid, x, y, val, mat);
        else update_x(2*node_x+2, mid+1, high_x, x, y, val, mat);
        
        // Every X ancestor also needs its Y-tree updated
        update_y(node_x, low_x, high_x, 0, 0, mat[0].size()-1, x, y, val);
        return;
    }
    
    // Query the Y-range [y1, y2] from one fixed X-node
    ll query_y(ll node_x, ll node_y, ll low_y, ll high_y, ll y1, ll y2){
        // No overlap l r low high OR low high l r // y1 y2 low_y high_y OR low_y high_y y1 y2
        if(y2<low_y || high_y<y1) return 0;
        
        // Complete overlap l low high r // y1 low_y high_y y2
        if(y1<=low_y && high_y<=y2) return seg[node_x][node_y];
        
        // Partial overlap -> search both Y children
        ll mid = low_y + (high_y - low_y)/2;
        ll left = query_y(node_x, 2*node_y+1, low_y, mid, y1, y2);
        ll right = query_y(node_x, 2*node_y+2, mid+1, high_y, y1, y2);
        return left + right;
    }
    
    // Query rectangle [x1, x2] × [y1, y2]
    ll query_x(ll node_x, ll low_x, ll high_x, ll x1, ll y1, ll x2, ll y2, vector<vector<ll>> &mat){
        // No Overlap l r low high OR low high l r // x1 x2 low_x high_x OR low_x high_x x1 x2 No overlap in X -> this entire subtree is irrelevant
        if(x2<low_x || high_x<x1) return 0;
        
        // Complete Overlap l low high r // x1 low_x high_x x2 Complete X overlap -> now only need to query Y
        if(x1<=low_x && high_x<=x2){
            return query_y(node_x, 0, 0, mat[0].size()-1, y1, y2);
        }
        
        // Partial X overlap -> search both X children
        ll mid = low_x + (high_x - low_x)/2;
        ll left = query_x(2*node_x+1, low_x, mid, x1, y1, x2, y2, mat);
        ll right = query_x(2*node_x+2, mid+1, high_x, x1, y1, x2, y2, mat);
        return left + right;
    }
    
};

int main() {
	// your code goes here
	ll n, q;
	cin>>n>>q;
	vector<vector<ll>> mat(n, vector<ll>(n, 0));
	for(ll i=0;i<n;i++){
        for(ll j=0;j<n;j++){
            char c;
            cin>>c;
            if(c == '*') mat[i][j] = 1;
        }
    }
    
    SegmentTree2D sg(n, n);
    sg.build_x(0, 0, n-1, mat);
    
    while(q--){
        ll x1, y1, x2, y2;
        cin>>x1>>y1>>x2>>y2;
        x1--;y1--;x2--;y2--;
        
        cout<<sg.query_x(0, 0, n-1, x1, y1, x2, y2, mat)<<endl;
    }
    
    return 0;
}
