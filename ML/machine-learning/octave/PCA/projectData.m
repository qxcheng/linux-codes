%投影：m x n -> m x k
function Z = projectData(X, U, K)

Z = zeros(size(X, 1), K);

U_reduce = U(:, 1:K);
Z = X * U_reduce;

end
