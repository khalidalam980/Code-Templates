class SparseTable{
private:
    vector<vector<int>> M;
    int k;
public:
    SparseTable(int n){
        k = log2(n); 
        M.resize(k+1, vector<int>(n));
    }
    
    void build(vector<int> &nums){
        int n = nums.size();
        M[0] = nums;
    	for(int j=1;j<=k;j++){
    	    for(int i=0;i+(1<<j)-1<n;i++){
    	        M[j][i] = min(M[j-1][i], M[j-1][i+(1<<(j-1))]);
    	    }
    	}
    }
    
    int RMQ(int l, int r){
        int count = r-l+1;
        int j = log2(count);
        return min(M[j][l], M[j][r-(1<<j)+1]);
    }
};
