using ll = long long;
class LazySegmentTree{
private:
    vector<ll> seg, lazy;
public:
    LazySegmentTree(int n){
        seg.resize(4*n+1, 0);
        lazy.resize(4*n+1, 0);
    }
    
    void build(int ind, int low, int high, vector<int> &arr){
        if(low == high){
            seg[ind] = arr[low];
            return;
        }
        int mid = low+(high-low)/2;
        build(2*ind+1, low, mid, arr);
        build(2*ind+2, mid+1, high, arr);
        seg[ind] = seg[2*ind+1]+seg[2*ind+2];
    }
    
    ll query(int ind, int low, int high, int l, int r){
        // Lazy Update if any
        if(lazy[ind] != 0){
            seg[ind] += (high-low+1)*lazy[ind];
            if(low != high){ // Still there is childrens to lazy propagate
                lazy[2*ind+1] += lazy[ind];
                lazy[2*ind+2] += lazy[ind];
            }
            lazy[ind] = 0;
        }
        
        // No overlap
        if(high<l || r<low){  // [{l,r} {low, hgih} || {low, hgih} {l,r}]
            return 0;     // 0^a = a
        }
        // Complete overlap
        if(l<=low && high<=r){  // [{l, low, high, r}]
            return seg[ind];
        }
        // Partial overlap
        int mid = low+(high-low)/2;
        ll left = query(2*ind+1, low, mid, l, r);
        ll right = query(2*ind+2, mid+1, high, l, r);
        return left+right;
    }
    
    void update(int ind, int low, int high, int l, int r, int val){
        // Lazy Update if any
        if(lazy[ind] != 0){
            seg[ind] += (high-low+1)*lazy[ind];
            if(low != high){ // Still there is childrens to lazy propagate
                lazy[2*ind+1] += lazy[ind];
                lazy[2*ind+2] += lazy[ind];
            }
            lazy[ind] = 0;
        }
        
        // No overlap
        if(high<l || low>r){
            return;
        }
        
        // Complete overlap
        if(low>=l && high<=r){
            seg[ind] += (high-low+1)*val;
            if(low != high){ // Still there is childrens to lazy propagate
                lazy[2*ind+1] += val;
                lazy[2*ind+2] += val;
            }
            return;
        }
        
        // Partial overlap
        int mid = low+(high-low)/2;
        update(2*ind+1, low, mid, l, r, val);
        update(2*ind+2, mid+1, high, l, r, val);
        seg[ind] = seg[2*ind+1]+seg[2*ind+2];
    }
};
