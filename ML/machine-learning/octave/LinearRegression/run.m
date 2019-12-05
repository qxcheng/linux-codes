load('ex5data1.mat')

m = length(y);
[X_norm, mu, X_range] = featureNormalize(X);

X = [ones(m, 1), X_norm];

lambda = 0.1;

theta = train(X, y, lambda);

m_val = length(yval);
X_val = [ones(m_val, 1), (Xval - repmat(mu, m_val, 1)) ./ repmat(X_range, m_val ,1)];

prediction = X_val * theta;  
errorist = (prediction - yval) ./ yval 