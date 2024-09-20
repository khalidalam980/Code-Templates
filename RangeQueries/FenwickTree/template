#include <bits/stdc++.h>
using namespace std;

class FenwickTree{
private:
    vector<long long> fen;
public:
    FenwickTree(int n){
        fen.resize(n, 0);
    }
    
    void update(int i, int val){
        int n = fen.size();
        while(i<n){
            fen[i] += val;
            i += (i&(-i));
        }
    }
    
    long long sum(int i){
        long long result=0;
        while(i>0){
            result += fen[i];
            i -= (i&(-i));
        }
        return result;
    }
};
 
int main() {
	int n, q;
	cin>>n>>q;
	FenwickTree ft(n+1);
	
	vector<int> nums(n+1);
	for(int i=1;i<=n;i++){
	    cin>>nums[i];
	}
	
	for(int i=1;i<=n;i++){
	    ft.update(i, nums[i]);
	}
	
	while(q--){
	    int type;
	    cin>>type;
	    if(type == 1){
	        int k, u;
	        cin>>k>>u;
	        ft.update(k, u-nums[k]);
	        nums[k] = u;
	    }else{
	        int a, b;
	        cin>>a>>b;
	        cout<<ft.sum(b)-ft.sum(a-1)<<endl;
	    }
	}
	return 0;
}
