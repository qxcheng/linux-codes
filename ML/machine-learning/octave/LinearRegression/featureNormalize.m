% 特征缩放和均值归一化
% 传入的 X 不应添加常数项

function [X_norm, mu, X_range] = featureNormalize(X)

m = length(X);

mu = sum(X) / m;

X_range = max(X) - min(X);

X_norm = (X - repmat(mu, m, 1)) ./ repmat(X_range, m, 1);
