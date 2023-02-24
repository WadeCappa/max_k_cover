1) input k, m, e, and input set file name to script
2) run max_k_cover on m subsets of the input file, each returning k values 
    1) Each max_k_cover instance gets a $n/m$ sized vertex set
    2) Each max_k_cover returns a vector of size k
3) build a new vertex list from the original data (which should not have been modified) using the $m \times k$ seeds generated from the 'local' max_k_covers. 
4) run max_k_cover on this new dataset
