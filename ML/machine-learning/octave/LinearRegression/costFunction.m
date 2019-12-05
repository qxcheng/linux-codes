function [J, grad] = costFunction(X, y, theta, lambda)

[m, n] = size(X); 

J = 0;
grad = zeros(n, 1);

J = (X * theta - y)' * (X * theta - y) / (2 * m) + ...
    theta(2:end) * theta(2:end) * lambda / (2 * m);

grad(1) = (X * theta - y)' * X(:, 1) / m;
grad(2:end) = (X * theta - y)' * X(:, 2:end) / m + lambda / m * theta(2:end);

grad = grad(:);
end
