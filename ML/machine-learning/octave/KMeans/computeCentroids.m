%重置聚类中心
function centroids = computeCentroids(X, idx, K)

[m n] = size(X);

centroids = zeros(K, n);

for i = 1:K,
    idx_i = (idx == i);
    num_i = sum(idx_i);
    centroids(i, :) = sum(X .* repmat(idx_i, 1, n)) / num_i;
end;

centroids = centroids;

end

