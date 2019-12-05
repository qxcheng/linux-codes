function theta = train(X, y, lambda)

initial_theta = zeros(size(X, 2), 1);

cost_function = @(t) costFunction(X, y, t, lambda);

options = optimset('MaxIter', 200, 'GradObj', 'on');

[optTheta, functionVal, exitFlag] = fminunc(cost_function, initial_theta, options);

theta = optTheta;

end

