load('D:\octave\logic\ex3data1');

m = length(y);
X = [ones(m, 1), X];

n = size(X,2);

allTheta = zeros(n, 10);

lambda = 0.1;

for i = 1:10,
    y2 = (y == i);
    theta = train(X, y2, lambda);
    allTheta(:, i) = theta;   
end;

[val, ind] = max(sigmoid(X(2800, :) * allTheta))

y(2800)