function [J, grad] = costFunction(X, y, theta, lambda)

[m, n] = size(X);

h_theta = sigmoid(X * theta);

J = -1 / m * (y' * log(h_theta) + (1 - y)' * log(1 - h_theta)) ...
    + lambda / (2 * m) * (theta(2:end)' * theta(2:end));

grad = zeros(n, 1);

grad(1) = 1 / m * (X(:, 1)' * (h_theta - y));
grad(2:end) = 1 / m * (X(:, 2:end)' * (h_theta - y)) + lambda / m * theta(2:end);

grad = grad(:);

end   
    


