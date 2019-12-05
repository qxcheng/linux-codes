%寻找每个样本的聚类中心 
function idx = findClosestCentroids(X, centroids)

K = size(centroids, 1);

idx = zeros(size(X,1), 1);

m = size(X, 1);

for i = 1:m,
    vect = repmat(X(i, :), K, 1);
    [val, ind] = min(sum((vect - centroids) .^ 2, axis = 2));
    idx(i) = ind;
end;

idx = idx;

end

